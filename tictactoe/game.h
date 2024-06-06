#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iostream>

enum class Player {
	NONE, PLAYER_X, PLAYER_O
};

class Game {
private:
	std::vector<std::vector<Player>> board;
	int max_depth = 6;
	int board_size;

	int minimax(int depth, bool is_max, int alpha, int beta, Player ai);

	int evaluateBoard();

public:
	explicit Game(int size);

	bool makeMove(Player player, int row, int col);

	Player getWinner();

	bool isMovesLeft();

	void reset();

	void makeAiMove(Player ai);

	[[nodiscard]] const std::vector<std::vector<Player>> &getBoard() const;

	std::string hashBoard();
};

#endif //TICTACTOE_GAME_H
