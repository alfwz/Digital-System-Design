#include "driver.h"
#include "monitor.h"
#include "seq_det.h"

int sc_main(int argc, char* argv[])
{
	//ports declaration 
	sc_signal <bool> t_data_in, t_data_out;
	sc_signal <bool> t_clear, t_reset;

	//module instantiation 
	seq_det det1("det1");
	monitor m1("m1");
	driver  d1("d1");
	
	sc_clock clock ("my_clock",2, SC_NS);
	det1.clock (clock);
	
	//to create a signal tracing file 
	//trace file destination 
	sc_trace_file *tfile = sc_create_vcd_trace_file("sequence_detector");
	
	//signal association 
	m1.m_clear(t_clear);
	m1.m_reset(t_reset);
	m1.m_data_in(t_data_in);
	m1.m_clock(clock);
	m1.m_data_out(t_data_out);

	det1.data_in(t_data_in);
	det1.clear(t_clear);
	det1.reset(t_reset);
	det1.data_out(t_data_out);
	
	d1.d_data_in(t_data_in);
	d1.d_clear(t_clear);
	d1.d_reset(t_reset);

	//trace a signal 
	sc_trace(tfile, t_data_in, "data_in");
	sc_trace(tfile, t_clear, "clear");
	sc_trace(tfile, t_reset, "reset");
	sc_trace(tfile, t_data_out, "data_out");
	sc_trace(tfile,clock,"clock");

	sc_start(50, SC_NS);

	//to close and save a file 
	sc_close_vcd_trace_file(tfile);
	return 0;
}
