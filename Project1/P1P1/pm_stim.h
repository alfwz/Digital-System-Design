#include <systemc.h>

SC_MODULE(pm_stim){
	sc_out<sc_bigint<160> > prog_addr;

	void prc_stim(){
		prog_addr.write(3);

		wait(5,SC_NS);
		sc_stop();
	}
	SC_CTOR(pm_stim){
		SC_THREAD(prc_stim);
	}
};
