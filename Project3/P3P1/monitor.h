#include <systemc.h>

SC_MODULE(monitor){
	// monitors the handshake events
	sc_in<bool> clock, rx_ack1, rx_ack2, tx_req1, tx_req2;
	SC_CTOR(monitor){
		SC_CTHREAD(prc_mobile1_handshake, clock.pos());
		SC_CTHREAD(prc_mobile2_handshake, clock.pos());
	}

	void prc_mobile1_handshake(){
		while(1){
			wait(tx_req1.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer data request" << endl;
			wait(rx_ack1.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer acknowledged, recieving" << endl;
			wait(tx_req1.negedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 1 transfer finished" << endl;
		}
	}

	void prc_mobile2_handshake(){
		while(1){
			wait(tx_req2.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer data request" << endl;
			wait(rx_ack2.posedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer acknowledged, recieving" << endl;
			wait(tx_req2.negedge_event());
			cout << "@" << sc_time_stamp() << " Mobile 2 transfer finished" << endl;
		}
	}
	
};
