#include "systemc.h"

SC_MODULE(packed){
	sc_in < bool > Reset, Clear, clock;
	sc_in < sc_uint<12> > Indata;
	sc_out < sc_uint<4> > Payload;
	sc_out < sc_uint<8> > Count;
	sc_out < sc_uint<8> > Error;

	void prc_packed();
	sc_uint<4> type, data, parity;
	sc_uint<12> temp_data;
	sc_uint<8> count, error;
	
	SC_CTOR(packed){
	SC_METHOD(prc_packed);
	sensitive<<clock.pos();
	}
};

void packed :: prc_packed(){
if(!Reset||Clear)
	{
	count=0;
	error=0;
	data=0;
}
else{
	temp_data=Indata;

	type=temp_data.range(11,8);

	if(type==1){

		data=temp_data.range(7,4);
		count=count+1;
		parity=temp_data.range(3,0);
		if(parity==0){

			if(data%2!=1){

				error=error+1;
				}
		}		
		if(parity==1){
			
			if(data%2==1){
				
				error=error+1;
				}
			}
		}
	}
	Error=error;
	Count=count;
	Payload=data;
}
