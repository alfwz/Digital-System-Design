#include <cmath>	// for absolute value

class processing: public sc_module{
public:
	sc_in<bool> clock, sig_clock_r1, sig_clock_r2, sig_clock_r3, sig_clock_r4;
	sc_in<int > sig_value_r1, sig_value_r2, sig_value_r3, sig_value_r4, 
	sc_in<double> r1_spd, r2_spd, r3_spd, r4_spd;
	sc_out<bool> sig_tr_clock_r1, sig_tr_clock_r2, sig_clock_r3, sig_clock_r4;
	sc_out<int > sig_tr_r1, sig_tr_r2, sig_tr_r3, sig_tr_r4;

	SC_HAS_PROCESS(processing);
	processing(sc_module_name name, int* map[][], double r1_grid, double r2_grid, double r3_grid, double r4_grid, int* obs_path[][]):
		sc_module(name), _map(map), grid_temp1(r1_grid), grid_temp2(r2_grid)
		{
		SC_METHOD(prc_update);
		sensitive <<  clock.pos();
		SC_METHOD(prc_receive);
		sensitive << sig_clock_r1.pos() << sig_clock_r2.pos();
		SC_METHOD(prc_transmit);
		sensitive << status_flag.pos();

		time = 0;
		// loop to center the robots in the grids
		for(int i=0; i<4; i++){
			for(int j = 0; j < 9; j++){
				for(int k = 0; k < 10; k++){
					if(_map[j][k] == int(_pro_table[i][1]){
						_pro_table[i][3] = 2*k+1;
						_pro_table[i][4] = 2*j+1;
						break;
					}
				}
			}
		}
		_pro_table[0][2] = grid_temp1;
		_pro_table[1][2] = grid_temp2;
		_pro_table[2][2] = grid_temp3;
		_pro_table[3][2] = grid_temp4;
		
		obs_spd = 0.005;		// obstacle speed = 5m/s, assume clk cycle is 1 ms, 0.005m / 1 ms
		status_flag.write(false);
		temp = 0;
	}

private:
	int* _map[][];	// array w/ grid numbers. Walls are represented by 0;
	int* _obs_path[][]	// path of obstacles
	int grid_x, grid_y, temp;
	double obs_spd, grid_temp1, grid_temp2, grid_temp3, grid_temp4, time;
	// table with moving status of robots and speed
	int _server_table[4][2];
	// table with bound, obstacle, grid, x_dist, y_dist
	double _pro_table[4][5];
	// table for obstacles: grid, x_dist, y_dist
	double _obs_table[4][3];

	int start[4];	// start signals for robots

	// True when change has occured in the _pro_table
	sc_signal<bool> status_flag;

	// update positions of robots and obstacles
	void prc_update(){
		// update timer, assume 1ms clock
		time += 0.001;
		// start robots at certain times
		if(time == 1){
			start[0] = 1;
		}
		if(time == 5){
			start[1] = 1;
		}
		if(time == 7){
			start[2] = 1;
		}
		if(time == 2){
			start[3] = 1;
		}
		status_flag.write(false);
		sig_tr_clock_r1.write(false);
		sig_tr_clock_r2.write(false);
		for(int i=0; i<4; i++){
			// if current robot has not started yet, skip to next robot
			if(start[i] != 1){
				continue;
			}
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
				_pro_table[i][4] = _pro_table[i][4] - _server_table[i][1];
				if(_pro_table[i][4] < grid_y - 2){
					_pro_table[i][2] = _map[grid_y/2 - 1][grid_x/2];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 2: // move down
				_pro_table[i][4] = _pro_table[i][4] + _server_table[i][1];
				if(_pro_table[i][4] > grid_y + 2){
					_pro_table[i][2] = _map[grid_y/2 + 1][grid_x/2];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 3: // move left
				_pro_table[i][3] = _pro_table[i][3] - _server_table[i][1];
				if(_pro_table[i][3] < grid_x - 2){
					_pro_table[i][2] = _map[grid_y/2][grid_x/2 - 1];
					_pro_table[i][0] = 0;
					temp = 7;
					status_flag.write(true);
				}
				break;
			case 4: // move right
				_pro_table[i][3] = _pro_table[i][3] + _server_table[i][1];
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
		for(int i=0; i<4; i++){
			if(_obs_table[i][1] < 20){
				_obs_table[i][1] = _obs_table[i][1] + obs_spd;
			}
			else{
				_obs_table[i][1] = _obs_table[i][1] - obs_spd;
			}
		}
		// if robot is close to boundary, send signal to robot
		for(int i=0; i<4; i++){
			// if current robot has not started yet, skip to next robot
			if(start[i] != 1){
				continue;
			}
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
		for(int i=0; i<4; i++){
			// if current robot has not started yet, skip to next robot
			if(start[i] != 1){
				continue;
			}
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
					_server_table[0][0] = 1;
					sig_tr_clock_r1.write(true);
					break;
				case 2:
					_server_table[0][0] = 2;
					sig_tr_clock_r1.write(true);
					break;
				case 3:
					_server_table[0][0] = 3;
					sig_tr_clock_r1.write(true);
					break;
				case 4:
					_server_table[0][0] = 4;
					sig_tr_clock_r1.write(true);
					break;
				default:
			}
			_server_table[0][1] = r1_spd.read();
		}
		// if robot 2 sent data
		if(sig_clock_r2.read()){
			switch(sig_value_r2.read()){
				case 1:
					_server_table[1][0] = 1;
					sig_tr_clock_r2.write(true);
					break;
				case 2:
					_server_table[1][0] = 2;
					sig_tr_clock_r2.write(true);
					break;
				case 3:
					_server_table[1][0] = 3;
					sig_tr_clock_r2.write(true);
					break;
				case 4:
					_server_table[1][0] = 4;
					sig_tr_clock_r2.write(true);
					break;
				default:
			}
			_server_table[2][1] = r2_spd.read();
		}
		if(sig_clock_r3.read()){
			switch(sig_value_r3.read()){
				case 1:
					_server_table[2][0] = 1;
					sig_tr_clock_r3.write(true);
					break;
				case 2:
					_server_table[2][0] = 2;
					sig_tr_clock_r3.write(true);
					break;
				case 3:
					_server_table[2][0] = 3;
					sig_tr_clock_r3.write(true);
					break;
				case 4:
					_server_table[2][0] = 4;
					sig_tr_clock_r3.write(true);
					break;
				default:
			}
			_server_table[2][1] = r3_spd.read();
		}
		if(sig_clock_r4.read()){
			switch(sig_value_r4.read()){
				case 1:
					_server_table[3][0] = 1;
					sig_tr_clock_r4.write(true);
					break;
				case 2:
					_server_table[3][0] = 2;
					sig_tr_clock_r4.write(true);
					break;
				case 3:
					_server_table[3][0] = 3;
					sig_tr_clock_r4.write(true);
					break;
				case 4:
					_server_table[3][0] = 4;
					sig_tr_clock_r4.write(true);
					break;
				default:
			}
			_server_table[3][1] = r4_spd.read();
		}
	}
	// send new info to robots based on _pro_table
	void prc_transmit(){
		for(i=0; i<4; i++){
			// if current robot has not started yet, skip to next robot
			if(start[i] != 1){
				continue;
			}
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
			if(i == 2){
				sig_tr_r3.write(temp);
				sig_tr_clock_r3.write(true);
			}
			if(i == 3){
				sig_tr_r4.write(temp);
				sig_tr_clock_r4.write(true);
			}
			status_flag.write(false);
			temp = 0;
		}
	}

};
