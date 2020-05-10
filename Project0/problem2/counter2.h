#include "systemc.h"

SC_MOODULE(counter2){
	sc_in<sc_uint<8>> in2, in3;
	sc_in<bool> clock, load2,dec2,c2_hold;
	sc_out<sc_uint<8>> count2;
	sc_out<bool> overflow2;
	
	void prc_counter2(){
		sc_uint<8> count, count_in3,count_temp;
	}
	
	SC_CTOR(counter2){
		SC_METHOD(prc_counter2);
		sensitive<<clock.pos();
	}
}；

void counter2 ：： prc_counter2(){
		count_in3=in3;
		if(load2){
			count=in2;
		}
		else if(c2_hold){
		}
		else if(dec2){
			count_temp=count;
			count=count-count_in3;
		}
		
		count2=count;
		if(count_temp<count){
			overflow2=true;
			count2=count_temp;
		}
}
			
	