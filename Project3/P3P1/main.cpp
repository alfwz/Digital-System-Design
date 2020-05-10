#include <systemc.h>
#include <mobile.h>
#include <server.h>
#include <monitor.h>


int sc_main(int argc, char* argv[]){
	sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );
	// set up 0.01 sec clock
	sc_clock clock("clock", 10, SC_MS);
	sc_signal<bool> tx_req1, tx_req2, rx_ack1, rx_ack2, network_busy;
	//sc_signal<packet> packet1, packet2;


	mobile m1("m1");
	m1.clock(clock);
	m1.network_busy(network_busy);
	m1.tx_req(tx_req1);
	m1.rx_ack(rx_ack1);
	//m1.tr_packet(packet1);

	mobile m2("m2");
	m2.clock(clock);
	m2.network_busy(network_busy);
	m2.tx_req(tx_req2);
	m2.rx_ack(rx_ack2);
	//m2.tr_packet(packet2);	

	server s1("s1");
	s1.clock(clock);
	s1.tx_req1(tx_req1);
	s1.tx_req2(tx_req2);
	s1.rx_ack1(rx_ack1);
	s1.rx_ack2(rx_ack2);
	s1.network_busy(network_busy);
	//s1.rx_data1(packet1);
	//s1.rx_data2(packet2);
	
	monitor mon("mon");
	mon.clock(clock);
	mon.rx_ack1(rx_ack1);
	mon.rx_ack2(rx_ack2);
	mon.tx_req1(tx_req1);
	mon.tx_req2(tx_req2);


	sc_start(SC_ZERO_TIME);
	sc_start();

	return 0;
}
