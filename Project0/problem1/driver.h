#include "systemc.h"
	
SC_MODULE(driver){
	sc_out<bool> d_data_in;
	sc_out<bool> d_clear, d_reset;
	
	void data_in_driver();
	void clear_driver();
	void reset_driver();
	
	SC_CTOR(driver){
		SC_THREAD(data_in_driver);
	}
};


void driver :: data_in_driver(){

	
	wait(2,SC_NS);
	d_clear=false;
	d_reset=false;
	wait(2,SC_NS);

	d_data_in=true;
	wait(2,SC_NS);
	d_data_in=false;
	wait(2,SC_NS);
	d_data_in=true;
	wait(2,SC_NS);
	d_data_in=true;
	wait(2,SC_NS);
	d_data_in=false;
	
	
	d_clear=true;
	d_reset=true;

	
}

	
