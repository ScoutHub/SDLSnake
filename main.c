#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define DEFAULT_POSITION_X 200
#define DEFAULT_POSITION_Y 200
#define BLOC_SIZE 20
#define GAME_SPEED 50

typedef struct {
	int len;
	SDL_Rect* snakeArray; 
} Snake;

SDL_Rect generate_apple() {
	SDL_Rect apple;
	int rand_x = rand() % SCREEN_WIDTH - 20, rand_y = rand() % SCREEN_HEIGHT - 20;
	rand_x = (rand_x / 20) * 20;
	rand_y = (rand_y / 20) * 20;
	apple.x = rand_x;
	apple.y = rand_y;
	apple.w = BLOC_SIZE;
	apple.h = BLOC_SIZE;
	return apple;
}

void destroy_game(SDL_Renderer* r, SDL_Window* w) {
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
}

void draw_background(SDL_Renderer* r) {
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255 );
    SDL_RenderClear(r);
}

void draw(SDL_Renderer* r, Snake* s, SDL_Rect* apple){
	for(int i = 0; i < s->len; i++) {
		SDL_RenderDrawRect(r, &(s->snakeArray[i]));
		SDL_SetRenderDrawColor(r, 0, 0, 255, 255 );
		SDL_RenderFillRect(r, &(s->snakeArray[i]));
	}
	SDL_SetRenderDrawColor(r, 0, 255, 0, 255 );
	SDL_RenderFillRect(r, apple);
	SDL_RenderPresent(r);
}

void check_dir(int* x, int* y, Snake* s){
	int i = 0, last_x = 0, last_y = 0;
	while(i < s->len) {
		if(*(x) == 1){
			if(i == 0){
				last_x = s->snakeArray[i].x;
				last_y = s->snakeArray[i].y;
				s->snakeArray[i].x += 20;
			} else {
				int temp_x = s->snakeArray[i].x, temp_y = s->snakeArray[i].y;
				s->snakeArray[i].x = last_x;
				s->snakeArray[i].y = last_y;
				last_x = temp_x;
				last_y = temp_y;
			} 
		} else if(*(x) == -1){
			if(i == 0){
				last_x = s->snakeArray[i].x;
				last_y = s->snakeArray[i].y;
				s->snakeArray[i].x -= 20;
			} else {
				int temp_x = s->snakeArray[i].x, temp_y = s->snakeArray[i].y;
				s->snakeArray[i].x = last_x;
				s->snakeArray[i].y = last_y;
				last_x = temp_x;
				last_y = temp_y;
			}
		} else if(*(y) == 1){
			if(i == 0){
				last_x = s->snakeArray[i].x;
				last_y = s->snakeArray[i].y;
				s->snakeArray[0].y += 20;
			} else {
				int temp_x = s->snakeArray[i].x, temp_y = s->snakeArray[i].y;
				s->snakeArray[i].x = last_x;
				s->snakeArray[i].y = last_y;
				last_x = temp_x;
				last_y = temp_y;
			}
		} else if(*(y) == -1){
			if(i == 0){
				last_x = s->snakeArray[i].x;
				last_y = s->snakeArray[i].y;
				s->snakeArray[0].y -= 20;
			} else {
				int temp_x = s->snakeArray[i].x, temp_y = s->snakeArray[i].y;
				s->snakeArray[i].x = last_x;
				s->snakeArray[i].y = last_y;
				last_x = temp_x;
				last_y = temp_y;
			}
		}
		s->snakeArray[i].w = BLOC_SIZE;
		s->snakeArray[i].h = BLOC_SIZE;
		i++;
	}
}

bool check_collision(Snake* s) {
	if(s->snakeArray[0].x == (SCREEN_WIDTH) || s->snakeArray[0].x < (0)) return true;
	else if(s->snakeArray[0].y == (SCREEN_HEIGHT) || s->snakeArray[0].y < (0)) return true;
	return false;
}

bool eat(SDL_Renderer* r ,Snake* s, SDL_Rect* apple) {
	return (s->snakeArray[0].x == apple->x && s->snakeArray[0].y == apple->y);
}

void set_default_pos(Snake* s) {
	s->snakeArray[0].x = DEFAULT_POSITION_X;
	s->snakeArray[0].y = DEFAULT_POSITION_Y;
	s->snakeArray[0].w = BLOC_SIZE;
	s->snakeArray[0].h = BLOC_SIZE;
}

int update_snake(Snake* s){
	++s->len;
	s->snakeArray = (SDL_Rect*) realloc(s->snakeArray, s->len * sizeof(SDL_Rect));
	if(s->snakeArray == NULL){
		printf("ERROR: Cannot reallocate Snake");
		return -1;
	}
	return 0;
}

int main(void) {
	srand(time(NULL));
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf("ERROR: SDL wasn't able to initialize.");
		return -1;
	}
	SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	if(!window) {
		printf("ERROR: Window couldnt' created.");
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(!renderer) {
		printf("ERROR: Renderer wasn't able to render");
		return -1;
	}
	//draw_background(renderer);

	int x_dir = 1, y_dir = 0, temp_x = 0, temp_y = 0;
	SDL_Event event;
	SDL_Rect apple = generate_apple();
	Snake snake;
	snake.len = 1;
	snake.snakeArray = (SDL_Rect*) malloc(snake.len * sizeof(SDL_Rect));
	if(snake.snakeArray == NULL) {
		printf("ERROR: Cannot malloc ptr.\n");
		destroy_game(renderer, window);
		return -1;
	}
	set_default_pos(&snake);
	bool quit = false;
	bool paused = false;
	while(!quit) {
		draw_background(renderer);
		check_dir(&x_dir, &y_dir, &snake);
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_UP:
							if(!paused) {
								if(y_dir == 1) break;
								y_dir = -1;
								x_dir = 0;
							}
							break;
						case SDLK_DOWN:
							if(!paused) {
								if(y_dir == -1) break;
								y_dir = 1;
								x_dir = 0;
							}
							break;
						case SDLK_LEFT:
							if(!paused) {
								if(x_dir == 1) break;
								x_dir = -1;
								y_dir = 0;
							}
							break;
						case SDLK_RIGHT:
							if(!paused) {
								if(x_dir == -1) break;
								x_dir = 1;
								y_dir = 0;
							}
							break;
						case SDLK_SPACE:
							if(!paused) {
								temp_x = x_dir;
								temp_y = y_dir;
								x_dir = 0;
								y_dir = 0;
								paused = true;
							} else {
								if(temp_x != 0) x_dir = temp_x;
								else if(temp_y != 0) y_dir = temp_y;
								temp_x = 0;
								temp_y = 0;
								paused = false;
							}
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					quit = true;
					break;
				default:
					break;
			}
		}
		bool is_eat = eat(renderer, &snake, &apple);
		if(is_eat) {
			apple = generate_apple();
			if(update_snake(&snake)){
				break;
			}
		};
		draw(renderer, &snake, &apple);
		quit = check_collision(&snake);
		SDL_Delay(GAME_SPEED);
	}
	char score[30];
	sprintf(score, "Score %d", snake.len - 1);
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Game info", score, window);
	free(snake.snakeArray);
	destroy_game(renderer, window);
	return 0;
}
