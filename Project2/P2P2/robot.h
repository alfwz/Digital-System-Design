#include <systemc.h>

SC_MODULE(robot){
	sc_in<bool> sig_rcfrom_server, sig_rcfrom_processing, start;
	sc_in<int> sig_valuefromserver, sig_valuefromprocessing;
	sc_in<double> spd_in;
	sc_out<bool> sig_trto_server, sig_trto_processing;
	sc_out<int> sig_valuetoserver, sig_valuetoprocessing
	sc_out<double> spd_out;
	
	sc_signal<int> rc_server_tr_processing, rc_processing_tr_server
	sc_signal<double> spd;
	
	
	SC_HAS_PROCESS(robot);
	robot(sc_module_name robot):
		sc_module(robot){
			SC_METHOD(robot_trto_server);
				sensitive<< rc_processing_tr_server;
			SC_METHOD(robot_trto_processing);
				sensitive<< rc_server_tr_processing << spd;
			SC_METHOD(robot_rcfrom_server);
				sensitive<<sig_rcfrom_server.pos();
			SC_METHOD(robot_rcfrom_processing);
				sensitive<<sig_rcfrom_processing.pos();
		}
};


void robot::robot_trto_server(){
	// transit temp value to server
	sig_trto_server.write(true);
	sig_valuetoserver.write(rc_processing_tr_server);
}

void robot::robot_trto_processing(){
	//transit temp value to processing 
	sig_valuetoprocessing.write(rc_server_tr_processing);
	spd_out.write(spd);
	sig_trto_processing.write(true);
}

void robot::robot_rcfrom_server(){
	//receive value from server then store in temp
	sig_trto_server.write(false);
	rc_server_tr_processing = sig_valuefromserver.read();
	spd = spd_in.read();
}

void robot::robot_rcfrom_processing(){
	//receive value from processing then store in temp
	sig_trto_process.write(false);
	rc_processing_tr_server = sig_valuefromprocessing.read();
}
			
		
