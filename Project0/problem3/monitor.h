#include "systemc.h"

SC_MODULE (monitor){
	sc_in <bool> m_clear, m_reset, m_clock;
	sc_in <sc_uint<12> > m_indata;
	sc_in <sc_uint<4> > m_payload;
	sc_in <sc_uint<8> > m_count, m_error;

	void prc_monitor();
	
	SC_CTOR(monitor){
		SC_METHOD (prc_monitor);
			sensitive << m_clear<< m_reset << m_clock << m_indata << m_payload << m_count << m_error;
	}
};

void monitor :: prc_monitor(){
	cout << "At time " << sc_time_stamp() << "::" ;
	cout << "(clock, clear, reset, indata): " << m_clock<<", "<<m_clear<<", "<<m_reset<<", "<<m_indata<<endl;
	cout << "(payload, count, error): " << m_payload<<", "<<m_count<<", "<<m_error<<endl;
}
