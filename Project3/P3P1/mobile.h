//#include <tuple.h>
//#include <packet.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

class mobile: public sc_module {
public:
	sc_in<bool> clock, network_busy, rx_ack;
	sc_out<bool> tx_req;
	//sc_out<packet> tr_packet;
	
	int tuple_counter, transmit_packet_counter;
	SC_HAS_PROCESS(mobile);


	mobile(sc_module_name name): sc_module(name){
		SC_METHOD(sensor);
		sensitive << clock.pos();
		SC_CTHREAD(tx_process, clock.pos());
		// assume bandwidth is 512 kbps
		// meaning a clock of 0.01 sec will move 5120 bits
		bandwidth = 5120;
		tuple_counter = 0;
		transmit_packet_counter = 0;
		cur_ROI = 0;
		time = 0;
		st_time = 0;
		infile.open("gaze_out.txt");
		if(!infile){
			cerr << "ERROR: Unable to open input " << "gaze_out.txt" << endl;
			sc_stop();
		}
		outfile.open("counter_data.csv");
		outfile << "Time, Tuple Count, Packet Count\n";
	}
private:
	int cur_ROI, gaze_x, gaze_y, new_ROI;
	double time, st_time, bandwidth;
	int image_data[5][4] = {{50,20,400,320}, {50,370,450,1000},{470,20,600,900}, {670,40,950,550}, {680, 700, 1000, 1000}};
	//tuple ROI_data[20];
	//tuple temp;
	// store packets
	//packet storage[100];

	// input file
	ifstream infile;
	// output file
	ofstream outfile;

	// get gaze data & convert
	void sensor(){
		// advance time, assume 0.01 sec clock
		time += 0.01;
		// acquire gaze data
		if(infile >> gaze_x >> gaze_y){
			// determine if gaze data is in ROI
			for(int i = 0; i < 5; i++){
				if( gaze_x > image_data[i][0] && gaze_y > image_data[i][1] && gaze_x < image_data[i][2] && gaze_y < image_data[i][3]){
					new_ROI = i+1;
					break;
				}
				else{
					new_ROI = 0;
				}
			}
			// if ROI changed
			if(new_ROI != cur_ROI){
				// check if prev gaze was in ROI, generate a tuple
				if(cur_ROI != 0){
					/*temp.ROI = cur_ROI;
					temp.start_time = st_time;
					temp.end_time = time;
					ROI_data[tuple_counter++] = temp;*/
					tuple_counter++;
				}
				// set the current ROI and start time
				outfile << sc_time_stamp() << ", " << tuple_counter << ", " << transmit_packet_counter << endl;
				cur_ROI = new_ROI;
				st_time = time;
			}
			// if there are 20 tuples, packetize
			if(tuple_counter == 20){
				transmit_packet_counter++;
				//storage[transmit_packet_counter++].data = ROI_data;
				tuple_counter = 0;
				outfile << sc_time_stamp() << ", " << tuple_counter << ", " << transmit_packet_counter << endl;
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
					while(transmit_packet_counter > 0){
						//tr_packet.write(storage[--transmit_packet_counter];
						// bandwidth at 512kbps means 1 cycle is more than enough to transfer 1 packet
						transmit_packet_counter--;
						wait(1);
					}
					// once all available data has been transfered, tx_req goes low
					tx_req.write(false);
				}
				else{
				// no packet ready to send, wait random time
					wait(rand() % 100  + 7);
				}
			}
			else{
			// network is busy, wait random time
				wait(rand() % 100  + 7);
			}
		}
	}
};													

