#include <systemc.h>

SC_MODULE(driver){
	sc_out< bool > d_reset, d_clear;
	sc_out< sc_uint<12> > d_indata;
	
	void prc_driver();

	SC_CTOR(driver){
		SC_THREAD(prc_driver);
	}
};

void driver :: prc_driver(){
	
	d_reset=false;
	d_clear=true;
	wait(1, SC_NS);
	d_reset=true;
	d_clear=false;
	wait(1, SC_NS);
	
	d_indata=0b000011110000;
	wait(1, SC_NS);
	d_indata=0b000111110001;
	wait(1, SC_NS);
	d_indata=0b000101110000;
	wait(1, SC_NS);
	d_indata=0b000011100000;
	wait(1, SC_NS);
	}
