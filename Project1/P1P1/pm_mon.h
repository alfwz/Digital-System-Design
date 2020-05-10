#include <systemc.h>

SC_MODULE(pm_mon){
	sc_in<sc_uint<16> > instr;

	void prc_mon(){
		cout<< "PM's instruction is "<< instr.read()<<endl;
	}

	SC_CTOR(pm_mon){
		SC_METHOD(prc_mon);
		sensitive<<instr;
	}
};
