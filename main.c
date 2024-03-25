#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "game.h"

SDL_Rect get_cell_rect(Game* game, int i, int j) {
    int x = game->goffset_x + j * game->cell_size;
    int y = game->goffset_y + i * game->cell_size;
    SDL_Rect r = {x, y, game->cell_size, game->cell_size};
    return r;
}

SDL_bool draw_cell(SDL_Renderer* renderer, SDL_Rect r, SDL_bool alive) {
    SDL_Color color = BLACK_COLOR;

    if(!alive)
        color = WHITE_COLOR;

    if(!draw_rect(renderer, r, color))
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool draw_grid(SDL_Renderer* renderer, Game* game) {
    for(int i = 0; i < game->gh; i++) {
        for(int j = 0; j < game->gw; j++) {
            if(!draw_cell(renderer, get_cell_rect(game, i, j), get(game, j, i)))
                return SDL_FALSE;
        }
    }

    return SDL_TRUE;
}

int get_int_size(int a) {
    int res = a;
    int size = 0;

    while(res > 0) {
        res = (int) (res / 10);
        size++;
    }

    return size;
}

SDL_bool draw_generation(SDL_Renderer* renderer, Game* game) {
    int gen_v_size = get_int_size(game->generation);
    int gen_size = 5 + gen_v_size + 1;
    char text[gen_size];
    SDL_Rect text_rect = {0, 0, gen_size * 10, 20};

    sprintf(text, "Gen: %d", game->generation);

    if(draw_text(renderer, text, text_rect, TEXT_COLOR))
        return SDL_FALSE;
    
    return SDL_TRUE;
}

SDL_bool interactive_draw(SDL_Renderer* renderer, Game* game, SDL_Point mouse) {
    int j = (int) floor(mouse.x / game->cell_size);
    int i = (int) floor(mouse.y / game->cell_size);
    SDL_bool is_alive = SDL_FALSE;

    if(is_left_mouse_pressed())
        is_alive = SDL_TRUE;
    
    set(game, j, i, is_alive);
    return SDL_TRUE;
}

SDL_bool draw_window(Game* game, SDL_Renderer* renderer) {
    if(!fill(renderer, WHITE_COLOR))
        return SDL_FALSE;
        
    if(!draw_grid(renderer, game))
        return SDL_FALSE;
    
    if(do_show_generation() && !draw_generation(renderer, game))
        return SDL_FALSE;

    refresh(renderer);

    return SDL_TRUE;
}

void clear(Game* game) {
    for(int i = 0; i < game->gh; i++) {
        for(int j = 0; j < game->gw; j++) {
            set(game, j, i, SDL_FALSE);
        }
    }
}

SDL_bool run(Game* game, SDL_Renderer* renderer, int framerate, SDL_bool interactive) {
    SDL_Rect click_window = {0, 0, game->gw * game->cell_size, game->gh * game->cell_size};

    if(!draw_window(game, renderer))
        return SDL_FALSE;

    tick(1);

    while(1) {
        handle_events();

        if(do_quit()) {
            break;
        }

        if(do_clear())
            clear(game);

        if(interactive && is_mouse_pressed()) {
            SDL_Point mouse = mouse_pos();

            if(point_in_rect(&mouse, &click_window) && !interactive_draw(renderer, game, mouse))
                return SDL_FALSE;
        } else if(!is_paused()) {
            tick(framerate);
            if(!update(game))
                return SDL_FALSE;
        }

        if(!draw_window(game, renderer))
            return SDL_FALSE;
    }

    return SDL_TRUE;
}

void print_in_game_help() {
    printf("In game controls:\n");
    printf("\tP\t\t\t\tPause/resume simulation\n");
    printf("\tG\t\t\t\tShow/hide generation count\n");
    printf("\tC\t\t\t\tClear simulation\n");
    printf("\tQ\t\t\t\tQuit\n");
    printf("\tMouse left button\t\tAdd cell\n");
    printf("\tMouse right button\t\tRemove cell\n");
}

void print_help(char* filename) {
    printf("Usage: %s <options>\n\n", filename);
    
    printf("Options:\n");
    printf("Help\n");
    printf("\t-h, --help\t\t\tDisplay this help message\n");
    printf("Simulation\n");
    printf("\t-p, --probability <value>\tProbability for a cell to be alive when filling board at startup in %% (default: 50%%)\n");
    printf("\t-r, --framerate <value>\t\tSimulation refresh rate in times per seconds (default: 25)\n");
    printf("\t-i, --no-interactive\t\tDisable interactible board\n");
    printf("\t-g, --show-generation\t\tShow generation count\n");
    printf("Window\n");
    printf("\t-c, --cell-size <value>\t\tCell size in pixels (default: 10 pixels)\n");
    printf("\t-f, --fullscreen\t\tFullscreen window\n");
    printf("\t--window-size=<width>x<height>\tWindow size in pixels (default: 500x500)\n");
    printf("\n");

    print_in_game_help();
}

int starts_with(char* str, char* prefix) {
    return strncmp(str, prefix, strlen(prefix));
}

SDL_bool get_int_arg(char* arg, char* strv, int* v) {
    char* end;
    int val = strtol(strv, &end, 10);

    if((val == 0 && (errno != 0 || strv == end)) || strlen(end) > 0)
        return SDL_FALSE;

    *v = val;
    return SDL_TRUE;
}

int handle_args(int argc, char** argv, 
    SDL_bool* fullscreen,
    SDL_bool* interactive,
    SDL_bool* show_generation,
    int* window_w,
    int* window_h,
    int* framerate,
    int* cell_size,
    int* cell_probability) {

    for(int i = 1; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_help(argv[0]);
            return 1;
        }

        if(strcmp(arg, "-f") == 0 || strcmp(arg, "--fullscreen") == 0) {
            *fullscreen = SDL_TRUE;
            continue;
        }

        if(strcmp(arg, "-i") == 0 || strcmp(arg, "--no-interactive") == 0) {
            *interactive = SDL_FALSE;
            continue;
        }

        if(strcmp(arg, "-g") == 0 || strcmp(arg, "--show-generation") == 0) {
            *show_generation = SDL_TRUE;
            continue;
        }

        if(strcmp(arg, "-r") == 0 || strcmp(arg, "--framerate") == 0) {
            if(i + 1 >= argc) {
                fprintf(stderr, "No value for '%s'\n", arg);
                return -1;
            }

            char* strv = argv[i + 1];
            int v;
            
            if(!get_int_arg(arg, strv, &v)) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }
            
            if(v <= 0) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }

            *framerate = v;
            i++;
            continue;
        }

        if(strcmp(arg, "-p") == 0 || strcmp(arg, "--probability") == 0) {
            if(i + 1 >= argc) {
                fprintf(stderr, "No value for '%s'\n", arg);
                return -1;
            }

            char* strv = argv[i + 1];
            int v;
            
            if(!get_int_arg(arg, strv, &v)) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }
            
            if(v < 0 || v > 100) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }

            *cell_probability = v;
            i++;
            continue;
        }

        if(strcmp(arg, "-c") == 0 || strcmp(arg, "--cell-size") == 0) {
            if(i + 1 >= argc) {
                fprintf(stderr, "No value for '%s'\n", arg);
                return -1;
            }

            char* strv = argv[i + 1];
            int v;
            
            if(!get_int_arg(arg, strv, &v)) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }
            
            if(v <= 0) {
                fprintf(stderr, "Invalid value for '%s': %s\n", arg, strv);
                return -1;
            }

            *cell_size = v;
            i++;
            continue;
        }

        if(starts_with(arg, "--window-size") == 0) {
            if(strlen(arg) < 14) {
                fprintf(stderr, "No value for '--window-size'\n");
                return -1;
            }

            if(arg[13] != '=') {
                fprintf(stderr, "Unknown argument: %s\n", arg);
                return -1;
            }

            int w, h;
            int xpos = -1;
            char* strv = &arg[14];

            for(int i = 0; i < strlen(strv); i++) {
                char c = strv[i];

                if(c >= '0' && c <= '9')
                    continue;

                if(xpos < 0) {
                    if(c != 'x') {
                        fprintf(stderr, "Invalid value for '--window-size': %s\n", strv);
                        return -1;
                    }

                    xpos = i;
                } else {
                    fprintf(stderr, "Invalid value for '--window-size': %s\n", strv);
                    return -1;
                }
            }

            if(xpos == -1) {
                fprintf(stderr, "Invalid value for '--window-size': %s\n", strv);
                return -1;
            }
            
            strv[xpos] = '\0';
            char* strvw = strv;
            char* strvh = &strv[xpos + 1];

            if(!get_int_arg(arg, strvw, &w)) {
                fprintf(stderr, "Invalid value for '--window-size': %sx%s\n", strvw, strvh);
                return -1;
            }

            if(!get_int_arg(arg, strvh, &h)) {
                fprintf(stderr, "Invalid value for '--window-size': %sx%s\n", strvw, strvh);
                return -1;
            }
            
            if(w <= 0 || h <= 0) {
                fprintf(stderr, "Invalid value for '--window-size': %sx%s\n", strvw, strvh);
                return -1;
            }

            *window_w = w;
            *window_h = h;
            continue;
        }

        fprintf(stderr, "Unknown argument: %s\n", arg);
        return -1;
    }

    return 0;
}

int main(int argc, char** argv) {
    Game* game;
    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_bool fullscreen = SDL_FALSE;
    SDL_bool interactive = SDL_TRUE;
    SDL_bool show_generation = SDL_FALSE;
    int window_w = 500;
    int window_h = 500;
    int framerate = 25;
    int cell_size = 10;
    int cell_probability = 50;

    int args_res = handle_args(argc, argv, &fullscreen, &interactive, &show_generation, 
        &window_w, &window_h, &framerate, &cell_size, &cell_probability);

    if(args_res > 0)
        return EXIT_SUCCESS;
    else if(args_res < 0) {
        printf("Use: %s --help, for help\n", argv[0]);
        return EXIT_FAILURE;
    }

    if(!init_display())
        return EXIT_FAILURE;

    if(!create_window(&window, window_w, window_h)) {
        destroy_display();
        return EXIT_FAILURE;
    }

    show_window(window);

    if(fullscreen && !set_window_fullscreen(window)) {
        destroy_window(window);
        destroy_display();
        return EXIT_FAILURE;
    }

    get_window_size(window, &window_w, &window_h);
    set_show_generation(show_generation);

    if(!create_game(&game, window_w, window_h, cell_size, cell_probability)) {
        destroy_window(window);
        destroy_display();
        return EXIT_FAILURE;
    }

    if(!create_renderer(window, &renderer)) {
        free_game(game);
        destroy_window(window);
        destroy_display();
        return EXIT_FAILURE;
    }

    printf("Starting simulation\n");
    printf("For program help, use: %s --help\n", argv[0]);
    print_in_game_help();

    run(game, renderer, framerate, interactive);

    free_game(game);
    destroy_renderer(renderer);
    destroy_window(window);
    destroy_display();
    return EXIT_SUCCESS;
}