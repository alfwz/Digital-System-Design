#include <systemc.h>

SC_MODULE(sys_mon){
	sc_in<bool> r1_to_server_clk, r1_to_pro_clk, server_to_r1_clk, pro_to_r1_clk, r2_to_server_clk, r2_to_pro_clk, server_to_r2_clk, pro_to_r2_clk, r3_to_server_clk, r3_to_pro_clk, server_to_r3_clk, pro_to_r3_clk, r4_to_server_clk, r4_to_pro_clk, server_to_r4_clk, pro_to_r4_clk;
	sc_in<int> r1_to_server_val, r1_to_pro_val, server_to_r1_val, pro_to_r1_val, r2_to_server_val, r2_to_pro_val, server_to_r2_val, pro_to_r2_val,r3_to_server_val,r4_to_server_val, r3_to_pro_val, r4_to_pro_val, server_to_r3_val, server_to_r4_val, pro_to_r3_val, pro_to_r4_val;
	
	void r1_to_server(){
	// read value r1 sent to server
		cout << "r1 sending " << r1_to_server_val.read() << " to server" << endl;
	}
	
	void r2_to_server(){
	// read value r2 sent to server
		cout << "r2 sending " << r2_to_server_val.read() << " to server" << endl;
	}
	void r3_to_server(){
	// read value r3 sent to server
		cout << "r3 sending " << r3_to_server_val.read() << " to server" << endl;
	}
	
	void r4_to_server(){
	// read value r4 sent to server
		cout << "r4 sending " << r4_to_server_val.read() << " to server" << endl;
	}

	void r1_to_pro(){
	// read value r1 sent to processing
		cout << "r1 sending " << r1_to_pro_val.read() << " to processing" << endl;
	}

	void r2_to_pro(){
	// read value r2 sent to processing
		cout << "r2 sending " << r2_to_pro_val.read() << " to processing" << endl;
	}

	void r3_to_pro(){
	// read value r3 sent to processing
		cout << "r3 sending " << r3_to_pro_val.read() << " to processing" << endl;
	}

	void r4_to_pro(){
	// read value r4 sent to processing
		cout << "r4 sending " << r4_to_pro_val.read() << " to processing" << endl;
	}

	void server_to_r1(){
	// read value server sent to r1
		cout << "server sending " << server_to_r1_val.read() << " to r1" << endl;
	}

	void pro_to_r1(){
	// read value pro sent to r1
		cout << "processing sending " << pro_to_r1_val.read() << " to r1" << endl;
	}

	void server_to_r2(){
	// read value server sent to r2
		cout << "server sending " << server_to_r2_val.read() << " to r2" << endl;
	}

	void pro_to_r2(){
	// read value pro sent to r2
		cout << "processing sending " << pro_to_r2_val.read() << " to r2" << endl;
	}
	void server_to_r3(){
	// read value server sent to r3
		cout << "server sending " << server_to_r3_val.read() << " to r3" << endl;
	}

	void pro_to_r3(){
	// read value pro sent to r3
		cout << "processing sending " << pro_to_r3_val.read() << " to r3" << endl;
	}

	void server_to_r4(){
	// read value server sent to r4
		cout << "server sending " << server_to_r4_val.read() << " to r4" << endl;
	}

	void pro_to_r4(){
	// read value pro sent to r4
		cout << "processing sending " << pro_to_r4_val.read() << " to r4" << endl;
	}

	SC_CTOR(sys_mon){
		SC_METHOD(r1_to_server);
		sensitive << r1_to_server_clk.pos();
		SC_METHOD(r2_to_server);
		sensitive << r2_to_server_clk.pos();
		SC_METHOD(r1_to_pro);
		sensitive << r1_to_pro_clk.pos();
		SC_METHOD(r2_to_pro);
		sensitive << r2_to_pro_clk.pos();
		SC_METHOD(server_to_r1);
		sensitive << server_to_r1_clk.pos();
		SC_METHOD(server_to_r2);
		sensitive << server_to_r2_clk.pos();
		SC_METHOD(pro_to_r1);
		sensitive << pro_to_r1_clk.pos();
		SC_METHOD(server_to_r2);
		sensitive << pro_to_r2_clk.pos();
		SC_METHOD(r3_to_server);
		sensitive << r3_to_server_clk.pos();
		SC_METHOD(r4_to_server);
		sensitive << r4_to_server_clk.pos();
		SC_METHOD(r3_to_pro);
		sensitive << r3_to_pro_clk.pos();
		SC_METHOD(r4_to_pro);
		sensitive << r4_to_pro_clk.pos();
		SC_METHOD(server_to_r3);
		sensitive << server_to_r3_clk.pos();
		SC_METHOD(server_to_r4);
		sensitive << server_to_r4_clk.pos();
		SC_METHOD(pro_to_r3);
		sensitive << pro_to_r3_clk.pos();
		SC_METHOD(server_to_r4);
		sensitive << pro_to_r4_clk.pos();
		
	}
};
