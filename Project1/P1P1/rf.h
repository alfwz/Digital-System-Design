class rf:public sc_module{
public:
	sc_in<bool> wr_en;
	sc_in<bool> clock;
	sc_in<sc_uint<16> > wr_data;
	sc_in<sc_uint<4> > rd_addr1, rd_addr2, wr_addr;
	sc_out<sc_uint<16> > rd_data1, rd_data2;

	SC_HAS_PROCESS(rf);
	rf(sc_module_name name, int* regfile):
		sc_module(name), _reg_file(regfile){
		SC_METHOD(prc_rf);
		sensitive << clock.pos();	 
	}
	~rf(){
	}
private:
	int* _reg_file;

	void prc_rf(){
		rd_data1.write(_reg_file[rd_addr1.read()]);
		rd_data2.write(_reg_file[rd_addr2.read()]);
	}
};
