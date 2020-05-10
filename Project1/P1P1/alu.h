class alu:public sc_module {
public:
	sc_in<sc_uint<16> > rsrc, rdest, imm;
	sc_in<sc_bv<3> > alu_op;
	sc_in<sc_bv<4> > ctrl;	// [0] = Immediate or Reg
			// [1] = 1's complement?
			// [2] = Carry In
			// [3] = 0's or Rdest?

	sc_out<bool> n, c, z;	// processor status bits
	sc_out<sc_uint<16> > data_out;

	SC_HAS_PROCESS(alu);
	alu(sc_module_name name):
		sc_module(name){
		SC_METHOD(prc_alu);
		sensitive << alu_op << ctrl;
		}
	~alu(){
	}

private:
	void prc_alu(){
/*
		if(ctrl[0]){
			
		}
		else{
			
		}
		case(alu_op):*/
		
	};
};
