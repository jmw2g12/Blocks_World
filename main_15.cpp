#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>

//Scalability investigation variables
#define BOARD_SIZE 3
#define TOWER_HEIGHT 2

struct board{
	char data[BOARD_SIZE][BOARD_SIZE];
	int agent[2];
	int depth;
	bool cant_move;
	int manhattan_cost;
};

//Methods
void print_board(board b);
board move_agent(board b, int direction);
void init_board(board *b);
void init_almost_complete_board(board *b);
void init_complete_board(board *b);
void init_mid_complete_board(board *b);
void init_random_board(board *b);
bool is_complete(board b);
bool run_depth_first_simulation(board b, int depth, bool without_checksum);
bool run_breadth_first_simulation(bool without_checksums);
bool run_iterative_deepening_search(board b);
bool run_iterative_deepening_depth_search(board b, int depth, std::vector<int64_t> node_checksums_previous_iteration);
bool run_a_star_search(bool without_checksums);
int compute_manhattan_distance(board b);
bool can_move_direction(board b, int direction);
std::vector<int> shuffled_direction_vect();
int run_options(board b);
int64_t get_checksum(board b);
bool board_checksum_exists(board b);
bool board_checksum_exists(int64_t board_checksum);
bool cs_exists_in_vector(std::vector<int64_t> vect, int64_t val);

//Global variables
int nodes_evaluated = 0;
int checksum_matches = 0;
int maximum_depth = 10;
std::vector<board> nodes_found;
std::vector<int64_t> node_checksums;

//Starting point
int main(int argc, char** argv){
	
    board b;
	init_board(&b);
	nodes_found.push_back(b);
	node_checksums.push_back(get_checksum(b));

	int input = run_options(b);
	while (input != 0){		//Main control loop
		if (input == 1){	//Breadth first
			std::cout << "Maximum number of nodes to evaluate?" << '\n';
			std::cin >> input;
			bool result = false;
			while (nodes_evaluated <= input){
				if (run_breadth_first_simulation(true)){
					result = true;
					break;
				}
			}
			if (!result) std::cout << '\n' << "---No solution found---" << '\n';
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if(input == 2){	//Depth first
			std::cout << "What value for maximum depth?" << '\n';
			std::cin >> maximum_depth;
			if (!run_depth_first_simulation(b,0,true)){
				std::cout << '\n' << "---No solution found---" << '\n';
			}
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if(input == 3){	//Iterative depth first
			std::cout << "What value for maximum depth?" << '\n';
			std::cin >> maximum_depth;
			if(!run_iterative_deepening_search(b)){
				std::cout << '\n' << "---No solution found---" << '\n';
			}
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if (input == 4){	//A* heuristic search
			std::cout << "Maximum number of nodes to evaluate?" << '\n';
			std::cin >> input;
			bool result = false;
			while (nodes_evaluated <= input){
				if (run_a_star_search(true)){
					result = true;
					break;
				}
			}
			if (!result) std::cout << '\n' << "---No solution found---" << '\n';
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if (input == 5){	//Breadth first with CS
			std::cout << "Maximum number of nodes to evaluate?" << '\n';
			std::cin >> input;
			bool result = false;
			while (nodes_evaluated <= input){
				if (run_breadth_first_simulation(false)){
					result = true;
					break;
				}
			}
			if (!result) std::cout << '\n' << "---No solution found---" << '\n';
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if(input == 6){	//Depth first with CS
			std::cout << "What value for maximum depth?" << '\n';
			std::cin >> maximum_depth;
			if (!run_depth_first_simulation(b,0,false)){
				std::cout << '\n' << "---No solution found---" << '\n';
			}
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}else if (input == 7){	//A* heuristic with CS
			std::cout << "Maximum number of nodes to evaluate?" << '\n';
			std::cin >> input;
			bool result = false;
			while (nodes_evaluated < input){
				if (run_a_star_search(false)){
					result = true;
					break;
				}
			}
			if (!result) std::cout << '\n' << "---No solution found---" << '\n';
			std::cout << "<press any button to continue>" << '\n';
			std::cin.ignore();
			std::cin.ignore();
		}
		
		//reset variables
		init_board(&b);
		nodes_evaluated = 0;
		nodes_found.clear();
		node_checksums.clear();
		nodes_found.push_back(b);
		checksum_matches = 0;
		input = run_options(b);
	}
	return 0;
}

int run_options(board b){
	std::cout << '\n';
	std::cout << "Which type of search to do on this board?" << '\n';
	print_board(b);
	std::cout << "1. Breadth first search" << '\n';
	std::cout << "2. Depth first search" << '\n';
	std::cout << "3. Iterative depth first search" << '\n';
	std::cout << "4. A* heuristic search" << '\n';
	std::cout << "5. Breadth first search (with checksum functionality)" << '\n';
	std::cout << "6. Depth first search (with checksum functionality)" << '\n';
	std::cout << "7. A* heauristic search (with checksum functionality)" << '\n';
	std::cout << "0. Exit program" << '\n';
	int input;
	std::cin >> input;
	return input;
}

bool run_breadth_first_simulation(bool without_checksums){
	
	board b = nodes_found.back();
	
	bool result = is_complete(b);
	if (result){
		std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b.depth << '\n' << "Final board:" << '\n';
		print_board(b);
		std::cout << '\n';
		return true;
	}
	
	nodes_evaluated++;
	
	std::vector<int> direction_order_array = shuffled_direction_vect();
	int64_t cs;
	for (int i = 0; i < 4; i++){
		if (direction_order_array[i]==1 && can_move_direction(b,1)){
			board b_up;
			b_up = move_agent(b,1);
			b_up.depth++;
			cs = get_checksum(b_up);
			if (!board_checksum_exists(cs) || without_checksums){
				nodes_found.insert(begin(nodes_found),b_up);
				node_checksums.push_back(cs);
			}else{
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==2 && can_move_direction(b,2)){
			board b_right;
			b_right = move_agent(b,2);
			b_right.depth++;
			cs = get_checksum(b_right);
			if (!board_checksum_exists(cs) || without_checksums){
				nodes_found.insert(begin(nodes_found),b_right);
				node_checksums.push_back(cs);
			}else{
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==3 && can_move_direction(b,3)){
			board b_down;
			b_down = move_agent(b,3);
			b_down.depth++;
			cs = get_checksum(b_down);
			if (!board_checksum_exists(cs) || without_checksums){
				nodes_found.insert(begin(nodes_found),b_down);
				node_checksums.push_back(cs);
			}else{
				checksum_matches++;
			}
		}
		if (direction_order_array[i]==4 && can_move_direction(b,4)){
			board b_left;
			b_left = move_agent(b,4);
			b_left.depth++;
			cs = get_checksum(b_left);
			if (!board_checksum_exists(cs) || without_checksums){
				nodes_found.insert(begin(nodes_found),b_left);
				node_checksums.push_back(cs);
			}else{
				checksum_matches++;
			}
		}
	}
	nodes_found.pop_back();	
	return false;
}

bool run_iterative_deepening_search(board b){
	std::vector<int64_t> node_checksums_previous_iteration;
	for (int i = 0; i < maximum_depth; i++){
		std::cout << "i = " << i << "   ************" << '\n';
		if (run_iterative_deepening_depth_search(b,maximum_depth-i,node_checksums_previous_iteration)){
			return true;
		}
		node_checksums_previous_iteration = node_checksums;
		node_checksums.clear();
	}
	return false;
}

bool run_iterative_deepening_depth_search(board b, int depth, std::vector<int64_t> node_checksums_previous_iteration){
	
	if (depth > maximum_depth) return false;
	nodes_evaluated++;
    board b_left;
    board b_right;
    board b_up;
    board b_down;

	std::vector<int> direction_order_array = shuffled_direction_vect();
	
	for (int i=0; i<4; i++){
		if (direction_order_array[i]==4){
			b_left = move_agent(b,4);
			b_left.depth = depth + 1;
			if (!b_left.cant_move){
				if (!board_checksum_exists(b_left)){
					int64_t cs = get_checksum(b_left);
					node_checksums.push_back(cs);
					if (!cs_exists_in_vector(node_checksums_previous_iteration,cs)){
						std::cout << "b.depth = " << b.depth << "    b_left.depth = " << b_left.depth << "    depth = " << depth << '\n';
						if (is_complete(b_left)){
							std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_left.depth << '\n' << "Final board:" << '\n';
							print_board(b_left);
							std::cout << '\n';
							return true;
						}else{
							if (run_iterative_deepening_depth_search(b_left,depth+1,node_checksums_previous_iteration)){
								return true;
							}
						}
					}else{
						if (run_iterative_deepening_depth_search(b_left,depth+1,node_checksums_previous_iteration)){
							return true;
						}
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==1){
			b_up = move_agent(b,1);
			b_up.depth = depth + 1;
			if (!b_up.cant_move){
				if (!board_checksum_exists(b_up)){
					int64_t cs = get_checksum(b_up);
					node_checksums.push_back(cs);
					if (!cs_exists_in_vector(node_checksums_previous_iteration,cs)){
						std::cout << "b.depth = " << b.depth << "    b_up.depth = " << b_up.depth << "    depth = " << depth << '\n';
						if (is_complete(b_up)){
							std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_up.depth << '\n' << "Final board:" << '\n';
							print_board(b_up);
							std::cout << '\n';
							return true;
						}else{
							if (run_iterative_deepening_depth_search(b_up,depth+1,node_checksums_previous_iteration)){
								return true;
							}
						}
					}else{
						if (run_iterative_deepening_depth_search(b_up,depth+1,node_checksums_previous_iteration)){
							return true;
						}
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==2){
			b_right = move_agent(b,2);
			b_right.depth = depth + 1;
			if (!b_right.cant_move){
				if (!board_checksum_exists(b_right)){
					int64_t cs = get_checksum(b_right);
					node_checksums.push_back(cs);
					if (!cs_exists_in_vector(node_checksums_previous_iteration,cs)){
						std::cout << "b.depth = " << b.depth << "    b_right.depth = " << b_right.depth << "    depth = " << depth << '\n';
						if (is_complete(b_right)){
							std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_right.depth << '\n' << "Final board:" << '\n';
							print_board(b_right);
							std::cout << '\n';
							return true;
						}else{
							if (run_iterative_deepening_depth_search(b_right,depth+1,node_checksums_previous_iteration)){
								return true;
							}
						}
					}else{
						if (run_iterative_deepening_depth_search(b_right,depth+1,node_checksums_previous_iteration)){
							return true;
						}
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==3){
			b_down = move_agent(b,3);
			b_down.depth = depth + 1;
			if (!b_down.cant_move){
				if (!board_checksum_exists(b_down)){
					int64_t cs = get_checksum(b_down);
					node_checksums.push_back(cs);
					if (!cs_exists_in_vector(node_checksums_previous_iteration,cs)){
						std::cout << "b.depth = " << b.depth << "    b_down.depth = " << b_down.depth << "    depth = " << depth << '\n';
						if (is_complete(b_down)){
							std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_down.depth << '\n' << "Final board:" << '\n';
							print_board(b_down);
							std::cout << '\n';
							return true;
						}else{
							if (run_iterative_deepening_depth_search(b_down,depth+1,node_checksums_previous_iteration)){
								return true;
							}
						}
					}else{
						if (run_iterative_deepening_depth_search(b_down,depth+1,node_checksums_previous_iteration)){
							return true;
						}
					}
				}else{
					checksum_matches++;
				}
			}
		}
	}
	return false;
}

bool run_depth_first_simulation(board b, int depth, bool without_checksum){
	
	nodes_evaluated++;
	if (depth >= maximum_depth) return false;
    board b_left;
    board b_right;
    board b_up;
    board b_down;

	std::vector<int> direction_order_array = shuffled_direction_vect();
	
	for (int i=0; i<4; i++){
		if (direction_order_array[i]==4){
			b_left = move_agent(b,4);
			b_left.depth = depth + 1;
			if (!b_left.cant_move){
				if (!board_checksum_exists(b_left) || without_checksum){
					node_checksums.push_back(get_checksum(b_left));
					//std::cout << "Moving left.  " << nodes_evaluated << '\n';
					if (is_complete(b_left)){
						std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_left.depth << '\n' << "Final board:" << '\n';
						print_board(b_left);
						std::cout << '\n';
						return true;
					}else{
						if (run_depth_first_simulation(b_left,depth+1,without_checksum)){
							return true;
						}
						//std::cout << "Returning from path." << '\n';
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==1){
			b_up = move_agent(b,1);
			b_up.depth = depth + 1;
			if (!b_up.cant_move){
				if (!board_checksum_exists(b_up) || without_checksum){
					node_checksums.push_back(get_checksum(b_up));
					//std::cout << "Moving up.  " << nodes_evaluated << '\n';
					if (is_complete(b_up)){
						std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_up.depth << '\n' << "Final board:" << '\n';
						print_board(b_up);
						std::cout << '\n';
						return true;
					}else{
						if (run_depth_first_simulation(b_up,depth+1,without_checksum)){
							return true;
						}
						//std::cout << "Returning from path." << '\n';
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==2){
			b_right = move_agent(b,2);
			b_right.depth = depth + 1;
			if (!b_right.cant_move){
				if (!board_checksum_exists(b_right) || without_checksum){
					node_checksums.push_back(get_checksum(b_right));
					//std::cout << "Moving right.  " << nodes_evaluated << '\n';
					if (is_complete(b_right)){
						std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_right.depth << '\n' << "Final board:" << '\n';
						print_board(b_right);
						std::cout << '\n';
						return true;
					}else{
						if (run_depth_first_simulation(b_right,depth+1,without_checksum)){
							return true;
						}
						//std::cout << "Returning from path." << '\n';
					}
				}else{
					checksum_matches++;
				}
			}
		}
		if (direction_order_array[i]==3){
			b_down = move_agent(b,3);
			b_down.depth = depth + 1;
			if (!b_down.cant_move){
				if (!board_checksum_exists(b_down) || without_checksum){
					node_checksums.push_back(get_checksum(b_down));
					//std::cout << "Moving down.  " << nodes_evaluated << '\n';
					if (is_complete(b_down)){
						std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b_down.depth << '\n' << "Final board:" << '\n';
						print_board(b_down);
						std::cout << '\n';
						return true;
					}else{
						if (run_depth_first_simulation(b_down,depth+1,without_checksum)){
							return true;
						}
						//std::cout << "Returning from path." << '\n';
					}
				}else{
					checksum_matches++;
				}
			}
		}
	}
	return false;
}

bool run_a_star_search(bool without_checksums){
	board b = nodes_found[0];
	int b_index = 0;
	int minimum_cost = nodes_found[0].manhattan_cost;
	for (int i = 1; i < nodes_found.size(); i++){
		if (nodes_found[i].manhattan_cost < minimum_cost){
			minimum_cost = nodes_found[i].manhattan_cost;
			b = nodes_found[i];
			b_index = i;
		}
	}
	
	nodes_evaluated++;
	
	bool result = is_complete(b);
	if (result){
		std::cout << '\n' << '\n' << "---Solution found---" << '\n' << "Nodes evaluated = " << nodes_evaluated << '\t' << '\t' << "Board at depth = " << b.depth << '\n' << "Final board:" << '\n';
		print_board(b);
		std::cout << '\n';
		return true;
	}
	
	int64_t cs;
	\
	board b_up;
	b_up = move_agent(b,1);
	b_up.depth++;
	b_up.manhattan_cost = b_up.depth + compute_manhattan_distance(b_up);
	cs = get_checksum(b_up);
	if (!board_checksum_exists(cs) || without_checksums){
		nodes_found.push_back(b_up);
		node_checksums.push_back(cs);
	}else{
		checksum_matches++;
	}

	board b_right;
	b_right = move_agent(b,2);
	b_right.depth++;
	b_right.manhattan_cost = b_right.depth + compute_manhattan_distance(b_right);
	cs = get_checksum(b_right);
	if (!board_checksum_exists(cs) || without_checksums){
		nodes_found.push_back(b_right);
		node_checksums.push_back(cs);
	}else{
		checksum_matches++;
	}

	board b_down;
	b_down = move_agent(b,3);
	b_down.depth++;
	b_down.manhattan_cost = b_down.depth + compute_manhattan_distance(b_down);
	cs = get_checksum(b_down);
	if (!board_checksum_exists(cs) || without_checksums){
		nodes_found.push_back(b_down);
		node_checksums.push_back(cs);
	}else{
		checksum_matches++;
	}

	board b_left;
	b_left = move_agent(b,4);
	b_left.depth++;
	b_left.manhattan_cost = b_left.depth + compute_manhattan_distance(b_left);
	cs = get_checksum(b_left);
	if (!board_checksum_exists(cs) || without_checksums){
		nodes_found.push_back(b_left);
		node_checksums.push_back(cs);
	}else{
		checksum_matches++;
	}
		
	nodes_found.erase(nodes_found.begin()+b_index);	
	
	return false;
}

int compute_manhattan_distance(board b){
	int total_distance = 0;
	for (int i=0; i < BOARD_SIZE; i++){
		for (int j=0; j < BOARD_SIZE; j++){
			if (b.data[i][j] == 'A'){
				total_distance += std::abs(i-((BOARD_SIZE/2)-1));
				total_distance += std::abs(j-(BOARD_SIZE-3));
			}else if (b.data[i][j] == 'B'){
				total_distance += std::abs(i-((BOARD_SIZE/2)-1));
				total_distance += std::abs(j-(BOARD_SIZE-2));
			}else if (b.data[i][j] == 'C'){
				total_distance += std::abs(i-((BOARD_SIZE/2)-1));
				total_distance += std::abs(j-(BOARD_SIZE-1));
			}else if (b.data[i][j] == '#'){
				total_distance += std::abs(i-(BOARD_SIZE-1));
				total_distance += std::abs(j-(BOARD_SIZE-1));
			}
		}
	}
	return total_distance;
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
	b.cant_move = true;
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
	for (int i = 0; i < TOWER_HEIGHT; i++){
		char temp = char(65+(TOWER_HEIGHT-i-1));
		b->data[TOWER_HEIGHT-i-1][BOARD_SIZE-1] = temp;
	}
    b->data[BOARD_SIZE-1][BOARD_SIZE-1] = '#';
	b->agent[0] = BOARD_SIZE-1;
	b->agent[1] = BOARD_SIZE-1;
	b->depth=0;
}

void init_almost_complete_board(board *b){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
    }
	for (int i = 0; i < TOWER_HEIGHT; i++){
		char temp = char(65+i);
		b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)+i] = temp;
	}
	b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)] = '#';
	b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)-1] = 'A';
	b->agent[0]=lround(BOARD_SIZE/2)-1;
	b->agent[1]=(BOARD_SIZE-TOWER_HEIGHT);
	b->depth=0;
}

void init_complete_board(board *b){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
    }
	for (int i = 0; i < TOWER_HEIGHT; i++){
		char temp = char(65+i);
		b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)+i] = temp;
	}
	b->data[BOARD_SIZE-1][BOARD_SIZE-1] = '#';
	b->agent[0]=BOARD_SIZE-1;
	b->agent[1]=BOARD_SIZE-1;
	b->depth=0;
}

void init_mid_complete_board(board *b){
	for (int i = 0; i < BOARD_SIZE; i++){
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
        }
    }
	for (int i = 0; i < TOWER_HEIGHT; i++){
		char temp = char(65+i);
		b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)+i] = temp;
	}
	b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)] = '0';
	b->data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)-1] = 'A';
	b->data[BOARD_SIZE-1][BOARD_SIZE-1] = '#';
	b->agent[0]=BOARD_SIZE-1;
	b->agent[1]=BOARD_SIZE-1;
	b->depth=0;
}

void init_random_board(board *b){
	
	// *** DOESN'T CURRENTLY WORK TOO WELL ***
	std::vector<int> potential_x_coords;
	std::vector<int> potential_y_coords;
	for (int i = 0; i < BOARD_SIZE; i++){
		potential_x_coords.push_back(i);
        for (int j = 0; j < BOARD_SIZE; j++){
            b->data[i][j] = '0';
			potential_y_coords.push_back(j);
        }
    }
	std::random_shuffle(potential_x_coords.begin(),potential_x_coords.end());
	std::random_shuffle(potential_y_coords.begin(),potential_y_coords.end());
	
	for (int i = 0; i < potential_x_coords.size(); i++){
		std::cout << potential_x_coords[i] << ", " << potential_x_coords[i] << '\n';
	}
	for (int i = 0; i < TOWER_HEIGHT + 1; i++){
		if (i == 0){
			b->data[potential_x_coords[0]][potential_y_coords[0]] = '#';
		}else if (i == 1){
			b->data[potential_x_coords[1]][potential_y_coords[1]] = 'A';
		}else if (i == 2){
			b->data[potential_x_coords[2]][potential_y_coords[2]] = 'B';
		}else if (i == 3){
			b->data[potential_x_coords[3]][potential_y_coords[3]] = 'C';
		}
	}
	
	// *** DOESN'T CURRENTLY WORK TOO WELL ***
}

bool is_complete(board b){
	
	for (int i = 0; i < TOWER_HEIGHT; i++){
		char temp = char(65+i);
		if (! (b.data[lround(BOARD_SIZE/2)-1][(BOARD_SIZE-TOWER_HEIGHT)+i] == temp) ) return false;
	}
	if (! (b.data[BOARD_SIZE-1][BOARD_SIZE-1] == '#')) return false;

    return true;
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

bool cs_exists_in_vector(std::vector<int64_t> vect, int64_t val){
	for (int i = 0; i < vect.size(); i++){
		if (vect[i] == val) return true;
	}
	return false;
}

int64_t get_checksum(board b){
	int64_t index = 0;
	int64_t checksum = 0;
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			index = BOARD_SIZE*i + j;	
			index = (pow(BOARD_SIZE,2)-1)-index;
			index = pow(TOWER_HEIGHT+2,index);
			if (b.data[j][i]=='0'){
				checksum += index*0;
			}else if (b.data[j][i]=='#'){
				checksum += index*1;
			}else if (b.data[j][i]=='A'){
				checksum += index*2;
			}else if (b.data[j][i]=='B'){
				checksum += index*3;
			}else if (b.data[j][i]=='C'){
				checksum += index*4;
			}else if (b.data[j][i]=='D'){
				checksum += index*5;
			}else if (b.data[j][i]=='E'){
				checksum += index*6;
			}else if (b.data[j][i]=='F'){
				checksum += index*7;
			}else if (b.data[j][i]=='G'){
				checksum += index*8;
			}
		}
	}
	return checksum;
}

bool board_checksum_exists(board b){
	int64_t board_checksum = get_checksum(b);
	for (int i = 0; i < node_checksums.size(); i++){
		if (node_checksums[i] == board_checksum) return true;
	}
	return false;
}	

bool board_checksum_exists(int64_t board_checksum){
	for (int i = 0; i < node_checksums.size(); i++){
		if (node_checksums[i] == board_checksum) return true;
	}
	return false;
}
