#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#define BOARD_SIZE 4 //>= 4 only

struct board{
	char data[BOARD_SIZE][BOARD_SIZE];
};

void print_board(char (*board)[BOARD_SIZE]);
bool move_agent(char (*board)[BOARD_SIZE], int* agent_pos, int direction);
void init_board(char (*board)[BOARD_SIZE]);
void init_almost_complete_board(char (*board)[BOARD_SIZE]);
bool is_complete(char (*board)[BOARD_SIZE]);
bool is_block(char symbol);
bool run_depth_first_simulation(char board[BOARD_SIZE][BOARD_SIZE], int* agent_pos, int depth);
void set_board_as(char (*board_to)[BOARD_SIZE],char board_from[BOARD_SIZE][BOARD_SIZE]);
void set_position_as(int* to, int from[2]);
void rand_by_time();
int run_random_move_simulation(char board[BOARD_SIZE][BOARD_SIZE], int agent_pos[2]);
bool run_breadth_first_simulation(int agent_pos[2], std::vector<board> nodes_found, int depth);

int nodes_evaluated = 0;

int main(int argc, char** argv){
    char board[BOARD_SIZE][BOARD_SIZE];
    int agent_pos[2] = {3,3};
    init_board(board);
	
	//****testing
	//agent_pos[0] = 1;
	//agent_pos[1] = 1;
	//init_almost_complete_board(board);
	//****end of testing
	std::cout << (time(NULL) % 8) << '\n';
	std::cout << "entering sim" << '\n';
	if (run_depth_first_simulation(board,agent_pos,0)){
		std::cout << "result of sim = true" << '\n';
	}else{
		std::cout << "result of sim = false" << '\n';
	}
    print_board(board);
}

int run_random_move_simulation(char board[BOARD_SIZE][BOARD_SIZE], int agent_pos[2]){
	int n = 0;
	while (!is_complete(board)){
		move_agent(board,agent_pos,(rand()%4)+1);
		n++;
	}
	return n;
}

bool run_breadth_first_simulation(int agent_pos[2], board b, std::vector<board> nodes_found, int depth){
	
	return false;
}

bool run_depth_first_simulation(char board[BOARD_SIZE][BOARD_SIZE], int agent_pos[2], int depth){
	
	nodes_evaluated++;
	
	std::cout << "Board at depth " << depth << "   Nodes evaluated = " << nodes_evaluated << '\n';
	if (depth > 5) return false;

    char board_left[BOARD_SIZE][BOARD_SIZE];
    set_board_as(board_left,board);
    int agent_pos_left[2];
    set_position_as(agent_pos_left,agent_pos);
	
    char board_right[BOARD_SIZE][BOARD_SIZE];	
    set_board_as(board_right,board);
    int agent_pos_right[2];
    set_position_as(agent_pos_right,agent_pos);
	
    char board_up[BOARD_SIZE][BOARD_SIZE];
    set_board_as(board_up,board);
    int agent_pos_up[2];
    set_position_as(agent_pos_up,agent_pos);
	
    char board_down[BOARD_SIZE][BOARD_SIZE];
    set_board_as(board_down,board);
    int agent_pos_down[2];
    set_position_as(agent_pos_down,agent_pos);

	std::vector<int> direction_order_array;
	direction_order_array.push_back(1);
	direction_order_array.push_back(2);
	direction_order_array.push_back(3);
	direction_order_array.push_back(4);
	
	srand(2);
	std::random_shuffle(direction_order_array.begin(),direction_order_array.end());
	
	for (int i=0; i<4; i++){
		if (direction_order_array[i]==4){
			if (move_agent(board_left,agent_pos_left,4)){
				std::cout << "moved left" << '\n';
				print_board(board_left);
				if (is_complete(board_left)){
					return true;
				}else{
					if (run_depth_first_simulation(board_left,agent_pos_left,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==1){
			if (move_agent(board_up,agent_pos_up,1)){
				std::cout << "moved up" << '\n';
				print_board(board_up);
				if (is_complete(board_up)){
					return true;
				}else{
					if (run_depth_first_simulation(board_up,agent_pos_up,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==2){
			if (move_agent(board_right,agent_pos_right,2)){
				std::cout << "moved right" << '\n';
				print_board(board_right);
				if (is_complete(board_right)){
					return true;
				}else{
					if (run_depth_first_simulation(board_right,agent_pos_right,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==3){
			if (move_agent(board_down,agent_pos_down,3)){
				std::cout << "moved down" << '\n';
				print_board(board_down);
				if (is_complete(board_down)){
					return true;
				}else{
					if (run_depth_first_simulation(board_down,agent_pos_down,depth+1)){
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool move_agent(char (*board)[BOARD_SIZE], int* agent_pos, int direction){
    char temp;
    if (direction == 1 && agent_pos[1] > 0){
        temp = board[agent_pos[0]][agent_pos[1]-1];
        board[agent_pos[0]][agent_pos[1]-1] = board[agent_pos[0]][agent_pos[1]];
        board[agent_pos[0]][agent_pos[1]] = temp;
        agent_pos[1]--;
        return true;
    }else if(direction == 2 && agent_pos[0] < BOARD_SIZE-1){
        temp = board[agent_pos[0]+1][agent_pos[1]];
        board[agent_pos[0]+1][agent_pos[1]] = board[agent_pos[0]][agent_pos[1]];
        board[agent_pos[0]][agent_pos[1]] = temp;
        agent_pos[0]++;
        return true;
    }else if(direction == 3 && agent_pos[1] < BOARD_SIZE-1){
        temp = board[agent_pos[0]][agent_pos[1]+1];
        board[agent_pos[0]][agent_pos[1]+1] = board[agent_pos[0]][agent_pos[1]];
        board[agent_pos[0]][agent_pos[1]] = temp;
        agent_pos[1]++;
        return true;
    }else if(direction == 4 && agent_pos[0] > 0){
        temp = board[agent_pos[0]-1][agent_pos[1]];
        board[agent_pos[0]-1][agent_pos[1]] = board[agent_pos[0]][agent_pos[1]];
        board[agent_pos[0]][agent_pos[1]] = temp;
        agent_pos[0]--;
        return true;
    }
    //std::cout << "CANNOT MOVE THAT WAY!" << '\n';
    return false;
}
void print_board(char (*board)[BOARD_SIZE]){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            std::cout << board[j][i] << " ";
        }
        std::cout << '\n';
    }
}
void init_board(char (*board)[BOARD_SIZE]){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            board[i][j] = '0';
        }
    }
    board[0][3] = 'A';
    board[1][3] = 'B';
    board[2][3] = 'C';
    board[3][3] = '#';
}
void init_almost_complete_board(char (*board)[BOARD_SIZE]){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            board[i][j] = '0';
        }
    }
    board[1][0] = 'A';
    board[1][2] = 'B';
    board[1][3] = 'C';
    board[1][1] = '#';
}
bool is_complete(char (*board)[BOARD_SIZE]){
    if (board[(BOARD_SIZE/2)-1][BOARD_SIZE-1] == 'C' && board[(BOARD_SIZE/2)-1][BOARD_SIZE-2] == 'B' && board[(BOARD_SIZE/2)-1][BOARD_SIZE-3] == 'A'){
        // && board[BOARD_SIZE-1][BOARD_SIZE-1] == '#'){
		std::cout << "IS COMPLETE == TRUE!" << '\n';
        return true;
    }else{
        return false;
    }
}
void set_board_as(char (*board_to)[BOARD_SIZE],char board_from[BOARD_SIZE][BOARD_SIZE]){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            board_to[i][j] = board_from[i][j];
        }
    }
}
void set_position_as(int* to, int from[2]){
    to[0] = from[0];
    to[1] = from[1];
}
void rand_by_time(){
	srand(time (NULL) % 8);
}