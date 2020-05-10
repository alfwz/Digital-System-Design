#include <systemc.h>

SC_MODULE(monitor){
	// monitors the handshake events
	sc_in<bool> rx_ack1, rx_ack2, tx_req1, tx_req2, img_req1, img_req2, m1_ack, m2_ack, tx_sig1, tx_sig2, tx_img1, tx_img2;
	SC_CTOR(monitor){
		SC_THREAD(prc_mobile1_handshake);
		SC_THREAD(prc_mobile2_handshake);
		SC_THREAD(prc_img1_handshake);
		SC_THREAD(prc_img2_handshake);
	}

	void prc_mobile1_handshake(){
		while(1){
			wait(tx_req1.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer data request" << endl;
			wait(rx_ack1.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer acknowledged, receiving" << endl;
			wait(tx_sig1.negedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer finished" << endl;
		}
	}

	void prc_mobile2_handshake(){
		while(1){
			wait(tx_req2.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer data request" << endl;
			wait(rx_ack2.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer acknowledged, receiving" << endl;
			wait(tx_sig2.negedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer finished" << endl;
		}
	}

	void prc_img1_handshake(){
		while(1){
			wait(img_req1.posedge_event());
			cout << "@" << sc_time_stamp() << " Request to send image packet to Mobile 1" << endl;
			wait(m1_ack.posedge_event());
			cout << "@" << sc_time_stamp() << " Image packet transfer acknowledged, Mobile 1 receiving" << endl;
			wait(tx_img1.negedge_event());
			cout << "@" << sc_time_stamp() << " Packet transfer finished" << endl;
		}
	}

	void prc_img2_handshake(){
		while(1){
			wait(img_req2.posedge_event());
			cout << "@" << sc_time_stamp() << " Request to send image packet to Mobile 2" << endl;
			wait(m2_ack.posedge_event());
			cout << "@" << sc_time_stamp() << " Image packet transfer acknowledged, Mobile 2 receiving" << endl;
			wait(tx_img2.negedge_event());
			cout << "@" << sc_time_stamp() << " Packet transfer finished" << endl;
		}
	}
	
};
