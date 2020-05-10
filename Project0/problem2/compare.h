#include "systemc.h"

SC_MODULE(compare){
	//I/O
	sc_in<sc_uint<8>> count1, count2;
	sc_in<bool> overflow1, overflow2, c_load1, C_load2;
	sc_out<bool> ended, c_hold;
	
	//process
	void prc_compare();
	//constructor
	SC_CTOR(compare){
	SC_METHOD(prc_compare);
	sensitive <<count1<<count2<<overflow1<<overflow2;
	}
};

void compare :: prc_compare(){
	if(count1==count2){
		ended=true;
		c_hold=true;
	}
	else if(overflow1||overflow2){
		ended=true;
		c_hold=true;
	}
	else{
		ended=false;
	}
	if(c_load1||c_load2){
		c_hold=false;
	}
}

	
	
	