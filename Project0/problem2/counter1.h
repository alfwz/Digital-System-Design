#include "systemc.h"

SC_MODULE(counter1){
	//ports 
	sc_in<sc_uint<8>> in1;
	sc_in<bool> clock, load1, dec1;
	sc_out<sc_uint<8>> count1;
	sc_out<bool> overflow1;

	//process 
	void prc_counter1();
	
	//internal data declaration 
	sc_uint<8> count, count_temp;
	
	//constructor
	SC_CTOR(counter1){
		SC_METHOD(prc_counter1);
		sensitive<<clock.pos();
	}
};

void counter1 :: prc_counter1(){
	if(load1){
		count=in1;
		count_temp=count;
	}
	else if(dec1){
		count= count-1;
		count_temp=count;
	}
	
	if(count_temp<count){
		overflow1= true;
		count1=temp_count;
	}
}