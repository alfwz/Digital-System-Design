
class server: public sc_module{
public:
	sc_in<bool> tx_req1, tx_req2, tx_sig1, tx_sig2, m1_ack, m2_ack;
	sc_out<bool> rx_ack1, rx_ack2, network_busy, tx_img1, tx_img2, tx_img_req1, tx_img_req2;

	SC_HAS_PROCESS(server);
	server(sc_module_name name, int ps, double bw): sc_module(name), packet_size(ps), bandwidth(bw){
		img_size = 1000 * 1000 * 8;	// image size in bits 1000x1000x8
		// total time to send 1 image
		total_time = double(packet_size / bandwidth);
		SC_THREAD(rx_process);
		SC_THREAD(tx_process);
	}

private:
	// packet size is in bits, bandwidth in bits/sec
	int packet_size, img_size;
	double bandwidth, total_time;
	void rx_process(){
		rx_ack1.write(false);
		rx_ack2.write(false);
		while(1){
			// wait for mobile devices to request transfer
			wait(tx_req1.posedge_event() | tx_req2.posedge_event());
			// check which device sent request
			if(tx_req1){
				rx_ack1.write(true);
				network_busy.write(false);
				wait(tx_sig1.posedge_event());
				rx_ack1.write(false);
				// wait for transfer to finish
				wait(tx_sig1.negedge_event());	
			}
			if(tx_req2){
				rx_ack2.write(true);
				network_busy.write(false);
				wait(tx_sig2.posedge_event());
				rx_ack2.write(false);
				// wait for transfer to finish
				wait(tx_sig2.negedge_event());
			}
			// transfer finished, not busy
			network_busy.write(true);
		}
	}

	// send image data
	void tx_process(){
		network_busy.write(true);
		tx_img_req1.write(false);
		tx_img_req2.write(false);
		while(1){
			// send image by 200 sec
			wait((200 - 2*total_time), SC_SEC);
			network_busy.write(false);
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req1.write(true);
				wait(m1_ack.posedge_event());
				// send 1 packet
				tx_img_req1.write(false);
				tx_img1.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img1.write(false);
				wait(SC_ZERO_TIME);
			}
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req2.write(true);
				wait(m2_ack.posedge_event());
				// send 1 packet
				tx_img_req2.write(false);
				tx_img2.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img2.write(false);
				wait(SC_ZERO_TIME);
			}
			network_busy.write(true);
			// send image by 220 sec
			wait((20 - 2*total_time), SC_SEC);
			network_busy.write(false);
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req1.write(true);
				wait(m1_ack.posedge_event());
				// send 1 packet
				tx_img_req1.write(false);
				tx_img1.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img1.write(false);
				wait(SC_ZERO_TIME);
			}
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req2.write(true);
				wait(m2_ack.posedge_event());
				// send 1 packet
				tx_img_req2.write(false);
				tx_img2.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img2.write(false);
				wait(SC_ZERO_TIME);
			}
			network_busy.write(true);
			wait(SC_ZERO_TIME);
			// send image by 225 sec
			wait((5 - 2*total_time), SC_SEC);
			network_busy.write(false);
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req1.write(true);
				wait(m1_ack.posedge_event());
				// send 1 packet
				tx_img_req1.write(false);
				tx_img1.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img1.write(false);
				wait(SC_ZERO_TIME);
			}
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req2.write(true);
				wait(m2_ack.posedge_event());
				// send 1 packet
				tx_img_req2.write(false);
				tx_img2.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img2.write(false);
				wait(SC_ZERO_TIME);
			}
			network_busy.write(true);
			wait(SC_ZERO_TIME);
			// send image by 230 sec
			wait((5 - 2*total_time), SC_SEC);
			network_busy.write(false);
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req1.write(true);
				wait(m1_ack.posedge_event());
				// send 1 packet
				tx_img_req1.write(false);
				tx_img1.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img1.write(false);
				wait(SC_ZERO_TIME);
			}
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req2.write(true);
				wait(m2_ack.posedge_event());
				// send 1 packet
				tx_img_req2.write(false);
				tx_img2.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img2.write(false);
				wait(SC_ZERO_TIME);
			}
			network_busy.write(true);
			wait(SC_ZERO_TIME);
			// send image by 250 sec
			wait((20 - 2*total_time), SC_SEC);
			network_busy.write(false);
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req1.write(true);
				wait(m1_ack.posedge_event());
				// send 1 packet
				tx_img_req1.write(false);
				tx_img1.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img1.write(false);
				wait(SC_ZERO_TIME);
			}
			for(int i = 0; i < img_size / packet_size; i++){
				// send request and wait for acknowledgement
				tx_img_req2.write(true);
				wait(m2_ack.posedge_event());
				// send 1 packet
				tx_img_req2.write(false);
				tx_img2.write(true);
				wait(double(packet_size / bandwidth), SC_SEC);
				tx_img2.write(false);
				wait(SC_ZERO_TIME);
			}
			network_busy.write(true);
			wait(SC_ZERO_TIME);
			wait();
		}
	}
};
