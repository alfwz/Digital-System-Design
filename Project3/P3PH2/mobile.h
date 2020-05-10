#include <stdlib.h>
#include <iostream>
#include <fstream>


class mobile: public sc_module {
public:
	sc_in<bool> clock, network_busy, rx_ack, rx_img, img_req;
	sc_out<bool> tx_req, tx_sig, m_ack;
	
	SC_HAS_PROCESS(mobile);


	mobile(sc_module_name name, int ps, double bw, char* outname): sc_module(name), packet_size(ps), bandwidth(bw), filename(outname){
		SC_METHOD(sensor);
		sensitive << clock.pos();
		SC_THREAD(tx_process);
		SC_THREAD(rx_process);

		tuple_counter = 0;
		transmit_packet_counter = 0;
		image_packet_counter = 0;
		image_counter = 0;
		cur_ROI = 0;
		time = 0;
		st_time = 0;
		infile.open("gaze_out.txt");
		if(!infile){
			cerr << "ERROR: Unable to open input " << "gaze_out.txt" << endl;
			sc_stop();
		}
		outfile.open(filename);
		outfile << "Time, Bytes\n";
	}
private:
	int cur_ROI, gaze_x, gaze_y, new_ROI, packet_size;
	double time, st_time, bandwidth;
	int tuple_counter, transmit_packet_counter, image_packet_counter, image_counter;
	int image_data1[5][4] = { {50,20,400,320}, {50,370,450,1000},{470,20,600,900}, {670,40,950,550}, {680, 700, 1000, 1000}};
	int image_data2[4][4] = { {10, 10, 150, 700}, {300, 10, 980, 250}, {300, 270, 980, 700}, {10, 740, 950, 1000} }; 
	int image_data3[4][4] = { {10, 10, 260, 900}, {270, 10, 520, 1000}, {570, 20, 700, 950}, {730, 10, 950, 950} };
	int image_data4[7][4] = { {10, 10, 160, 1000}, {170, 10, 220, 1000}, {230, 10, 380, 1000}, {390, 10, 540, 1000}, {550, 10, 700, 1000}, {710, 10, 860, 1000}, {870, 10, 1010, 1000} };
	int image_data5[7][4] = { {10, 10, 1000, 160}, {10, 170, 1000, 220}, {10, 230, 1000, 380}, {100, 390, 1000, 540}, {10, 550, 1000, 700}, {10, 710, 1000, 860}, {10, 870, 1000, 1010} };

	char* filename;

	// input file
	ifstream infile;
	// output file
	ofstream outfile;

	// get gaze data & convert
	void sensor(){
		// advance time, assume 0.01 sec clock
		time += 0.01;
		// wait for image to be received before getting gaze data
		if(image_counter > 0){
			// acquire gaze data
			if(infile >> gaze_x >> gaze_y){
				// determine if gaze data is in ROI of current image
				switch(image_counter){
				case 1:
					for(int i = 0; i < 5; i++){
						if( gaze_x > image_data1[i][0] && gaze_y > image_data1[i][1] && gaze_x < image_data1[i][2] && gaze_y < image_data1[i][3]){
							new_ROI = i+1;
							break;
						}
						else{
							new_ROI = 0;
						}
					}
					break;
				case 2:
					for(int i = 0; i < 4; i++){
						if( gaze_x > image_data2[i][0] && gaze_y > image_data2[i][1] && gaze_x < image_data2[i][2] && gaze_y < image_data2[i][3]){
							new_ROI = i+1;
							break;
						}
						else{
							new_ROI = 0;
						}
					}
					break;
				case 3:
					for(int i = 0; i < 4; i++){
						if( gaze_x > image_data3[i][0] && gaze_y > image_data3[i][1] && gaze_x < image_data3[i][2] && gaze_y < image_data3[i][3]){
							new_ROI = i+1;
							break;
						}
						else{
							new_ROI = 0;
						}
					}
					break;
				case 4:
					for(int i = 0; i < 7; i++){
						if( gaze_x > image_data4[i][0] && gaze_y > image_data4[i][1] && gaze_x < image_data4[i][2] && gaze_y < image_data4[i][3]){
							new_ROI = i+1;
							break;
						}
						else{
							new_ROI = 0;
						}
					}
					break;
				case 5:
					for(int i = 0; i < 5; i++){
						if( gaze_x > image_data5[i][0] && gaze_y > image_data5[i][1] && gaze_x < image_data5[i][2] && gaze_y < image_data5[i][3]){
							new_ROI = i+1;
							break;
						}
						else{
							new_ROI = 0;
						}
					}
					break;
				default:
					break;
				}
				// if ROI changed
				if(new_ROI != cur_ROI){
					// check if prev gaze was in ROI, generate a tuple
					if(cur_ROI != 0){
						tuple_counter++;
					}
					// set the current ROI and start time
					// output time stamp and byte image to file
					outfile << sc_time_stamp() << ", " << tuple_counter * 24 + transmit_packet_counter * 24 * 20 + image_packet_counter * packet_size / 8 + image_counter * 1000000 << endl;
					cur_ROI = new_ROI;
					st_time = time;
				}
				// if there are 20 tuples, packetize
				if(tuple_counter == 20){
					transmit_packet_counter++;
					//storage[transmit_packet_counter++].data = ROI_data;
					tuple_counter = 0;
					outfile << sc_time_stamp() << ", " << tuple_counter * 24 + transmit_packet_counter * 24 * 20 + image_packet_counter * packet_size / 8 + image_counter * 1000000 << endl;
				}
			}
			else{
				// gaze data finished, stop simulation.
				cout << "Gaze data finished" << endl;
				outfile.close();
				infile.close();
				sc_stop();
			}
		}
	}	

	void tx_process(){
		while(1){
			// check if network is busy
			if(network_busy){
				// if not busy, check if packet can be sent
				if(transmit_packet_counter > 0){
					tx_req.write(true);
					// there are packets to be sent
					// wait for acknowledgement from server
					wait(rx_ack.posedge_event());
					// if given ok, transfer data
					tx_req.write(false);
					tx_sig.write(true);
					while(transmit_packet_counter > 0){
						// wait the time needed to transfer 1 packet (1 packet is 3840 bits)
						wait(3840 / bandwidth, SC_SEC);
						transmit_packet_counter--;
					}
					// once all available data has been transfered, tx_sig goes low
					tx_sig.write(false);
				}
				else{
				// no packet ready to send, wait random time
					wait(rand() % 100  + 7, SC_MS);
				}
			}
			else{
			// network is busy, wait random time
				wait(rand() % 100  + 7, SC_MS);
			}
		}
	}
	
	// receive image from server
	void rx_process(){
		m_ack.write(false);
		while(1){
			// wait for server to send image
			wait(img_req.posedge_event());
			m_ack.write(true);
			wait(rx_img.posedge_event());
			m_ack.write(false);
			wait(rx_img.negedge_event());
			image_packet_counter++;
			// once enough image packets have been received, change to next image
			// flush current tuples into new packet
			if(image_packet_counter == 8000000 / packet_size){
				image_packet_counter = 0;
				image_counter++;
				cur_ROI = 0;
				if(tuple_counter > 0){
					tuple_counter = 0;
					transmit_packet_counter++;
				}
				outfile << sc_time_stamp() << ", " << tuple_counter * 24 + transmit_packet_counter * 24 * 20 + image_packet_counter * packet_size / 8 + image_counter * 1000000 << endl;
			}
		}
	}
};													

