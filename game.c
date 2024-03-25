#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "game.h"

SDL_bool create_grid(SDL_bool*** grid, int w, int h) {
    SDL_bool** g = (SDL_bool**) malloc(sizeof(SDL_bool*) * h);

    if(g == NULL)
        return SDL_FALSE;

    for(int i = 0; i < h; i++) {
        g[i] = (SDL_bool*) malloc(sizeof(SDL_bool) * w);

        if(g[i] == NULL)
            return SDL_FALSE;

        for(int j = 0; j < w; j++)
            g[i][j] = SDL_FALSE;
    }

    *grid = g;
    return SDL_TRUE;
}

SDL_bool free_grid(SDL_bool** grid, int gh) {
    for(int i = 0; i < gh; i++) {
        free(grid[i]);
    }

    free(grid);
}

SDL_bool create_game(Game** game, int w, int h, int cell_size, int cell_probability) {
    *game = (Game*) malloc(sizeof(Game));

    if(*game == NULL) {
        fprintf(stderr, "Memory allocation error when creating game\n");
        return SDL_FALSE;
    }

    int gh = (int) floor(h / cell_size);
    int gw = (int) floor(w / cell_size);

    SDL_bool** grid;
    if(!create_grid(&grid, gw, gh)) {
        fprintf(stderr, "Memory allocation error when creating game\n");
        return SDL_FALSE;
    }

    for(int i = 0; i < gh; i++) {
        for(int j = 0; j < gw; j++) {
            if(rand() % 100 < cell_probability)
                grid[i][j] = SDL_TRUE;
        }
    }

    (*game)->w = w;
    (*game)->h = h;
    (*game)->cell_size = cell_size;
    (*game)->gh = gh;
    (*game)->gw = gw;
    (*game)->goffset_x = floor((w - gw * cell_size) / 2);
    (*game)->goffset_y = floor((h - gh * cell_size) / 2);
    (*game)->grid = grid;
    (*game)->generation = 0;
    return SDL_TRUE;
}

SDL_bool free_game(Game* game) {
    free_grid(game->grid, game->gh);
    free(game);
}

SDL_bool set(Game* game, int x, int y, SDL_bool active) {
    game->grid[y][x] = active;
}

SDL_bool get(Game* game, int x, int y) {
    return game->grid[y][x];
}

SDL_bool coord_valid(int gw, int gh, int x, int y) {
    if(x >= 0 && x < gw && y >= 0 && y < gh) {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

int count_neighbours(SDL_bool** grid, int gw, int gh, int i, int j) {
    int count = 0;

    for(int y = i - 1; y <= i + 1; y++) {
        for(int x = j - 1; x <= j + 1; x++) {
            if(y == i && x == j)
                continue;

            if(coord_valid(gw, gh, x, y) && grid[y][x])
                count++;
        }
    }
    
    return count;
}

SDL_bool update(Game* game) {
    SDL_bool** grid = game->grid;
    SDL_bool** copy;

    if(!create_grid(&copy, game->gw, game->gh)) {
        fprintf(stderr, "Memory allocation error when creating game\n");
        return SDL_FALSE;
    }

    for(int i = 0; i < game->gh; i++) {
        for(int j = 0; j < game->gw; j++) {
            int count = count_neighbours(grid, game->gw, game->gh, i, j);

            if(count == 2) {
                copy[i][j] = grid[i][j];
            } else if(count == 3) {
                copy[i][j] = SDL_TRUE;
            } else {
                // Ignore, already false
                //copy[i][j] = SDL_FALSE;
            }
        }
    }

    free_grid(game->grid, game->gh);
    game->grid = copy;
    game->generation++;
    return SDL_TRUE;
}