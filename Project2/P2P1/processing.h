#include <cmath>

class processing: public sc_module{
public:
	sc_in<bool> clock, sig_clock_r1, sig_clock_r2;
	sc_in<int > sig_value_r1, sig_value_r2;
	sc_out<bool> sig_tr_clock_r1, sig_tr_clock_r2;
	sc_out<int > sig_tr_r1, sig_tr_r2;

	SC_HAS_PROCESS(processing);
	processing(sc_module_name name, int* map, double r1_grid, double r2_grid):
		sc_module(name), _map(map), grid_temp1(r1_grid), grid_temp2(r2_grid)
		{
		SC_METHOD(prc_update);
		sensitive <<  clock.pos();
		SC_METHOD(prc_receive);
		sensitive << sig_clock_r1.pos() << sig_clock_r2.pos();
		SC_METHOD(prc_transmit);
		sensitive << status_flag.pos();

		// loop to center the robots in the grids
		/*for(int i=0; i<2; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 10; k++){
					if(_map[j][k] == int(_pro_table[i][1]){
						_pro_table[i][3] = 2*k+1;
						_pro_table[i][4] = 2*j+1;
						break;
					}
				}
			}
		}*/
		_pro_table[0][2] = grid_temp1;
		_pro_table[1][2] = grid_temp2;
		// initialize obstacle x and y
		_obs_table[0][1] = 1;
		_obs_table[0][2] = 7;
		_obs_table[1][1] = 13;
		_obs_table[1][2] = 19;
		obs_spd = 0.004;		// obstacle speed = 4m/s, assume clk cycle is 10 ms, 0.004m / 1 ms
		r_spd = 0.002;
		status_flag.write(false);
		temp = 0;
	}

private:
	int* _map;	// array w/ grid numbers. Walls are represented by 0;
	int grid_x, grid_y, temp;
	double obs_spd, r_spd, grid_temp1, grid_temp2;
	// table with moving status of robots
	int _server_table[2];
	// table with bound, obstacle, grid, x_dist, y_dist
	double _pro_table[2][5];
	// table for obstacles: grid, x_dist, y_dist
	double _obs_table[2][3];
	// True when change has occured in the _pro_table
	sc_signal<bool> status_flag;

	// update positions of robots and obstacles
	void prc_update(){
		status_flag.write(false);
		sig_tr_clock_r1.write(false);
		sig_tr_clock_r2.write(false);
		for(int i=0; i<2; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 10; k++){
					if(_map[j][k] == int(_pro_table[i][2]){
						grid_x = 2*k;
						grid_y = 2*j;
						break;
					}
				}
			}
			// if robot status is moving, update position, else do nothing
			// if moved into a new grid, reset boundary and tell robot
			switch(_server_table[i]){
			case 1: // move up
				_pro_table[i][4] = _pro_table[i][4] - r_spd;
				if(_pro_table[i][4] < grid_y - 2){
					_pro_table[i][2] = _map[grid_y/2 - 1][grid_x/2];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 2: // move down
				_pro_table[i][4] = _pro_table[i][4] + r_spd;
				if(_pro_table[i][4] > grid_y + 2){
					_pro_table[i][2] = _map[grid_y/2 + 1][grid_x/2];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 3: // move left
				_pro_table[i][3] = _pro_table[i][3] - r_spd;
				if(_pro_table[i][3] < grid_x - 2){
					_pro_table[i][2] = _map[grid_y/2][grid_x/2 - 1];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 4: // move right
				_pro_table[i][3] = _pro_table[i][3] + r_spd;
				if(_pro_table[i][3] > grid_x + 2){
					_pro_table[i][2] = _map[grid_y/2][grid_x/2 + 1];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			default:
			}
			
		}
		// update obstacle positions
		for(int i=0; i<2; i++){
			if(_obs_table[i][1] < 20){
				_obs_table[i][1] = _obs_table[i][1] + obs_spd;
			}
			else{
				_obs_table[i][1] = _obs_table[i][1] - obs_spd;
			}
		}
		// if robot is close to boundary, send signal to robot
		for(int i=0; i<2; i++){
			switch(_server_table[i]){
			case 1:
				if((grid_y-2 - _pro_table[i][4]) > 0.5){
					_pro_table[i][1] = 1;
					status_flag.write(true);
				}
				break;
			case 2: // move down
				if((grid_y+2 - _pro_table[i][4]) > 0.5){
					_pro_table[i][1] = 1;	
					status_flag.write(true);				
				}
			case 3: // move left
				if((grid_x-2 - _pro_table[i][4]) > 0.5){
					_pro_table[i][1] = 1;
					status_flag.write(true);					
				}
				break;
			case 4: // move right
				if((grid_x+2 - _pro_table[i][4]) > 0.5){
					_pro_table[i][1] = 1;
					status_flag.write(true);					
				}
				break;
			default:
			}
		}
		// if robot is close to obstacle, send signal to robot
		for(int i=0; i<2; i++){
			if(abs(_pro_table[i][3]-_obs_table[0][1]) < 3 ||
			abs(_pro_table[i][4]- _obs_table[0][2]) < 3 ||
			abs(_pro_table[i][3]-_obs_table[1][1]) < 3 ||
			abs(_pro_table[i][4]- _obs_table[1][2]) < 3){

				_pro_table[i][1] = 1;
				status_flag.write(true);
			}
		}
	}
	// when the robots send data, check the values and update server table
	// 0 = stopped by server, 1 = move up, 2 = move down, 3 = move left, 4 = move right, 
	// 5 = boundary, 6 = obstacle, 7 = new grid
	void prc_receive(){
		
		// if robot 1 sent data
		if(sig_clock_r1.read()){
			switch(sig_value_r1.read()){
				case 1:
					_server_table[0] = 1;
					sig_tr_clock_r1.write(true);
					break;
				case 2:
					_server_table[0] = 2;
					sig_tr_clock_r1.write(true);
					break;
				case 3:
					_server_table[0] = 3;
					sig_tr_clock_r1.write(true);
					break;
				case 4:
					_server_table[0] = 4;
					sig_tr_clock_r1.write(true);
					break;
				default:
			}
		}
		// if robot 2 sent data
		if(sig_clock_r2.read()){
			switch(sig_value_r2.read()){
				case 1:
					_server_table[1] = 1;
					sig_tr_clock_r2.write(true);
					break;
				case 2:
					_server_table[1] = 2;
					sig_tr_clock_r2.write(true);
					break;
				case 3:
					_server_table[1] = 3;
					sig_tr_clock_r2.write(true);
					break;
				case 4:
					_server_table[1] = 4;
					sig_tr_clock_r2.write(true);
					break;
				default:
			}
		}
	}
	// send new info to robots based on _pro_table
	void prc_transmit(){
		for(i=0; i<2; i++){
			// if close to boundary, send 5
			if(_pro_table[i][0] == 1){
				temp = 5;
			}
			// if close to obstacle, send 6
			if(_pro_table[i][1] == 1){
				temp = 6;
			}	
			if(i == 0){
				sig_tr_r1.write(temp);
				sig_tr_clock_r1.write(true);
			}
			if(i == 1){
				sig_tr_r2.write(temp);
				sig_tr_clock_r2.write(true);
			}
			status_flag.write(false);
			temp = 0;
		}
	}

};
