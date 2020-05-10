class cnt:public sc_module {
public:
	// inputs
	sc_in<sc_uint<16> > instr;
	sc_in<sc_bit> n, z, c;
	sc_in<bool> clk;
	sc_in<sc_int<16> > rdest_data, rsrc_data, result, mem_data;
	// outputs
	sc_out<sc_bit> rf_en, dm_rd_en, dm_wr_en;
	sc_out<sc_bv<4> > ctrl;
	sc_out<sc_bv<3> > alu_op;
	sc_out<sc_uint<4> > rsrc, rdest, rf_wr_addr;
	sc_out<sc_int<16> > imm, rdest_out, rsrc_out, result_out, mem_wr_data;
	sc_out<sc_uint<16> > mem_addr;
	sc_out<sc_bigint<160> > prog_addr;

	SC_HAS_PROCESS(cnt);
	cnt(sc_module_name name):
		sc_module(name){
			t_pc = 0;
			t_imm = 0;
			t_extended_imm = 0;
			
			SC_METHOD(prc_pipe);
			sensitive << clk.pos();
			SC_METHOD(prc_decode);
			sensitive << ir;
			SC_METHOD(prc_update);
			sensitive << ir << rd_exe_reg << exe_mem_reg << mem_wb_reg << rdest_data;
		}
	~cnt(){
	}
private:

	
	bool branch, jump;
	sc_int<8> t_imm;
	sc_int<16> t_extended_imm;
	sc_uint<16> t_instr;
	sc_bv<4> t_ctrl;
	sc_bv<3> t_alu_op;
	sc_bv<3> rf_dm_en;	// [2] = dm_wr_en, [1] = dm_rd_en, [0] = rf_en
	sc_bigint<160> t_pc;	// temp program counter
	sc_bv<3> temp;
	sc_bv<19> temp2;
	// Pipeline registers
	// 1st level, instr reg
	sc_signal<sc_bv<19> > ir;	// ir[18:16] = ncz, ir[15:0] = instr
	// 2nd level, 
	sc_signal<sc_bv<62> > rd_exe_reg;	// [61:46] = imm, [45:30] =rdest_data, [29:14] = rsrc_data, [13:10] = ctrl, [9:7] = ALU_op 
 	// 3rd level
	sc_signal<sc_bv<55> > exe_mem_reg;	// [54:39] =rdest_data, [38:23] = rsrc_data, [22:7] = result, [6] = dm_wr_en, [5] = dm_rd_en,
	// 4th level, WB
	sc_signal<sc_bv<21> > mem_wb_reg;	// [20:5] = result, [4] = rf_wr_en, [3:0] = dest addr
	// decodes the instruction
	void prc_decode(){
		rsrc.write(ir.read().range(3,0).to_int());
		rdest.write(ir.read().range(11,8).to_int());
		t_imm = instr.read().range(7,0);
		if(t_imm < 0) {
			t_extended_imm = ("11111111", t_imm);
		}
		else{
			t_extended_imm = ("00000000", t_imm);
		}
		switch(ir.read().range(15,12).to_int())
		{
			case 0:
				switch(ir.read().range(7,4).to_int())
				{
					case 5: // add
						t_alu_op = "001";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 9: // sub
						t_alu_op = "001";
						t_ctrl = "1110";
						rf_dm_en = "001";
						break;
					case 11: // cmp
						t_alu_op = "001";
						t_ctrl = "1110";
						rf_dm_en = "000";
						break;
					case 1: // and
						t_alu_op = "010";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 2: // or
						t_alu_op = "011";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 3: // xor
						t_alu_op = "100";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 13: // mov
						t_alu_op = "001";
						t_ctrl = "0000";
						rf_dm_en = "000";
						break;
					default: // nop
						t_alu_op = "011";
						t_ctrl = "0000";
						rf_dm_en = "000";
				}
				// imm not needed, set to 0
				t_extended_imm = 0;
				break;
			case 8:
				switch(ir.read().range(7,4).to_int())
				{
					case 4: // lsh
						t_alu_op = "101";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 6: // ash
						t_alu_op = "110";
						t_ctrl = "1000";
						rf_dm_en = "001";
						break;
					case 0: // lshi, pos imm
						t_extended_imm = ir.read().range(3,0).to_uint();
						t_alu_op = "101";
						t_ctrl = "1001";
						rf_dm_en = "001";
						break;
					case 1: // lshi, neg imm
						t_extended_imm = -(ir.read().range(3,0).to_uint());
						t_alu_op = "101";
						t_ctrl = "1001";
						rf_dm_en = "001";
						break;
					case 2: // ashi, pos imm
						t_extended_imm = ir.read().range(3,0).to_uint();
						t_alu_op = "110";
						t_ctrl = "1001";
						rf_dm_en = "001";
						break;
					case 3: // ashi, neg imm
						t_extended_imm = -(ir.read().range(3,0).to_uint());
						t_alu_op = "110";
						t_ctrl = "1001";
						rf_dm_en = "001";
						break;
					default: // nop
						t_alu_op = "011";
						t_ctrl = "0000";
						rf_dm_en = "000";
				}
				break;
			case 15: // lui
				t_alu_op = "111";
				t_ctrl = "1000";
				rf_dm_en = "001";
				break;
			case 5: // addi
				t_alu_op = "001";
				t_ctrl = "1001";
				rf_dm_en = "001";
				break;
			case 9: // subi
				t_alu_op = "001";
				t_ctrl = "1111";
				rf_dm_en = "001";
				break;
			case 11: // cmpi
				t_alu_op = "001";
				t_ctrl = "1111";
				rf_dm_en = "001";
				break;
			case 1: // andi
				t_alu_op = "010";
				t_ctrl = "1001";
				rf_dm_en = "001";
				break;
			case 2: // ori
				t_alu_op = "011";
				t_ctrl = "1001";
				rf_dm_en = "001";
				break;
			case 3: // xori
				t_alu_op = "100";
				t_ctrl = "1001";
				rf_dm_en = "001";
				break;
			case 13: // movi
				t_alu_op = "001";
				t_ctrl = "0000";
				rf_dm_en = "001";
				break;
			case 12: // branch
				t_alu_op = "000";
				t_ctrl = "0000";
				switch(ir.read().range(11,8).to_int())
				{
					case 0: // equal
						if(ir.read()[16]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 1: // not equal
						if(~ir.read()[16]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 13: // greater than or equal
						if(ir.read()[16]==1 | ir.read()[18]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 2: // carry set
						if(ir.read()[17]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 3: // carry clear
						if(~ir.read()[17] == 1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 6: // greater than
						if(ir.read()[18]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 7: // less than 
						if(~ir.read()[18]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 12: // less than or equal
						if(~ir.read()[18]==1 & ~ir.read()[16]==1){
							branch = true;
						}
						else{
							branch = false;
						}
					case 14: // unconditional
						branch = true;
					default:
					// don't jump, do nothing
					branch = false;
					break;
				}
				break;
			case 4: // jump, load, stor
				switch(ir.read().range(7,4).to_int())
				{
					case 12:
					switch(ir.read().range(11,8).to_int()){
						case 12:
						t_alu_op = "000";
						t_ctrl = "0000";
						rf_dm_en = "001";
						switch(ir.read().range(11,8).to_int()){
						case 0: // equal
							if(ir.read()[16]==1){
								jump = true;
							}
							else{
								jump = false;
							}
							break;
						case 1: // not equal
							if(~ir.read()[16]==1){
								jump = true;
							}
							else{
								jump = false;
							}
							break;
						case 13: // greater than or equal
							if(ir.read()[16]==1 | ir.read()[18]==1){
								jump = true;
							}
							else{
								jump = false;
							}
							break;
						case 2: // carry set
							if(ir.read()[17]==1){
								jump = true;
							}
							else{
								jump = false;
							}
							break;
						case 3: // carry clear
							if(~ir.read()[17]==1){
								jump = true;
							}
							else{
								jump = false;
							}
							break;
						case 6: // greater than
							if(ir.read()[18]==1){
								jump = true;
							}
							else{
								jump = false;
							}			
							break;
						case 7: // less than
							if(~ir.read()[18]==1){
								jump = true;
							}
							else{
								jump = false;
							}		
							break;
						case 12: // less than or equal
							if(~ir.read()[18]==1 & ~ir.read()[16]==1){
								jump = true;
							}
							else{
								jump = false;
							}		
							break;
						case 14:
							jump = true;
							break;
						case 8:
							jump = true;
							t_alu_op = "000";
							t_ctrl = "1000";
							rf_dm_en = "001";
					}
					break;
					case 0:
						t_alu_op = "000";
						t_ctrl = "0000";
						rf_dm_en = "011";
						break;
					case 4:
						t_alu_op = "000";
						t_ctrl = "0000";
						rf_dm_en = "100";
						break;	
				}
				break;			
			default: // nop
				t_alu_op = "011";
				t_ctrl = "0000";
				rf_dm_en = "000";
		}
	}
}
	// moves signals into next pipeline register
	void prc_pipe(){
		temp=(n.read(), c.read(), z.read());
		temp2 =(temp, instr.read());
		ir.write(temp2);
		rd_exe_reg = (t_extended_imm, rdest_data.read(), rsrc_data.read(), t_ctrl, t_alu_op, rf_dm_en, ir.read().range(11,8));
		exe_mem_reg = (rd_exe_reg.read().range(45,14), result.read(), rd_exe_reg.read().range(6,0));
		if(exe_mem_reg.read()[5] ==1& exe_mem_reg.read()[4]==1){
			mem_wb_reg = (mem_data.read(), exe_mem_reg.read().range(4,0));
		}
		else{
			mem_wb_reg = (exe_mem_reg.read().range(22,7), exe_mem_reg.read().range(4,0));
		}
	}
	// update outputs
	void prc_update(){
		if(branch){
			t_pc = t_pc + t_imm;
		}
		else if(jump){
			t_pc = rdest_data.read();
		}
		else{
			t_pc++;
		}
		prog_addr.write(t_pc);
		ctrl.write(rd_exe_reg.read().range(13,10));
		alu_op.write(rd_exe_reg.read().range(9,7));
		dm_wr_en.write(sc_bit(exe_mem_reg.read()[6]));
		dm_rd_en.write(sc_bit(exe_mem_reg.read()[5]));
		rf_en.write(sc_bit(mem_wb_reg.read()[4]));
		prog_addr.write(t_pc);
		imm.write(exe_mem_reg.read().range(61,46).to_int());
		rdest_out.write(exe_mem_reg.read().range(45,30).to_int());
		rsrc_out.write(exe_mem_reg.read().range(29,14).to_int());
		mem_addr.write(exe_mem_reg.read().range(38,23).to_int());
		mem_wr_data.write(exe_mem_reg.read().range(54,39).to_int());
		result_out.write(mem_wb_reg.read().range(20,5).to_int());
		rf_wr_addr.write(mem_wb_reg.read().range(4,0).to_int());
		
	}
};
