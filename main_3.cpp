#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#define BOARD_SIZE 4 //>= 4 only
#define MAXIMUM_DEPTH 5

struct board{
	char data[BOARD_SIZE][BOARD_SIZE];
	int agent[2];
	int depth;
};

//Methods
void print_board(board b);
bool move_agent(board *b, int direction);
void init_board(board *b);
void init_almost_complete_board(board *b);
bool is_complete(board b);
bool run_depth_first_simulation(board b, int depth);
int run_random_move_simulation(board b);
bool run_breadth_first_simulation(int depth);
std::vector<int> shuffled_direction_vect();

//Global variables
int nodes_evaluated = 0;
std::vector<board> nodes_found;

//Starting point
int main(int argc, char** argv){
    board b;
	nodes_found.push_back(b);
	init_almost_complete_board(&b);
	print_board(b);

	//if (run_depth_first_simulation(b,0)) std::cout << "FINISHED" << '\n';
	//return 0;

	while (nodes_evaluated < 10){
		if (run_breadth_first_simulation(0)){
			std::cout << "result of sim = true" << '\n';
			break;
		}else{
			std::cout << "nodes expanded = " << nodes_evaluated << '\n';
			std::cout << "result of sim = false" << '\n';
		}
	}
    print_board(b);
}

//Random, single path node expansion
int run_random_move_simulation(board b){
	int n = 0;
	while (!is_complete(b)){
		move_agent(&b,(rand()%4)+1);
		n++;
	}
	return n;
}

//Breadth first search
bool run_breadth_first_simulation(int depth){
	
	std::cout << "starting func" << '\n';
	
	nodes_evaluated++;
	
	bool result = is_complete(nodes_found.back());
	if (result) return true;
	
	std::cout << "mid func 1" << '\n';
	
	board b_up = nodes_found.back();
	std::cout << "made up board via copy" << '\n';
	move_agent(&b_up,1);
	std::cout << "moved up board agent" << '\n';
	board b_down = nodes_found.back();
	move_agent(&b_down,3);
	board b_left = nodes_found.back();
	move_agent(&b_left,4);
	board b_right = nodes_found.back();
	move_agent(&b_right,2);
	
	std::cout << "mid func 2" << '\n';
	
	std::vector<int> direction_order_array = shuffled_direction_vect();
	
	for (int i = 0; i < 4; i++){
		if (direction_order_array[i]==1) nodes_found.insert(begin(nodes_found),b_up);
		if (direction_order_array[i]==2) nodes_found.insert(begin(nodes_found),b_right);
		if (direction_order_array[i]==3) nodes_found.insert(begin(nodes_found),b_down);
		if (direction_order_array[i]==4) nodes_found.insert(begin(nodes_found),b_left);
	}
	
	print_board(nodes_found.back());
	nodes_found.pop_back();
	
	std::cout << "finished func" << '\n';
	
	return false;
}

//Depth first search
bool run_depth_first_simulation(board b, int depth){
	
	nodes_evaluated++;
	
	std::cout << "Board at depth " << depth << "   Nodes evaluated = " << nodes_evaluated << '\n';
	if (depth > MAXIMUM_DEPTH) return false;

    board b_left = b;
    board b_right = b;
    board b_up = b;
    board b_down = b;

	std::vector<int> direction_order_array = shuffled_direction_vect();
	
	for (int i=0; i<4; i++){
		if (direction_order_array[i]==4){
			if (move_agent(&b_left,4)){
				std::cout << "moved left" << '\n';
				print_board(b_left);
				if (is_complete(b_left)){
					return true;
				}else{
					if (run_depth_first_simulation(b_left,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==1){
			if (move_agent(&b_up,1)){
				std::cout << "moved up" << '\n';
				print_board(b_up);
				if (is_complete(b_up)){
					return true;
				}else{
					if (run_depth_first_simulation(b_up,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==2){
			if (move_agent(&b_right,2)){
				std::cout << "moved right" << '\n';
				print_board(b_right);
				if (is_complete(b_right)){
					return true;
				}else{
					if (run_depth_first_simulation(b_right,depth+1)){
						return true;
					}
				}
			}
		}
		if (direction_order_array[i]==3){
			if (move_agent(&b_down,3)){
				std::cout << "moved down" << '\n';
				print_board(b_down);
				if (is_complete(b_down)){
					return true;
				}else{
					if (run_depth_first_simulation(b_down,depth+1)){
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool move_agent(board *b, int direction){
	std::cout << "got here 0" << '\n';
    char temp;
    if (direction == 1 && b->agent[1] > 0){
		std::cout << "got here 1" << '\n';
        temp = b->data[b->agent[0]][b->agent[1]-1];
        b->data[b->agent[0]][b->agent[1]-1] = b->data[b->agent[0]][b->agent[1]];
		std::cout << "got here 2" << '\n';
        b->data[b->agent[0]][b->agent[1]] = temp;
        b->agent[1]--;
        return true;
    }else if(direction == 2 && b->agent[0] < BOARD_SIZE-1){
		std::cout << "got here 1" << '\n';
        temp = b->data[b->agent[0]+1][b->agent[1]];
        b->data[b->agent[0]+1][b->agent[1]] = b->data[b->agent[0]][b->agent[1]];
		std::cout << "got here 2" << '\n';
        b->data[b->agent[0]][b->agent[1]] = temp;
        b->agent[0]++;
        return true;
    }else if(direction == 3 && b->agent[1] < BOARD_SIZE-1){
		std::cout << "got here 1" << '\n';
        temp = b->data[b->agent[0]][b->agent[1]+1];
        b->data[b->agent[0]][b->agent[1]+1] = b->data[b->agent[0]][b->agent[1]];
		std::cout << "got here 2" << '\n';
        b->data[b->agent[0]][b->agent[1]] = temp;
        b->agent[1]++;
        return true;
    }else if(direction == 4 && b->agent[0] > 0){
		std::cout << "got here 1" << '\n';
        temp = b->data[b->agent[0]-1][b->agent[1]];
        b->data[b->agent[0]-1][b->agent[1]] = b->data[b->agent[0]][b->agent[1]];
		std::cout << "got here 2" << '\n';
        b->data[b->agent[0]][b->agent[1]] = temp;
        b->agent[0]--;
        return true;
    }
    //std::cout << "CANNOT MOVE THAT WAY!" << '\n';
    return false;
}
void print_board(board b){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            std::cout << b.data[j][i] << " ";
        }
        std::cout << '\n';
    }
}
void init_board(board *b){
    for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
    }
    b->data[0][3] = 'A';
    b->data[1][3] = 'B';
    b->data[2][3] = 'C';
    b->data[3][3] = '#';
	b->agent[0] = 3;
	b->agent[1] = 3;
}
void init_almost_complete_board(board *b){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
	}
	b->data[1][0] = 'A';
	b->data[1][2] = 'B';
	b->data[1][3] = 'C';
	b->data[1][1] = '#';
	b->agent[0]=1;
	b->agent[1]=2;
}
bool is_complete(board b){
    if (b.data[(BOARD_SIZE/2)-1][BOARD_SIZE-1] == 'C' && b.data[(BOARD_SIZE/2)-1][BOARD_SIZE-2] == 'B' && b.data[(BOARD_SIZE/2)-1][BOARD_SIZE-3] == 'A'){ // && b.data[BOARD_SIZE-1][BOARD_SIZE-1] == '#'){
		std::cout << "IS COMPLETE == TRUE!" << '\n';
        return true;
    }else{
        return false;
    }
}
std::vector<int> shuffled_direction_vect(){
	std::vector<int> direction_order_array;
	direction_order_array.push_back(1);
	direction_order_array.push_back(2);
	direction_order_array.push_back(3);
	direction_order_array.push_back(4);
	
	std::random_shuffle(direction_order_array.begin(),direction_order_array.end());
	return direction_order_array;
}