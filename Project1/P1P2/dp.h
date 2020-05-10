#include <rf.h>
#include <pm.h>
#include <dm.h>
#include <alu.h>

class dp:public sc_module {
public:
	// inputs
	sc_in<sc_bv<4> > ctrl;// [0] = Immediate or Reg
				// [1] = 1's complement?
				// [2] = Carry In
				// [3] = 0's or Rdest?
	sc_in<bool> rf_en, dm_rd_en, dm_wr_en;	// [4] = write to RF?
					// [5] = go to DM?
	sc_in<sc_bv<3> > alu_op;
	sc_in<sc_uint<4> > rsrc_addr, rdest_addr, rf_wr_addr;
	sc_in<sc_int<16> > imm, mem_addr, mem_wr_data;
	sc_in<bool> clock;
	sc_in<sc_bigint<160> > prog_addr;
	sc_in<sc_int<16> > rdest_in, rsrc_in, result_in;
	// outputs
	sc_out<sc_uint<16> > instr;
	sc_out<bool> n, z, c;
	sc_out<sc_int<16> > rdest_out, rsrc_out, result_out, mem_out;
	
	SC_HAS_PROCESS(dp);
	dp(sc_module_name name, int* load_pm, int* load_rf, int* load_dm):
		// constructor, instantiates the sub modules
		sc_module(name){
			rf rf1("rf1", load_rf);
			rf1.clock(clock);
			rf1.wr_en(rf_en);
			rf1.wr_data(result_in);
			rf1.rd_addr1(rsrc_addr);
			rf1.rd_addr2(rdest_addr);
			rf1.wr_addr(rf_wr_addr);
			rf1.rd_data1(rsrc_out);
			rf1.rd_data2(rdest_out);

			pm pm1("pm1", load_pm);
			pm1.instr(instr);
			pm1.prog_addr(prog_addr);

			dm dm1("dm1", load_dm);
			dm1.rd_en(dm_rd_en);
			dm1.wr_en(dm_wr_en);
			dm1.addr(mem_addr);
			dm1.data_in(mem_wr_data);
			dm1.data_out(mem_out);

			alu alu1("alu1");
			alu1.rsrc(rsrc_in);
			alu1.rdest(rdest_in);
			alu1.imm(imm);
			alu1.alu_op(alu_op);
			alu1.ctrl(ctrl);
			alu1.data_out(result_out);
			alu1.n(n);
			alu1.z(z);
			alu1.c(c);

			SC_METHOD(src_dp);
			sensitive << clock.pos();
		
		}
	~dp(){
	}
		
private:
	void src_dp(){

	};
};
	
