#include <systemc.h>
struct tuple {
	int ROI_index;
	double start_time, end_time;

	bool operator== (const tuple& other){
		return other.ROI_index == ROI_index && other.start_time == start_time && other.end_time == end_time;
	}
};

ostream& operator<< (ostream& o, const tuple& t){
	return o << t.ROI_index << '/' << t.start_time << '/' << t.end_time;
}

void sc_trace (sc_trace_file *tf, const tuple& t, const sc_string& name){
	sc_trace(tf, t.ROI_index, name+".ROI_index");
	sc_trace(tf, t.start_time, name+".start_time");
	sc_trace(tf, t.end_time, name+".end_time");
}
