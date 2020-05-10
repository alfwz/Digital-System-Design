//================================
//Function: Sequence Detector
//File name: seq_det.h
//=================================

#include "systemc.h"

SC_MODULE(seq_det){
	//ports 
	sc_in<bool> clock, reset, clear;
	sc_in<bool> data_in;
	sc_out<bool> data_out;
	
	//process
	void prc_seq_det();
	void prc_data_out();
	//internal data declaration 
	sc_signal<bool> first, second, third, fourth;

	//constructor 
	SC_CTOR(seq_det){
		//process declaration 
		SC_METHOD(prc_seq_det);
			//sensitivities 
			sensitive_pos<<clock;
		SC_METHOD(prc_data_out);
			sensitive<<first<<second<<third<<fourth;
	}
};

void seq_det ::prc_seq_det(){
	if(reset){
		first=false;
		second=false;
		third=false;
		fourth=false;
	}
	else{
		first=data_in;
		second=first;
		third=second;
		fourth=third;
	}
}

void seq_det::prc_data_out(){
	if(clear){
		data_out=false;
	}
	else{
		data_out=first&second&(!third)&fourth;
	}
}



