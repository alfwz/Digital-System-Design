#include "systemc.h"
#include "driver.h"
#include "monitor.h"
#include "Packed.h"

int sc_main(int argc, char*argv[]){
	sc_signal<bool> t_reset, t_clear;
	sc_signal<sc_uint <12> > t_indata;
	sc_signal<sc_uint <8> > t_error, t_count;
	sc_signal<sc_uint <4> > t_payload;

	sc_clock clock ("clock", 1, SC_NS);
	
	monitor m1("m1");
	packed p1("p1");
	driver d1("d1");
	
	m1.m_clear(t_clear);
	m1.m_reset(t_reset);
	m1.m_clock(clock);
	m1.m_indata(t_indata);
	m1.m_payload(t_payload);
	m1.m_count(t_count);
	m1.m_error(t_error);
	
	d1.d_reset(t_reset);
	d1.d_clear(t_clear);
	d1.d_indata(t_indata);
	
	p1.Reset(t_reset);
	p1.Clear(t_clear);
	p1.clock(clock);
	p1.Indata(t_indata);
	p1.Payload(t_payload);
	p1.Count(t_count);
	p1.Error(t_error);

	sc_start(10, SC_NS);

	return 0;
}
