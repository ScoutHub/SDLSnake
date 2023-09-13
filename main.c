#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

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

void destroy_game(SDL_Renderer* r, SDL_Window* w) {
	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);
	SDL_Quit();
}

void draw_background(SDL_Renderer* r) {
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255 );
    SDL_RenderClear(r);
}

void draw_rectangle(SDL_Renderer* r, Snake* s){
	for(int i = 0; i < s->len; i++) {
		SDL_SetRenderDrawColor( r, 0, 0, 255, 255 );
		SDL_RenderFillRect(r, s->snakeArray[i]);
		SDL_RenderPresent(r);
	}
}

void check_dir(int* x, int* y, SDL_Rect* r){
	if(*(x) == 1) r->x += 20;
	else if(*(x) == -1) r->x -= 20;
	else if(*(y) == 1) r->y += 20;
	else if(*(y) == -1) r->y -= 20;
}

bool check_collision(SDL_Rect* r) {
	if(r->x == (SCREEN_WIDTH - BLOC_SIZE) || r->x < (0 + BLOC_SIZE)) return true;
	else if(r->y == (SCREEN_HEIGHT - BLOC_SIZE) || r->y < (0 + BLOC_SIZE)) return true;
	return false;
}

void set_default_pos(Snake* s) {
	s->snakeArray[0].x = DEFAULT_POSITION_X;
	s->snakeArray[0].y = DEFAULT_POSITION_Y;
	s->snakeArray[0].w = BLOC_SIZE;
	s->snakeArray[0].h = BLOC_SIZE;
}

int main(void) {
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
	draw_background(renderer);

	int x_dir = 1, y_dir = 0;
	SDL_Event event;
	Snake snake;
	snake.len = 1;
	snake.snakeArray = (SDL_Rect*) malloc(snake.len * sizeof(SDL_Rect));
	if(snake.snakeArray == NULL) printf("Cannot malloc ptr.\n");

	set_default_pos(&snake);
	for (int i = 0; i < snake.len; i++) {
		printf("index of element : %d\n", i);
		printf("x position : %d\n", snake.snakeArray[i].x);
		printf("y position : %d\n", snake.snakeArray[i].y);
	}
	/*SDL_Rect r;

	r.x = DEFAULT_POSITION_X;
	r.y = DEFAULT_POSITION_Y;
	r.w = BLOC_SIZE;
	r.h = BLOC_SIZE;
	bool quit = false;
	draw_rectangle(renderer, &r);
	while(!quit) {
		draw_background(renderer);
		check_dir(&x_dir, &y_dir, &r);
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_UP:
							if(y_dir == 1) break;
							y_dir = -1;
							x_dir = 0;
							break;
						case SDLK_DOWN:
							if(y_dir == -1) break;
							y_dir = 1;
							x_dir = 0;
							break;
						case SDLK_LEFT:
							if(x_dir == 1) break;
							x_dir = -1;
							y_dir = 0;
							break;
						case SDLK_RIGHT:
							if(x_dir == -1) break;
							x_dir = 1;
							y_dir = 0;
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
		draw_rectangle(renderer, &r);
		quit = check_collision(&r);
		SDL_Delay(GAME_SPEED);
	}
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Game info", "You losed.", window);
	*/
	free(snake.snakeArray);
	destroy_game(renderer, window);
	return 0;
}
