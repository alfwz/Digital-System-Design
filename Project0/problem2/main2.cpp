#include "counter1.h"
#include "counter2.h"
#include "compare.h"
#include "driver.h"
#include "monitor.h"

int sc_main(int argc, char*argv[]){
	//ports declaration
	sc_signal<sc_uint<8>>t_count1,t_count2;
	sc_signal<sc_uint<8>>t_in1,t_in2,t_in3;
	sc_signal<bool> t_overflow1,t_overflow2,t_ended;
	sc_signal<bool>t_load1,t_load2,t_dec1,t_dec2,t_hold;
	sc_clock clock("clock", 1,SC_NS);
	
	//module instantiation
	counter1 c1（"c1");
	counter2 c2("c2");
	compare cp("cm");
	driver d1("d1");
	monitor m1("m1");
	
	//trace file definition
	sc_trace_file *tfile= sc_create_vcd_trace_file("compare");
	
	//signal 
	m1.m_load1(t_load1);
	m1.m_load2(t_load2);
	m1.m_in1(t_in1);
	m1.m_in2(t_in2);
	m1.m_in3(t_in3);
	m1.m_dec1(t_dec1);
	m1.m_dec2(t_dec2);
	m1.m_clock(clock);
	m1.m_overflow1(t_overflow1);
	m1.m_overflow2(t_overflow2);
	m1.m_count1(t_count1);
	m1.m_count2(t_count2);
	m1.m_ended(t_ended);
	
	d1.d_load1(t_load1);
	d1.d_load2(t_load2);
	d1.d_dec1(t_dec1);
	d1.d_dec2(t_dec2);
	d1.d_in1(t_in1);
	d1.d_in2(t_in2);
	d1.d_in3(t_in3);

	c1.clock(clock);
	c1.load1(t_load1);
	c1.dec1(t_dec1);
	c1.in1(t_in1);
	c1.count1(t_count1);
	c1.overflow1(t_overflow1);
	c1.c1_hold(t_hold);

	c2.clock(clock);
	c2.load2(t_load2);
	c2.dec2(t_dec2);
	c2.in2(t_in2);
	c2.in3(t_in3);
	c2.count2(t_count2);
	c2.overflow2(t_overflow2);
	c2.c2_hold(t_hold);

	cp.count1(t_count1);
	cp.count2(t_count2);
	cp.overflow1(t_overflow1);
	cp.overflow2(t_overflow2);
	cp.ended(t_ended);
	cp.c_load1(t_load1);
	cp.c_load2(t_load2);
	cp.c_hold(t_hold);

	sc_trace(tfile, clock, "clock");
	sc_trace(tfile, t_load1, "t_load1");
	sc_trace(tfile, t_dec1, "t_dec1");
	sc_trace(tfile, t_load2, "t_load2");
	sc_trace(tfile, t_dec2, "t_dec2");
	sc_trace(tfile, t_in1, "t_in1");
	sc_trace(tfile, t_in2, "t_in2");
	sc_trace(tfile, t_in3, "t_in3");
	sc_trace(tfile, t_count1, "t_count1");
	sc_trace(tfile, t_count2, "t_count2");
	sc_trace(tfile, t_overflow1, "t_overflow1");
	sc_trace(tfile, t_overflow2, "t_overflow2");
	sc_trace(tfile, t_ended, "t_ended");

	sc_start(50, SC_NS);

	sc_close_vcd_trace_file(tfile);
	return 0;
}
	

	