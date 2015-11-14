#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>
#define BOARD_SIZE 4 //>= 4 only
#define TOWER_HEIGHT 3


struct board{
	char data[BOARD_SIZE][BOARD_SIZE];
	int agent[2];
	int depth;
	bool move_flag;
};

//Methods
void print_board(board b);
board move_agent(board b, int direction);
void init_board(board *b);
void init_almost_complete_board(board *b);
void init_mid_complete_board(board *b);
bool is_complete(board b);
bool run_depth_first_simulation(board b, int depth);
int run_random_move_simulation(board b);
bool run_breadth_first_simulation();
bool run_iterative_deepening_search(board b);
bool can_move_direction(board b, int direction);
std::vector<int> shuffled_direction_vect();
int run_options(board b);
long get_checksum(board b);
bool board_checksum_exists(board b);
bool board_checksum_exists(long board_checksum);
void print_node_checksum_list();

//Global variables
int nodes_evaluated = 0;
int checksum_matches = 0;
int maximum_depth = 8;
std::vector<board> nodes_found;
std::vector<long> node_checksums;

//Starting point
int main(int argc, char** argv){
	
    board b;
	init_board(&b);
	nodes_found.push_back(b);
	board_checksum_exists(get_checksum(b));

	int input = run_options(b);
	while (input != 0){
		if (input == 1){	//Breadth first
			std::cout << "How many nodes to evaluate?" << '\n';
			std::cin >> input;
			while (nodes_evaluated < input){
				print_node_checksum_list();
				if (run_breadth_first_simulation()){
					std::cout << "Solution found" << '\n';
					break;
				}else{
					//std::cout << "No solution was found" << '\n';
				}
			}
			init_mid_complete_board(&b);
			nodes_evaluated = 0;
			nodes_found.clear();
			node_checksums.clear();
			nodes_found.push_back(b);
		}else if(input == 2){	//Depth first
			std::cout << "What value for maximum depth?" << '\n';
			std::cin >> maximum_depth;
			if (run_depth_first_simulation(b,0)){
				std::cout << "Solution found" << '\n';
			}else{
				std::cout << "No solution was found" << '\n';
			}
		}else if(input == 3){	//Iterative depth first
			std::cout << "What value for maximum depth?" << '\n';
			std::cin >> maximum_depth;
			if(run_iterative_deepening_search(b)){
				std::cout << "Solution found" << '\n';
			}else{
				std::cout << "No solution was found" << '\n';
			}
		}else if(input == 4){
			run_random_move_simulation(b);
		}
		input = run_options(b);
	}
	return 0;

	//if (run_depth_first_simulation(b,0)) std::cout << "FINISHED" << '\n';
	//return 0;

	while (nodes_evaluated < 10000){
		if (run_breadth_first_simulation()){
			std::cout << "result of sim = true" << '\n';
			break;
		}else{
			std::cout << "nodes expanded = " << nodes_evaluated << '\n';
			std::cout << "result of sim = false" << '\n';
		}
	}
}

void print_node_checksum_list(){
	for (std::vector<long>::iterator it = node_checksums.begin() ; it != node_checksums.end(); ++it){
		std::cout << *it << ", ";
	}
}

long get_checksum(board b){
	long index, checksum = 0;
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			index = BOARD_SIZE*i + j;	
			index = (pow(BOARD_SIZE,2)-1)-index;
			index = pow(TOWER_HEIGHT+2,index);
			//std::cout << "index = " << index << '\n';
			if (b.data[j][i]=='0'){			    //REPLACE THIS WITH VECTOR OF SYMBOLS?
				//std::cout << "value at " << index << " is a 0. " << index*0 << " added to checksum." << '\n';
				checksum += index*0;
			}else if (b.data[j][i]=='A'){
				//std::cout << "value at " << index << " is an A. " << index*1 << " added to checksum." << '\n';
				checksum += index*1;
			}else if (b.data[j][i]=='B'){
				//std::cout << "value at " << index << " is a B. "  << index*2 << " added to checksum." << '\n';
				checksum += index*2;
			}else if (b.data[j][i]=='C'){
				//std::cout << "value at " << index << " is a C. " << index*3 << " added to checksum." << '\n';
				checksum += index*3;
			}else if (b.data[j][i]=='#'){
				//std::cout << "value at " << index << " is a #. " << index*4 << " added to checksum." << '\n';
				checksum += index*4;
			}
			//std::cout << "checksum = " << checksum << '\n' << '\n';
		}
	}
	return checksum;
}

bool board_checksum_exists(board b){
	long board_checksum = get_checksum(b);
	for (int i = 0; i < node_checksums.size(); i++){
		if (node_checksums[i] == board_checksum) return true;
	}
	return false;
}	

bool board_checksum_exists(long board_checksum){
	for (int i = 0; i < node_checksums.size(); i++){
		if (node_checksums[i] == board_checksum) return true;
	}
	return false;
}

int run_options(board b){
	std::cout << '\n';
	std::cout << "Which type of search to do on this board?" << '\n';
	print_board(b);
	std::cout << "1. Breadth first search" << '\n';
	std::cout << "2. Depth first search" << '\n';
	std::cout << "3. Iterative depth first search" << '\n';
	std::cout << "4. Random move search" << '\n';
	std::cout << "0. Exit program" << '\n';
	int input;
	std::cin >> input;
	return input;
}

//Random, single path node expansion
int run_random_move_simulation(board b){
	int n = 0;
	while (!is_complete(b)){
		b=move_agent(b,(rand()%4)+1);
		n++;
	}
	return n;
}

//Breadth first search
bool run_breadth_first_simulation(){
	
	nodes_evaluated++;
	//std::cout << "adding checksum " << get_checksum(nodes_found.back()) << '\n';
	//node_checksums.push_back(get_checksum(nodes_found.back()));
	
	std::cout << "Entered function.     Board at depth " << nodes_found.back().depth << "   Nodes evaluated = " << nodes_evaluated << "   Original board:" << '\n';
	
	print_board(nodes_found.back());
	
	std::cout << '\n';
	
	bool result = is_complete(nodes_found.back());
	if (result){
		std::cout << "Final board = " << '\n';
		print_board(nodes_found.back());
		std::cout << '\n';
		return true;
	}
	
	board b_up;
	b_up = move_agent(nodes_found.back(),1);
	b_up.depth++;
	board b_down;
	b_down = move_agent(nodes_found.back(),3);
	b_down.depth++;
	board b_left;
	b_left = move_agent(nodes_found.back(),4);
	b_left.depth++;
	board b_right;
	b_right = move_agent(nodes_found.back(),2);
	b_right.depth++;
	
	std::vector<int> direction_order_array = shuffled_direction_vect();
	long cs;
	std::cout << "direction order array = " << direction_order_array[0] << ", " << direction_order_array[1] << ", " << direction_order_array[2] << ", " << direction_order_array[3] << '\n';
	for (int i = 0; i < 4; i++){
		if (direction_order_array[i]==1 && can_move_direction(b_up,1)){
			cs = get_checksum(b_up);
			std::cout << "checksum for board b_up = " << cs << '\n';
			print_board(b_up);
			if (!board_checksum_exists(cs)){
				nodes_found.insert(begin(nodes_found),b_up);
				node_checksums.push_back(cs);
			}else{
				std::cout << "checksum for b_up exists in vector *****************" << '\n';
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==2 && can_move_direction(b_right,2)){
			cs = get_checksum(b_right);
			std::cout << "checksum for board b_right = " << cs << '\n';
			print_board(b_right);
			if (!board_checksum_exists(cs)){
				nodes_found.insert(begin(nodes_found),b_right);
				node_checksums.push_back(cs);
			}else{
				std::cout << "checksum for b_right exists in vector *****************" << '\n';
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==3 && can_move_direction(b_down,3)){
			cs = get_checksum(b_down);
			std::cout << "checksum for board b_down = " << cs << '\n';
			print_board(b_down);
			if (!board_checksum_exists(cs)){
				nodes_found.insert(begin(nodes_found),b_down);
				node_checksums.push_back(cs);
			}else{
				std::cout << "checksum for b_down exists in vector *****************" << '\n';
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==4 && can_move_direction(b_left,4)){
			cs = get_checksum(b_left);
			std::cout << "checksum for board b_left = " << cs << '\n';
			print_board(b_left);
			if (!board_checksum_exists(cs)){
				nodes_found.insert(begin(nodes_found),b_left);
				node_checksums.push_back(cs);
			}else{
				std::cout << "checksum for b_left exists in vector *****************" << '\n';
				checksum_matches++;
			}
		}
	}
	
	//print_board(nodes_found.back());
	nodes_found.pop_back();
	std::cout << "Nodes found size = " << nodes_found.size() << "   Checksum matches = " << checksum_matches << '\n';
	std::cout << '\n' << '\n';
	
	
	return false;
}

//Iterative deepening search
bool run_iterative_deepening_search(board b){
	for (int i = 0; i < maximum_depth; i++){
		if (run_depth_first_simulation(nodes_found.back(),maximum_depth-i)){
			return true;
		}
	}
	return false;
}

//Depth first search
bool run_depth_first_simulation(board b, int depth){
	
	if (depth >= maximum_depth) return false;
	std::cout << "Board at depth " << depth << "   Nodes evaluated = " << nodes_evaluated << "   Max depth = " << maximum_depth << '\n';
	nodes_evaluated++;
    board b_left;
    board b_right;
    board b_up;
    board b_down;

	std::vector<int> direction_order_array = shuffled_direction_vect();
	
	for (int i=0; i<4; i++){
		if (direction_order_array[i]==4){
			b_left = move_agent(b,4);
			if (b_left.move_flag){
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
			b_up = move_agent(b,1);
			if (b_up.move_flag){
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
			b_right = move_agent(b,2);
			if (b_right.move_flag){
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
			b_down = move_agent(b,3);
			if (b_down.move_flag){
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

bool can_move_direction(board b, int direction){
	if (direction == 1){
		return b.agent[1] > 0;
	}else if (direction == 2){
		return b.agent[0] < BOARD_SIZE-1;
	}else if (direction == 3){
		return b.agent[1] < BOARD_SIZE-1;
	}else if (direction == 4){
		return b.agent[0] > 0;
	}
	return false;
}

board move_agent(board b, int direction){
	
    char temp;
	if (can_move_direction(b,direction)){
		if (direction == 1){
			temp = b.data[b.agent[0]][b.agent[1]-1];
			b.data[b.agent[0]][b.agent[1]-1] = b.data[b.agent[0]][b.agent[1]];
			b.data[b.agent[0]][b.agent[1]] = temp;
			b.agent[1]--;
			return b;
		}else if(direction == 2){
			temp = b.data[b.agent[0]+1][b.agent[1]];
			b.data[b.agent[0]+1][b.agent[1]] = b.data[b.agent[0]][b.agent[1]];
			b.data[b.agent[0]][b.agent[1]] = temp;
			b.agent[0]++;
			return b;
		}else if(direction == 3){
			temp = b.data[b.agent[0]][b.agent[1]+1];
			b.data[b.agent[0]][b.agent[1]+1] = b.data[b.agent[0]][b.agent[1]];
			b.data[b.agent[0]][b.agent[1]] = temp;
			b.agent[1]++;
			return b;
		}else if(direction == 4){
			temp = b.data[b.agent[0]-1][b.agent[1]];
			b.data[b.agent[0]-1][b.agent[1]] = b.data[b.agent[0]][b.agent[1]];
			b.data[b.agent[0]][b.agent[1]] = temp;
			b.agent[0]--;
			return b;
		}
	}
    //std::cout << "CANNOT MOVE THAT WAY!" << '\n';
	b.move_flag = false;
    return b;
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
	b->depth=0;
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
	b->agent[1]=1;
	b->depth=0;
}

void init_mid_complete_board(board *b){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
	}
	b->data[1][0] = 'A';
	b->data[1][2] = 'B';
	b->data[1][3] = 'C';
	b->data[3][3] = '#';
	b->agent[0]=3;
	b->agent[1]=3;
	b->depth=0;
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
