#include <systemc.h>

SC_MODULE(dm_stim){
	sc_out<bool>wr_en;
	sc_out<sc_uint<16> > addr, data_in;
	
	void prc_stim(){
		wr_en.write(1);
		data_in.write(0);
		addr.write(1); 
		wait(5, SC_NS);
		sc_stop();
	}
	SC_CTOR(dm_stim){
		SC_THREAD(prc_stim);
	}
};
