#include <systemc.h>
#include <rf.h>
#include <rf_stim.h>
#include <rf_mon.h>

int sc_main(int argc, char *argv[]){
	sc_signal<bool> wr_en, clock;
	sc_signal<sc_uint<16> > wr_data;
	sc_signal<sc_uint<4> > rd_addr1, rd_addr2, wr_addr;
	sc_signal<sc_uint<16> > rd_data1, rd_data2;
	
	int reg[] = {0, 4, 5, 6};
	rf rf1("rf1", &reg[0]);
	rf1.clock(clock);
	rf1.wr_en(wr_en);
	rf1.wr_data(wr_data);
	rf1.rd_addr1(rd_addr1);
	rf1.rd_addr2(rd_addr2);
	rf1.rd_data1(rd_data1);
	rf1.rd_data2(rd_data2);
	rf1.wr_addr(wr_addr);
	
	rf_stim stim1("stim1");
	stim1.wr_en(wr_en);
	stim1.wr_data(wr_data);
	stim1.rd_addr1(rd_addr1);
	stim1.rd_addr2(rd_addr2);
	stim1.wr_addr(wr_addr);
	stim1.clock(clock);
	
	rf_mon mon1("mon1");
	mon1.rd_data1(rd_data1);
	mon1.rd_data2(rd_data2);

	sc_start();
	
	
	return 0;	
}

