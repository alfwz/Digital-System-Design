#include <systemc.h>
#include <dp.h>
#include <cnt.h>

int sc_main(int argc, char *argv[]) {
	// pm, rf, dm initialization
	
	int load_pm[] = {0x4203, 0, 0x4405, 0, 0x5811,0x0452,0x4607, 0, 0x980f,0x4205, 0, 0x0258, 0x4843, 0, 0x4445,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int load_rf[16] = {};
	int load_dm[100] = {};
	load_dm[10] = 36;
	load_dm[12] = 12;
	load_dm[14] = -24;
	load_rf[3] = 10;
	load_rf[5] = 12;
	load_rf[7] = 14;
	load_rf[8] = 12;
	sc_signal<sc_uint<16> > instr;
	sc_signal<sc_bit> n, z, c, rf_en, dm_rd_en, dm_wr_en;
	sc_signal<sc_bv<4> > ctrl;
	sc_signal<sc_bv<3> > alu_op;
	sc_signal<sc_uint<4> > rsrc_addr, rdest_addr, rf_wr_addr;
	sc_signal<sc_int<16> > imm, alu_rdest, alu_rsrc, mem_out, alu_out, wb_data, rf_src_data, rf_dest_data, mem_wr_data;
	sc_signal<sc_uint<16> > mem_addr;
	sc_signal<sc_bigint<160> > prog_addr;
	sc_clock clock("clock", 50, SC_NS);
	
	dp dp1("dp1", &load_pm[0], &load_rf[0], &load_dm[0]);
	dp1.ctrl(ctrl);
	dp1.rf_en(rf_en);
	dp1.dm_rd_en(dm_rd_en);
	dp1.dm_wr_en(dm_wr_en);
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
	dp1.rdest_in(alu_rdest);
	dp1.rsrc_in(alu_rsrc);
	dp1.rf_wr_addr(rf_wr_addr);
	dp1.result_out(alu_out);
	dp1.mem_out(mem_out);
	dp1.result_in(wb_data);
	dp1.rdest_out(rf_dest_data);
	dp1.rsrc_out(rf_src_data);
	dp1.mem_addr(mem_addr);
	dp1.mem_wr_data(mem_wr_data);

	cnt cnt1("cnt1");
	cnt1.instr(instr);
	cnt1.n(n);
	cnt1.z(z);
	cnt1.c(c);
	cnt1.rf_en(rf_en);
	cnt1.dm_rd_en(dm_rd_en);
	cnt1.dm_wr_en(dm_wr_en);
	cnt1.ctrl(ctrl);
	cnt1.alu_op(alu_op);
	cnt1.rsrc(rsrc_addr);
	cnt1.rdest(rdest_addr);
	cnt1.imm(imm);
	cnt1.prog_addr(prog_addr);
	cnt1.rf_wr_addr(rf_wr_addr);
	cnt1.rdest_out(alu_rdest);
	cnt1.rsrc_out(alu_rsrc);
	cnt1.result(alu_out);
	cnt1.mem_data(mem_out);
	cnt1.result_out(wb_data);
	cnt1.rdest_data(rf_dest_data);
	cnt1.rsrc_data(rf_src_data);
	cnt1.mem_addr(mem_addr);
	cnt1.clk(clock);
	cnt1.mem_wr_data(mem_wr_data);
	
	sc_start(750,SC_NS);
	
	return 0;
}
