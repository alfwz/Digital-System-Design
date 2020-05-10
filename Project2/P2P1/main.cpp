#include <systemc.h>
#include <server.h>
#include <processing.h>
#include <robot.h>
#include <sys_mon.h>

int sc_main(int argc, char* argv[]){
	int map[9][10] = { {1, 2, 3, 4, 5 ,6, 7, 8, 9, 10}, 
			{11, 0, 0, 0, 0, 0, 0, 0, 0, 12},
			{13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
			{23, 0, 0, 0, 0, 24, 0, 0, 0, 25},
			{26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
			{36, 0, 0, 0, 0, 0, 37, 0, 0, 38},
			{39, 40, 41, 42, 43, 44, 45, 46, 47, 48},
			{49, 0, 0, 0, 0, 0, 0, 0, 0, 50},
			{51, 52, 53, 54, 55, 56, 57, 58, 59, 60}};
	int rpath[2][9] = { {1, 11, 13, 14, 15, 16, 17, 18, 24},
			{60, 50, 48, 47, 46, 45, 37, 32, 31}};

	sc_clock clock("clock", 1, NC_MS);

	sc_signal<bool> server_to_r1_clk, server_to_r2_clk, r1_to_server_clk, r2_to_server_clk, pro_to_r1_clk, pro_to_r2_clk, r1_to_pro_clk, r2_to_pro_clk;
	sc_signal<int> server_to_r1_val, server_to_r2_val, r1_to_server_val, r2_to_server_val, pro_to_r1_val, pro_to_r2_val, r1_to_pro_val, r2_to_pro_val;
	server server1("server1", &map[0], &rpath[0]);
	server1.sig_clock_r1(r1_to_server_clk);
	server1.sig_clock_r2(r2_to_server_clk);
	server1.sig_value_r1(r1_to_server_val);
	server1.sig_value_r2(r2_to_server_val);
	server1.sig_tr_clock_r1(server_to_r1_clk);
	server1.sig_tr_clock_r2(server_to_r2_clk);
	server1.sig_tr_value_r1(server_to_r1_val);
	server1.sig_tr_value_r2(server_to_r2_val);
	server1.clock(clock);



	processing pro1("pro1", &map[0], rpath[0][0], rpath[1][0]);
	pro1.sig_clock_r1(r1_to_pro_clk);
	pro1.sig_clock_r2(r2_to_pro_clk);
	pro1.sig_value_r1(r1_to_pro_val);
	pro1.sig_value_r2(r2_to_pro_val);
	pro1.sig_tr_clock_r1(pro_to_r1_clk);
	pro1.sig_tr_clock_r2(pro_to_r2_clk);
	pro1.sig_tr_value_r1(pro_to_r1_val);
	pro1.sig_tr_value_r2(pro_to_r2_val);
	pro1.clock(clock);



	robot r1("r1");
	r1.sig_rcfrom_server(server_to_r1_clk);
	r1.sig_rcfrom_processing(pro_to_r1_clk);
	r1.sig_valuefromserver(server_to_r1_val);
	r1.sig_valuefromprocessing(pro_to_r1_val);
	r1.sig_trto_server(r1_to_server_clk);
	r1.sig_trto_processing(r1_to_pro_clk);
	r1.sig_valuetoserver(r1_to_server_val);
	r1.sig_valuetoprocessing(r1_to_pro_val);



	robot r2("r2");
	r2.sig_rcfrom_server(server_to_r2_clk);
	r2.sig_rcfrom_processing(pro_to_r2_clk);
	r2.sig_valuefromserver(server_to_r2_val);
	r2.sig_valuefromprocessing(pro_to_r2_val);
	r2.sig_trto_server(r2_to_server_clk);
	r2.sig_trto_processing(r2_to_pro_clk);
	r2.sig_valuetoserver(r2_to_server_val);
	r2.sig_valuetoprocessing(r2_to_pro_val);


	sys_mon mon1("mon1");
	mon1.r1_to_server_clk(r1_to_server_clk);
	mon1.r2_to_server_clk(r2_to_server_clk);	
	mon1.r1_to_pro_clk(r1_to_pro_clk);
	mon1.r2_to_pro_clk(r2_to_pro_clk);
	mon1.server_to_r1_clk(server_to_r1_clk);
	mon1.server_to_r2_clk(server_to_r2_clk);
	mon1.pro_to_r1_clk(pro_to_r1_clk);
	mon1.pro_to_r2_clk(pro_to_r2_clk);
	mon1.r1_to_server_val(r1_to_server_val);
	mon1.r2_to_server_val(r2_to_server_val);	
	mon1.r1_to_pro_val(r1_to_pro_val);
	mon1.r2_to_pro_val(r2_to_pro_val);
	mon1.server_to_r1_val(server_to_r1_val);
	mon1.server_to_r2_val(server_to_r2_val);
	mon1.pro_to_r1_val(pro_to_r1_val);
	mon1.pro_to_r2_val(pro_to_r2_val);


	sc_start(15, SC_SEC);



	return 0;

}
