
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>


const int DEPTH_TO_SEARCH = 4;


class game_state
{
public:
	int state[6][6];
	int heuristic;


	game_state(int state[6][6])
	{
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
				this->state[i][j] = state[i][j];
		heuristic = heuristic_for_element();
	}
	game_state()
	{}

	int heuristic_for_element()
	{

		int count_player = 0;
		int count_pc = 0;
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
				if (state[i][j] == 1)
					count_player++;
				else if (state[i][j] == -1)
					count_pc++;
		return count_player - count_pc;
	}

};


class infection
{

	game_state state;
	int player;
	int winner;
	int first_turn;

	//Generates possible moves for choosen player (1 or -1).
	std::vector<game_state> generate_moves(int side, game_state(state))
	{
		std::vector<game_state> result;
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
			{
				if (state.state[i][j] == side)
				{
					for (int it = 0; it < 6; it++)
						for (int jt = 0; jt < 6; jt++)
							if (state.state[it][jt] == 0)
							{
								if (std::abs(it - i) <= 1 && std::abs(jt - j) <= 1)
								{
									int tmp[6][6];
									for (int i = 0; i < 6; i++)
										for (int j = 0; j < 6; j++)
											tmp[i][j] = state.state[i][j];
									tmp[it][jt] = side;
									for (int itt = 0; itt < 6; itt++)
										for (int jtt = 0; jtt < 6; jtt++)
										{
											if (std::abs(itt - it) <= 1 && std::abs(jtt - jt) <= 1 && tmp[itt][jtt] == (-side))
											{
												tmp[itt][jtt] = side;
											}
										}
									//print(tmp);
									result.push_back(game_state(tmp));

								}
								else if ((std::abs(it - i) <= 2 && std::abs(jt - j) <= 2))
								{
									int tmp[6][6];
									for (int i = 0; i < 6; i++)
										for (int j = 0; j < 6; j++)
											tmp[i][j] = state.state[i][j];
									tmp[i][j] = 0;
									tmp[it][jt] = side;
									for (int itt = 0; itt < 6; itt++)
										for (int jtt = 0; jtt < 6; jtt++)
										{
											if (std::abs(itt - it) <= 1 && std::abs(jtt - jt) <= 1 && tmp[itt][jtt] == (-side))
											{
												tmp[itt][jtt] = side;
											}
										}
									//print(tmp);
									result.push_back(game_state(tmp));
								}
							}
				}
			}
		return result;

	}

	//Current state is matrix of int with values = -1,0,1. It is in not comfortable to print it like that.
	//So this function sets more comfortable chars to print
	char translate(int i)
	{
		return (i == 1) ? 'A' : (i == -1) ? 'B' : '_';
	}

	//Function to print current station of game
	void print()
	{
		std::cout << "   ";
		for (int i = 0; i < 6; i++)
			std::cout << i + 1 << ' ';
		std::cout << std::endl << "  ";
		for (int i = 0; i < 6; i++)
			std::cout << "__";
		std::cout << std::endl;
		for (int i = 0; i < 6; i++)
		{
			std::cout << i + 1 << "| ";
			for (int j = 0; j < 6; j++)
				std::cout << translate(state.state[i][j]) << ' ';
			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;


	}

	//Next 2 functions finding best move for players using heuristic function and alpha-beta prunning algorithm. We need 2 functions, because heuristic for players is opposite at the same state

	std::tuple<game_state, int> find_best_move1(game_state state, int side = 1, int depth = 4, int current_step = 1, int alpha = INT_MIN, int beta = INT_MAX)
	{
		if (depth == 0)
		{
			//print(state.state);
			//std::cout << state.heuristic << std::endl;

			return std::tuple<game_state, int>(state, state.heuristic);
		}
		int best_on_this_level = INT_MIN;
		int current;
		game_state current_best_state = this->state;

		if (current_step % 2 == 1)
		{
			for (auto x : generate_moves(side, state))
			{
				current = std::get<1>(find_best_move1(x.state, -side, depth - 1, current_step + 1, alpha, beta));
				alpha = std::max(alpha, current);

				if (current > best_on_this_level)
				{

					best_on_this_level = current;
					current_best_state = x;
				}
				if (beta < alpha)
				{
					//std::cout << "!!" << std::endl;
					break;
				}
			}
		}
		else
		{
			best_on_this_level = INT_MAX;
			for (auto x : generate_moves(side, state))
			{
				current = std::get<1>(find_best_move1(x.state, -side, depth - 1, current_step + 1, alpha, beta));

				if (current < best_on_this_level)
				{
					beta = std::min(current, beta);

					best_on_this_level = current;
					current_best_state = x;
				}
				if (beta < alpha)
				{
					//std::cout << "!!" << std::endl;
					break;
				}
			}
		}

		//print(current_best_state.state);
		//std::cout <<  std::endl;
		return std::make_tuple(current_best_state, best_on_this_level);
	}


	std::tuple<game_state, int> find_best_move2(game_state state, int side = -1, int depth = 4, int current_step = 1, int alpha = INT_MIN, int beta = INT_MAX)
	{
		if (depth == 0)
		{
			//print(state.state);
			//std::cout << state.heuristic << std::endl;
			//std::cout <<  -state.heuristic << std::endl;
			return std::tuple<game_state, int>(state, -state.heuristic);
		}
		int best_on_this_level = INT_MIN;
		int current;
		game_state current_best_state = this->state;

		if (current_step % 2 == 1)
		{
			for (auto x : generate_moves(side, state))
			{
				current = std::get<1>(find_best_move2(x.state, -side, depth - 1, current_step + 1, alpha, beta));
				alpha = std::max(alpha, current);
				if (current > best_on_this_level)
				{
					best_on_this_level = current;
					current_best_state = x;
				}
				if (beta < alpha)
				{
					//std::cout << "!!" << std::endl;
					break;
				}

			}
		}
		else
		{
			best_on_this_level = INT_MAX;
			for (auto x : generate_moves(side, state))
			{
				current = std::get<1>(find_best_move2(x.state, -side, depth - 1, current_step + 1, alpha, beta));
				beta = std::min(current, beta);
				if (current < best_on_this_level)
				{

					best_on_this_level = current;
					current_best_state = x;
				}

				if (beta < alpha)
				{
					//std::cout << "!!" << std::endl;
					break;
				}
			}
		}

		//print(current_best_state.state);
		//std::cout <<  std::endl;
		return std::make_tuple(current_best_state, best_on_this_level);
	}

	//Takes 1 and -1 as mark for which player make move. 
	void make_best_move(int player, int depth)
	{
		if (player == 1)
			state = std::get<0>(find_best_move1(state,1,depth));
		else state = std::get<0>(find_best_move2(state,-1,depth));
	}

	//Print rules of game
	void rules()
	{
		std::cout << "RULES OF INFECTION GAME" << std::endl << std::endl;
		std::cout << "Principles of movement:" << std::endl;
		std::cout << "1. Each of your units can move at any direction." << std::endl;
		std::cout << "2. It is possible to move only 2 cells away from current position." << std::endl;
		std::cout << "3. If unit moves 1 cell away, it duplicates itself. Original unit stays where he was and the copy moves to the directed cell." << std::endl;
		std::cout << "4. After each movement to new cell, all adjoining opponent's cells infecting and becaming your units." << std::endl << std::endl;


		std::cout << "Conditions for victory:" << std::endl;
		std::cout << "1. You've infected all opponent's units." << std::endl;
		std::cout << "2. There's no avaliable moves for both of you and the opponent and you have much units on the table." << std::endl << std::endl;

	}

	//Asks player for his move. In case of incorrect input asks again. Makes move.
	void user_input()
	{
	label1:
		int l1, r1, l2, r2;

		std::cout << "Enter point coordinates: ";
		std::cin >> l1 >> r1;
		int frees = 0;
		for (int itt = 0; itt < 6; itt++)
			for (int jtt = 0; jtt < 6; jtt++)
				if (std::abs(itt - l1 + 1) <= 2 && std::abs(jtt - r1 + 1) <= 2 && state.state[itt][jtt] == 0)
					frees++;
		if (frees == 0)
			goto label1;

		if (l1 < 1 || l1>6 || r1 < 1 || r1>6)
			goto label1;
		l1--;
		r1--;
		if (this->state.state[l1][r1] != this->player)
			goto label1;
	label2:
		std::cout << "Enter target coordinates: ";
		std::cin >> l2 >> r2;
		if (l2 == -100)
			goto label1;
		if (l2 < 1 || l2>6 || r2 < 1 || r2>6)
			goto label2;
		l2--;
		r2--;
		if (std::abs(l2 - l1) > 2 || std::abs(r2 - r1) > 2)
			goto label2;
		if (this->state.state[l2][r2] != 0)
			goto label2;
		if (std::abs(l2 - l1) <= 1 && std::abs(r2 - r1) <= 1)
			this->state.state[l2][r2] = player;
		else
		{
			this->state.state[l2][r2] = player;
			this->state.state[l1][r1] = 0;
		}
		for (int itt = 0; itt < 6; itt++)
			for (int jtt = 0; jtt < 6; jtt++)
				if (std::abs(itt - l2) <= 1 && std::abs(jtt - r2) <= 1 && state.state[itt][jtt] == (-player))
					state.state[itt][jtt] = player;


	}

	//Conditions for ending game
	bool game_over()
	{
		int count_player = 0;
		int count_pc = 0;
		for (int i = 0; i < 6; i++)
			for (int j = 0; j < 6; j++)
				if (state.state[i][j] == 1)
					count_player++;
				else if (state.state[i][j] == -1)
					count_pc++;
		if (count_pc == 0)
		{
			winner = 1;
			return true;
		}
		else if (count_player == 0)
		{
			winner = -1;
			return true;
		}
		else if (generate_moves(1, state).empty() && generate_moves(-1, state).empty())
		{
			if (state.heuristic > 0)
			{
				winner = 1;
				return true;
			}
			else if (state.heuristic < 0)
			{
				winner = -1;
				return true;
			}
			else
			{
				winner = 0;
				return true;
			}
		}
		return false;
	}

public:
	infection(int state[6][6],int player = 1, int first_turn = 1)
	{
		this->state = game_state(state);
		this->player = player;
		this->first_turn = first_turn;
	}



	//Start game. Player can set side and first turn;
	void start(int human_side = 1, int first_turn = 1)
	{
		this->player = human_side;

		this->first_turn = first_turn;

		rules();

		std::cout << "START POSITION" << std::endl << std::endl;

		print();

		if (this->first_turn != this->player)
		{
			if (!generate_moves(-1, state).empty())
			{
				make_best_move(-this->player, DEPTH_TO_SEARCH);
				std::cout << "OPPONENT MOVE" << std::endl << std::endl;
				print();
			}
		}

		while (!game_over())
		{
			if (!generate_moves(1,state).empty())
			{
				user_input();
				std::cout << std::endl << "YOUR MOVE" << std::endl << std::endl;
				print();
			}
			if (!generate_moves(-1, state).empty())
			{
				make_best_move(-this->player, DEPTH_TO_SEARCH);
				std::cout <<  "OPPONENT MOVE" << std::endl << std::endl;
				print();
			}
		}
		switch (winner)
		{
		case 1: 
			std::cout  << "CONGRATS! YOU WON!" << std::endl;
			break;
		case -1:
			std::cout << "YOU LOST" << std::endl;
			break;
		case 0:
			std::cout << "DRAW" << std::endl;
			break;

		}

	}

	//Start game without player with set search depth for both bots
	void automatic_game(int p1_d = 4, int p2_d = 4, int first_turn = 1)
	{
		this->first_turn = first_turn;
		std::cout << "START POSITION" << std::endl << std::endl;

		print();

		if (this->first_turn != this->player)
		{
			if (!generate_moves(-1, state).empty())
			{
				make_best_move(-this->player, DEPTH_TO_SEARCH);
				std::cout << "PLAYER 2 MOVE" << std::endl << std::endl;
				print();
			}
		}

		while (!game_over())
		{
			if (!generate_moves(1,state).empty())
			{
				make_best_move(this->player, p1_d);
				std::cout << std::endl << "PLAYER 1 MOVE" << std::endl << std::endl;
				print();
			}
			if (!generate_moves(-1, state).empty())
			{
				make_best_move(-this->player, p2_d);
				std::cout <<  "PLAYER 2 MOVE" << std::endl << std::endl;
				print();
			}
		}
		switch (winner)
		{
		case 1: 
			std::cout  << "PLAYER 1 WON!" << std::endl;
			break;
		case -1:
			std::cout << "PLAYER 2 WON!" << std::endl;
			break;
		case 0:
			std::cout << "DRAW" << std::endl;
			break;

			

		}
	}

};




int main()
{
	int state[6][6];
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			state[i][j] = 0;
	state[0][0] = -1;
	state[5][5] = -1;
	state[5][0] = 1;
	state[0][5] = 1;




	infection game(state);
	game.start(1,1);
	//game.automatic_game(5,5);
}

