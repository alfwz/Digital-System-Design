#include "systemc.h"

SC_MODULE(driver){
	sc_out<sc_uint<8>> d_in1,d_in2,d_in3;
	sc_out<bool> d_load1,d_load2, d_dec1, d_dec2;
	
	void prc_driver();
	
	SC_CTOR(driver){
		SC_THREAD(prc_driver);
	}
};

void driver::prc_driver(){
	
	d_in1=7;
	d_in2=8;
	d_in3=9;
	wait(1,SC_NS);
	//load1 load2 high
	d_load1=true;
	d_load2=true;
	wait(1,SC_NS);
	//dec1 dec2 high, load1 load2 low
	d_dec1=true;
	d_dec2=true;
	d_load1=false;
	d_load2=false;
	wait(1,SC_NS);
	//
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
	d_dec1=true;
	d_dec2=true;
	wait(1,SC_NS);
}