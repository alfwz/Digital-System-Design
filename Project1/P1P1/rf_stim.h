#include <systemc.h>

SC_MODULE(rf_stim) {
	sc_out<bool> wr_en, clock;
	sc_out<sc_uint<16> > wr_data;
	sc_out<sc_uint<4> > rd_addr1, rd_addr2, wr_addr;

	void prc_stim(){
		wr_en.write(1);
		clock.write(0);
		rd_addr1.write(1);
		rd_addr2.write(3);
		wr_addr.write(0);
		wr_data.write(0);
		wait(5, SC_NS);
		clock.write(1);
		wait(10, SC_NS);
		sc_stop();
	}
	SC_CTOR(rf_stim){
		SC_THREAD(prc_stim);
	}
};
