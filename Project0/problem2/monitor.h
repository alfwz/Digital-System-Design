#include "systemc.h"

SC_MODLE(monitor){
	sc_in<bool> m_load1, m_load2, m_dec1, m_dec2, m_overflow1,m_overflow2, m_ended, m_clock;
	sc_in<sc_uint<8>> m_count1, m_count2, m_in1, m_in2, m_in3;
	
	void prc(monitor);
	
	SC_CTOR(monitor){
		SC_METHOD(prc_monitor){
			sensitive<<m_load1<<m_load2<<m_dec2<<m_overflow1<<m_overflow2<<m_ended<<m_clock<<m_count1<<m_count2;
		}
	};
	
	
void monitor::prc_monitor(){
		cout<<"At time" <<sc_time_stamp()<< "::";
		cout<<"(load1,in1,dec1,overflow1,count1):"<<m_load1<<","<<m_in1<<","<<m_dec1<<","<<m_overflow1<<","<<m_count1<<","<<end1;
		cout<<"(load2,in2,in3,dec2,overflow2,count2):"<<m_load2<<","<<m_in2<<","<<m_in3<<","<<m_dec2<<","<<m_overflow2<<","<<m_count2<<","<<m_endl;
		cout<<"(ended):"<<m_ended<<endl;
		cout<<endl;
}