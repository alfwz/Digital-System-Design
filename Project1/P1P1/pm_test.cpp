#include <systemc.h>
#include <pm.h>
#include <pm_stim.h>
#include <pm_mon.h>

int sc_main(int argc, char *argv[]){
	sc_signal<sc_bigint<160> > prog_addr;
	sc_signal<sc_uint<16> > instr;

	int array[]={0,4,5,6};
	pm pm1("pm1", &array[0]);
	pm1.prog_addr(prog_addr);
	pm1.instr(instr);

	pm_stim stim1("stim1");
	stim1.prog_addr(prog_addr);
	

	pm_mon mon1("mon1");
	mon1.instr(instr);

	sc_start();
	return 0;
}
