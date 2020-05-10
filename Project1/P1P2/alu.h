class alu:public sc_module {
public:
	sc_in<sc_int<16> > rsrc, rdest, imm;
	sc_in<sc_bv<3> > alu_op;
	sc_in<sc_bv<4> > ctrl;	// [0] = Immediate or Rsrc
			// [1] = 1's complement?
			// [2] = Carry In
			// [3] = 0's or Rdest?

	sc_out<sc_bit> n, c, z;	// processor status bits
	sc_out<sc_int<16> > data_out;

	SC_HAS_PROCESS(alu);
	alu(sc_module_name name):
		sc_module(name){
		SC_METHOD(prc_alu);
		sensitive << alu_op << ctrl;
		}
	~alu(){
	}

private:
	int t_operand1, t_operand2, result, c_in;
	void prc_alu(){
		if(ctrl[2]){
			c_in = 1;
		}
		else{
			c_in = 0;
		}


		if(ctrl[0]){
			t_operand2 = imm.read();
		}
		else{
			t_operand2 = rsrc.read();
		}


		if(ctrl[1]){
			t_operand2 = ~(t_operand1);
		} 


		if(ctrl[3]){
			t_operand1 = rdest.read();
		}
		else{
			t_operand1 = 0;
		}



		switch(alu_op.read().to_int()){
			//ADD SUB MOV LOAD STOR
			case 1:
				result = t_operand1 + t_operand2 + c_in;
				if(result == 0) {
					// set zero bit if == 0
					z.write(sc_bit(1));
					c.write(sc_bit(0));
					n.write(sc_bit(0));
				}
				else if (result < 0){
					// set negative bit if < 0
					z.write(sc_bit(0));
					n.write(sc_bit(1));
					if(result < -32768) {
						c.write(sc_bit(0));
					}
					else{
						c.write(sc_bit(0));
					}
				}
				else{
					// a + b + c_in > 0
					z.write(sc_bit(0));
					n.write(sc_bit(0));
					if(result >= 32768){
						c.write(sc_bit(1));
					}
					else{
						c.write(sc_bit(0));
					}
				}

				break;
			//AND 
			case 2:
				result = t_operand1 & t_operand2;
				break;
			//OR
			case 3:
				result = t_operand1 | t_operand2;
				break;
			//XOR
			case 4:
				result = t_operand1 ^ t_operand2;
				break;
			//LSH
			case 5:
				if(t_operand2 > 0) {
					//left shift 
					result = t_operand1 << t_operand2;
				}
				else{
					//right shift 
					result = t_operand1 >> t_operand2;
				}
				break;
			//ASH
			case 6:
				//left shift 
				if(t_operand2 > 0) {
	
					if(t_operand1 < 0){
					result = t_operand1 << t_operand2;
					result = result | 0b1000000000000000;
					}
					
					else{
					result = t_operand1 << t_operand2;
					}
				}	
				//right shift 
				else{		
					
					if(t_operand1 < 0){
						while(t_operand2 > 0 ){
							t_operand1 = t_operand1 >> 1;
							result = t_operand1 | 0b1000000000000000;
							t_operand2--;
						}
					}
					else{
					result = t_operand1 >> t_operand2;
					}
				}
				break;
			//LUI
			case 7:
				result = imm.read() << 8;
				break;
			
			case 0:
			default:
				result = 0;
		}
		data_out.write(result);		
	}
};
