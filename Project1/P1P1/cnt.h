class cnt:public sc_module {
public:
	// inputs
	sc_in<sc_uint<16> > instr;
	sc_in<bool> n, z, c;
	// outputs
	sc_out<bool> rf_en, dm_en;
	sc_out<sc_bv<4> > ctrl;
	sc_out<sc_bv<3> > alu_op;
	sc_out<sc_uint<4> > rsrc, rdest;
	sc_out<sc_uint<16> > imm;
	sc_out<sc_bigint<160> > prog_addr;

	SC_HAS_PROCESS(cnt);
	cnt(sc_module_name name):
		sc_module(name){
			SC_METHOD(prc_cnt);
			sensitive << instr;
		}
	~cnt(){
	}
private:
	void prc_cnt(){

	};
};
