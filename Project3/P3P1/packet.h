#include <tuple.h>
#include <systemc.h>

struct packet {
	tuple data[20];
	bool operator== (const packet& other){
		bool temp = true;
		for(int i = 0; i < 20; i++){
			temp == bool && other.data[i] == data[i];
		}
		return bool;
	}
};

ostream& operator<< (ostream& o, const packet& p){
	return o << p.data[0] << p.data[1] << p.data[2] << p.data[3] << p.data[4] << p.data[5] << p.data[6] << p.data[7] << p.data[8] << p.data[9]
		<< p.data[10] << p.data[11] << p.data[12] << p.data[13] << p.data[14] << p.data[15] << p.data[16] << p.data[17] << p.data[18] << p.data[19];

}

void sc_trace (sc_trace_file *tf, const packet& p, const sc_string& name){
	for(int i = 0; i < 20; i++){
		sc_trace(tf, p.data[i], name);
	}
}

