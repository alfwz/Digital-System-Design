//#include <packet.h>

class server: public sc_module{
public:
	sc_in<bool> clock, tx_req1, tx_req2;
	//sc_in<packet> rx_data1, rx_data2;
	sc_out<bool> rx_ack1, rx_ack2, network_busy;

	SC_HAS_PROCESS(server);
	server(sc_module_name name): sc_module(name){
		SC_CTHREAD(rx_process, clock.pos());

	}

private:
	void rx_process(){
		while(1){
			rx_ack1.write(false);
			rx_ack2.write(false);
			network_busy.write(true);
			// wait for mobile devices to request transfer
			wait(tx_req1.posedge_event() | tx_req2.posedge_event());
			// check which device sent request
			if(tx_req1){
				rx_ack1.write(true);
				network_busy.write(false);
				// wait for transfer to finish
				wait(tx_req1.negedge_event());	
			}
			if(tx_req2){
				rx_ack2.write(true);
				network_busy.write(false);
				// wait for transfer to finish
				wait(tx_req2.negedge_event());
			}
			// transfer finished, not busy
			network_busy.write(true);
		}
	}

};
