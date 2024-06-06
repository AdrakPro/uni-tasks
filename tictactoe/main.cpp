#include <SDL2/SDL.h>
#include <unistd.h>
#include "game.h"


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool quit = false;
bool is_game_over = false;
int board_size = 3;
int cell_width = 100;
int window_size = board_size * cell_width;
Player current_player = Player::PLAYER_O;
Player starting_player;

void drawX(int x, int y) {
	SDL_RenderDrawLine(
			renderer, x * cell_width + 20, y * cell_width + 20, (x + 1) * cell_width - 20,
			(y + 1) * cell_width - 20
	);
	SDL_RenderDrawLine(
			renderer, (x + 1) * cell_width - 20, y * cell_width + 20, x * cell_width + 20,
			(y + 1) * cell_width - 20
	);
}

void drawCircle(int center_x, int center_y) {
	const int radius = cell_width / 2 - 10;
	const int diameter = radius * 2;

	int x = radius - 1;
	int y = 0;
	int tx = 1;
	int ty = 1;
	int error = tx - diameter;

	while (x >= y) {
		SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
		SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
		SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
		SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
		SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
		SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
		SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
		SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

		if (error <= 0) {
			++y;
			error += ty;
			ty += 2;
		}

		if (error > 0) {
			--x;
			tx += 2;
			error += (tx - diameter);
		}
	}
}

void restartGame(Game &game) {
	game.reset();
	is_game_over = false;
	current_player = Player::PLAYER_O;
}

void initGui() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
			"Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size,
			window_size, SDL_WINDOW_SHOWN
	);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

void closeGui() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void drawBoard(Game &game) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	for (int i = 1; i < board_size; ++i) {
		SDL_RenderDrawLine(renderer, i * cell_width, 0, i * cell_width, window_size);
		SDL_RenderDrawLine(renderer, 0, i * cell_width, window_size, i * cell_width);
	}

	for (int y = 0; y < board_size; ++y) {
		for (int x = 0; x < board_size; ++x) {
			if (game.getBoard()[y][x] == Player::PLAYER_X) {
				drawX(x, y);
			} else if (game.getBoard()[y][x] == Player::PLAYER_O) {
				drawCircle(x * 100 + 50, y * 100 + 50);
			}
		}
	}

	SDL_RenderPresent(renderer);

	if (is_game_over) {
		sleep(3);
		restartGame(game);
	}
}

int main(int argc, char* args[]) {
	if (argc > 1) {
		board_size = std::stoi(args[1]);

		if (board_size < 3) {
			std::cout << "Board size must be at least 3. Setting field size to 3." << std::endl;
			return EXIT_FAILURE;
		}
		window_size = board_size * cell_width;
	}

	if (argc > 2) {
		std::string player = args[2];

		if (player == "x" || player == "X") {
			starting_player = Player::PLAYER_X;
		} else if (player == "o" || player == "O") {
			starting_player = Player::PLAYER_O;
		} else {
			std::cout << "Invalid player specified. Defaulting to Player X." << std::endl;
			return EXIT_FAILURE;
		}
	}


	initGui();
	Game game(board_size);

	while (!quit) {
		SDL_Event e;

		drawBoard(game);

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && !is_game_over) {
				int x = e.button.x / cell_width;
				int y = e.button.y / cell_width;

				if (game.makeMove(current_player, y, x)) {
					if (game.getWinner() != Player::NONE || !game.isMovesLeft()) {
						is_game_over = true;
					} else {
						current_player = (current_player == Player::PLAYER_X) ? Player::PLAYER_O
																																	: Player::PLAYER_X;
					}
				}
			}
		}

		if (current_player != starting_player && !is_game_over) {
			game.makeAiMove(current_player);

			if (game.getWinner() != Player::NONE || !game.isMovesLeft()) {
				is_game_over = true;
			} else {
				current_player = starting_player;
			}
		}

	}

	closeGui();

	return EXIT_SUCCESS;
}
