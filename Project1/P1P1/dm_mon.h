#include <systemc.h>

SC_MODULE(dm_mon){
	sc_in<sc_uint<16> > data_out;
	
	void prc_mon(){
		cout << "DM[1] is  " << data_out.read() << endl;
	}

	SC_CTOR(dm_mon){
		SC_METHOD(prc_mon);
		sensitive<< data_out;
	}	
};
