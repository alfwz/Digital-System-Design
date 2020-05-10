#include <systemc.h>
#include <string>
#include <mobile.h>
#include <server.h>
#include <monitor.h>


int sc_main(int argc, char* argv[]){
	sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );
	// set up 0.01 sec clock
	sc_clock clock("clock", 0.1, SC_SEC);
	sc_signal<bool> tx_req1, tx_req2, rx_ack1, rx_ack2, img_req1, img_req2, m1_ack, m2_ack, tx_img1, tx_img2, tx_sig1, tx_sig2;
	sc_signal<bool, SC_MANY_WRITERS> network_busy;
	int ps = 1000000;
	double bw = 512000;
	char out1[] = "output1.csv";
	char out2[] = "output2.csv";


	mobile m1("m1", ps, bw, out1);
	m1.clock(clock);
	m1.network_busy(network_busy);
	m1.tx_req(tx_req1);
	m1.tx_sig(tx_sig1);
	m1.rx_ack(rx_ack1);
	m1.rx_img(tx_img1);
	m1.img_req(img_req1);
	m1.m_ack(m1_ack);
	

	mobile m2("m2", ps, bw, out2);
	m2.clock(clock);
	m2.network_busy(network_busy);
	m2.tx_req(tx_req2);
	m2.tx_sig(tx_sig2);
	m2.rx_ack(rx_ack2);
	m2.rx_img(tx_img2);
	m2.img_req(img_req2);
	m2.m_ack(m2_ack);

	server s1("s1", ps, bw);
	s1.tx_req1(tx_req1);
	s1.tx_req2(tx_req2);
	s1.tx_sig1(tx_sig1);
	s1.tx_sig2(tx_sig2);
	s1.rx_ack1(rx_ack1);
	s1.rx_ack2(rx_ack2);
	s1.network_busy(network_busy);
	s1.tx_img_req1(img_req1);
	s1.tx_img_req2(img_req2);
	s1.m1_ack(m1_ack);
	s1.m2_ack(m2_ack);
	s1.tx_img1(tx_img1);
	s1.tx_img2(tx_img2);
	
	monitor mon("mon");
	mon.rx_ack1(rx_ack1);
	mon.rx_ack2(rx_ack2);
	mon.tx_req1(tx_req1);
	mon.tx_req2(tx_req2);
	mon.tx_sig1(tx_sig1);
	mon.tx_sig2(tx_sig2);
	mon.img_req1(img_req1);
	mon.img_req2(img_req2);
	mon.m1_ack(m1_ack);
	mon.m2_ack(m2_ack);
	mon.tx_img1(tx_img1);
	mon.tx_img2(tx_img2);

	sc_start(SC_ZERO_TIME);
	sc_start();

	return 0;
}
