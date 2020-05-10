class dm:public sc_module{
public:
	sc_in<sc_uint<16> > addr;
	sc_in<bool> wr_en;
	sc_in<sc_uint<16> > data_in;
	sc_out<sc_uint<16> > data_out;

	SC_HAS_PROCESS(dm);
	dm(sc_module_name name, int* init_mem):
		sc_module(name), _mem(init_mem){
		SC_METHOD(prc_dm);
		sensitive << wr_en << data_in << addr;
	}
	~dm(){
	}
private:
	int* _mem;
	void prc_dm(){
		if(wr_en){
			data_out = sc_uint<16> (_mem[addr.read()]);
		}
	}

};
