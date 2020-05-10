#include <systemc.h>
#include <dp.h>
#include <cnt.h>

int sc_main(int argc, char *argv[]) {
	int load_pm[] = {0, 1, 2};
	int load_rf[] = {0, 1, 3};
	int load_dm[] = {0, 5, 9};
	sc_signal<sc_uint<16> > instr;
	sc_signal<bool> n, z, c, rf_en, dm_en;
	sc_signal<sc_bv<4> > ctrl;
	sc_signal<sc_bv<3> > alu_op;
	sc_signal<sc_uint<4> > rsrc_addr, rdest_addr;
	sc_signal<sc_uint<16> > imm;
	sc_signal<sc_bigint<160> > prog_addr;
	sc_clock clock("clock", 50, SC_NS);
	
	dp dp1("dp1", &load_pm[0], &load_rf[0], &load_dm[0]);
	dp1.ctrl(ctrl);
	dp1.rf_en(rf_en);
	dp1.dm_en(dm_en);
	dp1.alu_op(alu_op);
	dp1.rsrc_addr(rsrc_addr);
	dp1.rdest_addr(rdest_addr);
	dp1.imm(imm);
	dp1.clock(clock);
	dp1.prog_addr(prog_addr);
	dp1.instr(instr);
	dp1.n(n);
	dp1.z(z);
	dp1.c(c);

	cnt cnt1("cnt1");
	cnt1.instr(instr);
	cnt1.n(n);
	cnt1.z(z);
	cnt1.c(c);
	cnt1.rf_en(rf_en);
	cnt1.dm_en(dm_en);
	cnt1.ctrl(ctrl);
	cnt1.alu_op(alu_op);
	cnt1.rsrc(rsrc_addr);
	cnt1.rdest(rdest_addr);
	cnt1.imm(imm);
	cnt1.prog_addr(prog_addr);
	
	return 0;
}
