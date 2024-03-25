#ifndef GAME_H
#define GAME_H

typedef struct Game {
    int w, h, cell_size, gw, gh, goffset_x, goffset_y, generation;
    SDL_bool** grid;
} Game;

SDL_bool create_game(Game** game, int w, int h, int cell_size, int cell_probability);
SDL_bool free_game(Game* game);

SDL_bool set(Game* game, int i, int j, SDL_bool active);
SDL_bool get(Game* game, int i, int j);
SDL_bool update(Game* game);

#endif