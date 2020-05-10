class server: public sc_module{
public:
	sc_in<bool> clock, sig_clock_r1, sig_clock_r2, sig_clock_r3, sig_clock_r4;
	sc_in<int> sig_value_r1, sig_value_r2, sig_value_r3, sig_value_r4;
	sc_out<bool> sig_tr_clock_r1, sig_tr_clock_r2, sig_tr_clock_r3, sig_tr_clock_r4;
	sc_out<int> sig_tr_value_r1, sig_tr_value_r2, sig_tr_value_r3, sig_tr_value_r4;
	sc_out<double> r1_spd, r2_spd, r3_spd, r4_spd;

	SC_HAS_PROCESS(server);
	server(sc_module_name name, int* map, int* rpath):
		sc_module(name), _map(map), _rpath(rpath){
		SC_METHOD(prc_update);
		sensitive << clock.pos();
		SC_METHOD(prc_receive);
		sensitive << sig_clock_r1.pos() << sig_clock_r2.pos();
		SC_METHOD(prc_transmit);
		sensitive << status_flag.pos();

		bound1 = false;
		bound2 = false;
		// intialize _status_table
		for(int i=0;i<3;i++){
			_status_table[i][0] = _rpath[i][0];
			_status_table[i][1] = _rpath[i][1];
		}
		r1_next = 2;
		r2_next = 2;
		r3_next = 2;
		r4_next = 2;
		temp = 100;
		// initialize the node table
		_node_table[0][0] = 1;
		_node_table[1][0] = 10;
		_node_table[2][0] = 13;
		_node_table[3][0] = 18;
		_node_table[4][0] = 22;
		_node_table[5][0] = 26;
		_node_table[6][0] = 31;
		_node_table[7][0] = 32;
		_node_table[8][0] = 35;
		_node_table[9][0] = 39;
		_node_table[10][0] = 45;
		_node_table[11][0] = 48;
		_node_table[12][0] = 51;
		_node_table[13][0] = 60;
		// set up arrival order
		for(int i=0; i<14; i++){
			t_index = 1;
			// for each path, check if the path contains the node
			// and how far in the path. If distance is less, higher on priority list
			for(int j = 0; j <3; j++){
				for(int k = 0; k < 15; k++){
					if(_rpath[j][k] == _node_table[i][0]){
						if(k < temp){
							// if the node is closer than prev robot
							// place higher in priority
							temp2 = _node_table[t_index];
							_node_table[t_index] = j;
							_node_table[t_index++] = temp2;
						}
						else{
							// further in path, place lower in priority
							_node_table[t_index++] = j;
							temp = k;
						}
					}
					

				}
			}
		}
	}

private:
	int* _map[][];
	int* _rpath[][];
	double time, travel_time;
	int grid_x1, grid_y1, grid_x2, grid_y2, r1_next, r2_next, r3_next, r4_next, temp, temp2, t_index;
	bool bound1, bound2;
	// Current grid, next grid, mov, speed
	int _status_table[4][4];
	// node table: index, arrival order
	int _node_table[14][5]
	sc_signal<bool> status_flag;

	void prc_update(){
		sig_tr_clock_r1.write(false);
		sig_tr_clock_r2.write(false);
		sig_tr_clock_r3.write(false);
		sig_tr_clock_r4.write(false);
		// update grid values 
		for(int i = 0; i<4; i++){
			for(int j=0;j<4;j++){
				// if next grid is same as current grid of another robot
				// and close to boundary, stop
				// else keep moving
				if(i != j){
					if(_status_table[i][1] == _status_table[j][0] && ((i == 0 && bound1) || (i==1 && bound2)) ){
						_status_table[i][2] = 0;
						status_flag.write(true);
					}
				}
			}
		}
		// find x and y values for grids
		for(int i = 0; i<4; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 10; k++){
					if(_map[j][k] == _status_table[i][0]){
						grid_y1 = j;
						grid_x1 = k;
						break;
					}
					if(_map[j][k] == _status_table[i][1]){
						grid_y2 = j;
						grid_x2 = k;
						break;
					}
				}
			}
			// compare current node and next node to get direction
			if(grid_y2 > grid_y1){	// if y2 is greater than y1, move down
				_status_table[i][2] = 2;
			}
			else if(grid_y2 < grid_y1){	// if y2 is less than y1, move up
				_status_table[i][2] = 1;
			}
			else if(grid_x2 > grid_x1){	// if x2 is greater than x1, move right
				_status_table[i][2] = 4;
			}
			else if(grid_x2 < grid_x1){	// if x2 is less than x1, move left
				_status_table[i][2] = 3;
			}
			status_flag.write(true);
		}
		// calculate speed for each robot
		// make robot w/ higher priority faster
		for(int i = 0; i < 14; i++){
			// find nodes with more than 1 robot in the list
			// find distance from current grid to next node
			for(int j = 1; j < 5; j++){
				if (_node_table[i][j] == 0){
					break;
				}
				temp = j;
			}
			for(int k = 1; k <= temp; j++){
				// find index of the node in the path
				for(int l = 0; l = 15; i++){
					if(_rpath[k][l] == _node_table[i][0]){
						temp2 = l;
						break;
					}
				}
				switch(_node_table[i][k]){
				// compare to current grid and generate speed
					case 0:
						if(k == 1){
							// if highest priority, set speed to 2 m/s
							travel_time = temp2 - (r1_next - 1);
							_status_table[0][3] = 0.002;
						}
						else{
							// else calc speed based on travel time of highest priority
							_status_table[0][3] =  (2 *temp2 - (r1_next - 1)) / travel_time / 1000;
						}
						break;
					case 1:
						if(k == 1){
							// if highest priority, set speed to 2 m/s
							travel_time = temp2 - (r2_next - 1);
							_status_table[1][3] = 0.002;
						}
						else{
							// else calc speed based on travel time of highest priority
							_status_table[1][3] =  (2 *temp2 - (r2_next - 1)) / travel_time / 1000;
						}
						break;						
					case 2:
						if(k == 1){
							// if highest priority, set speed to 2 m/s
							travel_time = temp2 - (r3_next - 1);
							_status_table[2][3] = 0.002;
						}
						else{
							// else calc speed based on travel time of highest priority
							_status_table[2][3] =  (2 *temp2 - (r3_next - 1)) / travel_time / 1000;
						}
						break;						
					case 3:
						if(k == 1){
							// if highest priority, set speed to 2 m/s
							travel_time = temp2 - (r4_next - 1);
							_status_table[3][3] = 0.002;
						}
						else{
							// else calc speed based on travel time of highest priority
							_status_table[3][3] =  (2 *temp2 - (r4_next - 1)) / travel_time / 1000;
						}
						break;
					default:
				}
				status_flag.write(true);	
			}
		}
	}
	
	void prc_receive(){
		// if received data from r1, update
		if(sig_clock_r1.read()){
			switch(sig_value_r1.read()){
				case 5: // bound
					bound1 = true;
					break;
				case 6: // obstacle
					_status_table[0][2] = 0;
					status_flag.write(true);
					break;
				case 7: // passed boundary
				// update current node and next node
					_status_table[0][0] = _status_table[0][1];
					_status_table[0][1] = _rpath[r1_next++];
					break;
				default:
			}
		}
		// if received data from r2, update
		if(sig_clock_r2.read()){
			switch(sig_value_r2.read()){
				case 5: // bound
					bound2 = true;
					break;
				case 6: // obstacle
					_status_table[1][2] = 0;
					status_flag.write(true);
					break;
				case 7: // passed boundary
				// update current node and next node
					_status_table[1][0] = _status_table[1][1];
					_status_table[1][1] = _rpath[r2_next++];
					break;
				default:				
			}
		}
		if(sig_clock_r3.read()){
			switch(sig_value_r3.read()){
				case 5: // bound
					bound1 = true;
					break;
				case 6: // obstacle
					_status_table[2][2] = 0;
					status_flag.write(true);
					break;
				case 7: // passed boundary
				// update current node and next node
					_status_table[2][0] = _status_table[2][1];
					_status_table[2][1] = _rpath[r3_next++];
					break;
				default:
			}
		}
		if(sig_clock_r4.read()){
			switch(sig_value_r4.read()){
				case 5: // bound
					bound1 = true;
					break;
				case 6: // obstacle
					_status_table[3][2] = 0;
					status_flag.write(true);
					break;
				case 7: // passed boundary
				// update current node and next node
					_status_table[3][0] = _status_table[3][1];
					_status_table[3][1] = _rpath[r4_next++];
					break;
				default:
			}
		}
	}

	void prc_transmit(){
		for(int i=0; i<4; i++){
			// write out move value and speed
			if(i == 0){
				sig_tr_value_r1.write(_status_table[0][2]);
				sig_tr_clock_r1.write(true);
				r1_spd.write(_status_table[0][3]);
			}
			if(i == 1){
				sig_tr_value_r2.write(_status_table[1][2]);
				sig_tr_clock_r2.write(true);
				r2_spd.write(_status_table[1][3]);
			}
			if(i == 2){
				sig_tr_value_r3.write(_status_table[2][2]);
				sig_tr_clock_r3.write(true);
				r3_spd.write(_status_table[2][3]);
			}
			if(i == 3){
				sig_tr_value_r4.write(_status_table[3][2]);
				sig_tr_clock_r4.write(true);
				r4_spd.write(_status_table[3][3]);
			}
			status_flag.write(false);
			temp = 0;
		}		

	}
};
