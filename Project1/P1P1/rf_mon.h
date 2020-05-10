#include <systemc.h>

SC_MODULE(rf_mon) {
	sc_in<sc_uint<16> > rd_data1, rd_data2;

	void prc_mon(){
		cout << "RF[1] is  " << rd_data1.read() << " RF[3] is " << rd_data2.read() << endl;
	}

	SC_CTOR(rf_mon){
		SC_METHOD(prc_mon);
		sensitive << rd_data1 << rd_data2;
	}
};
