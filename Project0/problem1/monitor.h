#include "systemc.h"

SC_MODULE(monitor){
	sc_in<bool>m_clear,m_reset,m_data_in, m_clock, m_data_out;
	void prc_monitor();
	
	SC_CTOR(monitor){
		SC_METHOD(prc_monitor);
			sensitive<<m_clear<<m_reset<<m_data_in<<m_clock<<m_data_out;
	}
};

void monitor ::prc_monitor(){
	cout<<"At time"<<sc_time_stamp()<<"::";
	cout<<"(clock,data_in,reset,clear,data_out):"
	<<m_clock<<m_data_in<<m_reset<<m_clear<<m_data_out<<endl;
	}
	
