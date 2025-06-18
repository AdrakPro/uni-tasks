#include "game.h"

Game::Game(int size) : board_size(size),
											 board(size, std::vector<Player>(size, Player::NONE)) {}

bool Game::makeMove(Player player, int row, int col) {
	if (board[row][col] == Player::NONE) {
		board[row][col] = player;
		return true;
	}

	return false;
}

void Game::makeAiMove(Player ai) {
	int best = std::numeric_limits<int>::min();
	int best_row = -1;
	int best_col = -1;

	for (int i = 0; i < board_size; ++i) {
		for (int j = 0; j < board_size; ++j) {
			if (board[i][j] == Player::NONE) {
				board[i][j] = ai;
				int move_value = minimax(
						0, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(),
						ai
				);
				board[i][j] = Player::NONE;

				if (move_value > best) {
					best_row = i;
					best_col = j;
					best = move_value;
				}
			}
		}
	}

	makeMove(ai, best_row, best_col);
}

int Game::minimax(int depth, bool is_max, int alpha, int beta, Player ai) {
	std::string board_hash = hashBoard();

	if (transposition_table.find(board_hash) != transposition_table.end()) {
		return transposition_table[board_hash];
	}

	Player player = ai == Player::PLAYER_X ? Player::PLAYER_O : Player::PLAYER_X;
	Player winner = getWinner();

	if (winner == Player::PLAYER_X) {
		return ai == Player::PLAYER_X ? 10 - depth : depth - 10;
	} else if (winner == Player::PLAYER_O) {
		return ai == Player::PLAYER_O ? 10 - depth : depth - 10;
	}

	if (!isMovesLeft() || depth > max_depth) {
		return 0;
	}

	if (is_max) {
		int best = std::numeric_limits<int>::min();

		for (int i = 0; i < board_size; ++i) {
			for (int j = 0; j < board_size; ++j) {
				if (board[i][j] == Player::NONE) {
					board[i][j] = ai;
					best = std::max(best, minimax(depth + 1, false, alpha, beta, ai));
					board[i][j] = Player::NONE;
					alpha = std::max(alpha, best);

					if (beta <= alpha) {
						break;
					}
				}
			}
		}

		transposition_table[board_hash] = best;
		return best;
	} else {
		int best = std::numeric_limits<int>::max();
		for (int i = 0; i < board_size; ++i) {
			for (int j = 0; j < board_size; ++j) {
				if (board[i][j] == Player::NONE) {
					board[i][j] = player;
					best = std::min(best, minimax(depth, true, alpha, beta, ai));
					board[i][j] = Player::NONE;
					beta = std::min(beta, best);

					if (beta <= alpha) {
						break;
					}
				}
			}
		}

		transposition_table[board_hash] = best;
		return best;
	}
}

int Game::evaluateBoard() {
	auto check_line = [&](int start_x, int start_y, int step_x, int step_y) -> int {
		Player first = board[start_x][start_y];

		if (first == Player::NONE) {
			return 0;
		}

		for (int i = 1; i < board_size; ++i) {
			if (board[start_x + i * step_x][start_y + i * step_y] != first) {
				return 0;
			}
		}

		return (first == Player::PLAYER_O) ? 10 : -10;
	};

	// Check rows and columns
	for (int i = 0; i < board_size; ++i) {
		int row_result = check_line(i, 0, 0, 1);

		if (row_result != 0) {
			return row_result;
		}

		int col_result = check_line(0, i, 1, 0);

		if (col_result != 0) {
			return col_result;
		}
	}

	// Check main diagonal
	int main_diag_result = check_line(0, 0, 1, 1);

	if (main_diag_result != 0) {
		return main_diag_result;
	}
	// Check anti-diagonal
	int anti_diag_result = check_line(0, board_size - 1, 1, -1);

	if (anti_diag_result != 0) {
		return anti_diag_result;
	}

	return 0;
}


bool Game::isMovesLeft() {
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			if (board[i][j] == Player::NONE) {
				return true;
			}
		}
	}

	return false;
}

Player Game::getWinner() {
	int score = evaluateBoard();

	if (score == 10) {
		return Player::PLAYER_O;
	}

	if (score == -10) {
		return Player::PLAYER_X;
	}

	return Player::NONE;
}

void Game::reset() {
	board.assign(board_size, std::vector<Player>(board_size, Player::NONE));
}

const std::vector<std::vector<Player>> &Game::getBoard() const {
	return board;
}

std::string Game::hashBoard() {
	std::string hash;

	for (int i = 0; i < board_size; ++i) {
		for (int j = 0; j < board_size; ++j) {
			hash += std::to_string(static_cast<int>(board[i][j]));
		}
	}

	return hash;
}
