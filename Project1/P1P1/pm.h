class pm: public sc_module{
public:
	sc_in<sc_bigint<160> > prog_addr;
	sc_out<sc_uint<16> > instr;

	SC_HAS_PROCESS(pm);
	pm(sc_module_name name, int* prog_mem):
		sc_module(name), _prog_mem(prog_mem){
		SC_METHOD(prc_pm);
		sensitive << prog_addr;
	}
	~pm(){
	}
private:
	int* _prog_mem;
	sc_bigint<160> temp;
	void prc_pm(){
		instr.write(_prog_mem[prog_addr.read().to_int()]);
	}

};
