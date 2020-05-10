class server: public sc_module{
public:
	sc_in<bool> clock, sig_clock_r1, sig_clock_r2;
	sc_in<int> sig_value_r1, sig_value_r2;
	sc_out<bool> sig_tr_clock_r1, sig_tr_clock_r2;
	sc_out<int> sig_tr_value_r1, sig_tr_value_r2;

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
		for(int i=0;i<2;i++){
			_status_table[i][0] = _rpath[i][0];
			_status_table[i][1] = _rpath[i][1];
		}
		r1_next = 2;
		r2_next = 2;
	}

private:
	int* _map;
	int* _rpath;
	int grid_x1, grid_y1, grid_x2, grid_y2, r1_next, r2_next;
	bool bound1, bound2;
	// Current grid, next grid, mov
	int _status_table[2][3];
	sc_signal<bool> status_flag;

	void prc_update(){
		sig_tr_clock_r1.write(false);
		sig_tr_clock_r2.write(false);
		// update grid values 
		for(int i = 0; i<2; i++){
			for(int j=0;j<2;j++){
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
		for(int i = 0; i<2; i++){
			for(int j = 0; j < 10; j++){
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
	}

	void prc_transmit(){
		for(int i=0; i<2; i++){
			// write out move value
			if(i == 0){
				sig_tr_value_r1.write(_status_table[0][2]);
				sig_tr_clock_r1.write(true);
			}
			if(i == 1){
				sig_tr_value_r2.write(_status_table[0][2]);
				sig_tr_clock_r2.write(true);
			}
			status_flag.write(false);
			temp = 0;
		}		

	}
};
