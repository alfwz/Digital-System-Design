#include "systemc.h"
#include "dm_stim.h"
#include "dm.h"
#include "dm_mon.h"


int sc_main(int argc, char*argv[]){
	sc_signal<bool> wr_en;
	sc_signal<sc_uint<16> > addr, data_in;
	sc_signal<sc_uint<16> > data_out;
	
	
	int reg[]={0,4,5,6};
	dm dm1("dm1", &reg[0]);
	dm1.wr_en(wr_en);
	dm1.addr(addr);
	dm1.data_in(data_in);
	dm1.data_out(data_out);

	dm_stim stim1("stim1");
	stim1.wr_en(wr_en);
	stim1.addr(addr);
	stim1.data_in(data_in);

	
	dm_mon mon1("mon1");
	mon1.data_out(data_out);
	
	sc_start();
	
	return 0;
}
