#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "display.h"

const SDL_Color BLACK_COLOR = {0, 0, 0, 255};
const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color TEXT_COLOR = {255, 90, 0, 255};

typedef struct Events {
    SDL_bool quit;
    SDL_bool pause;
    SDL_bool clear;
    SDL_bool generation;
    SDL_bool left_mouse;
    SDL_bool right_mouse;
} Events;

Events EVENTS = {
    .quit = SDL_FALSE,
    .pause = SDL_FALSE,
    .clear = SDL_FALSE,
    .generation = SDL_FALSE,
    .left_mouse = SDL_FALSE,
    .right_mouse = SDL_FALSE
};

TTF_Font* FONT;
long NEXT_FRAME_MILLIS = 0;

SDL_bool init_display() {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "Unable to initiate display: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    if(TTF_Init() != 0) {
        fprintf(stderr, "Unable to initiate display: %s\n", TTF_GetError());
        return SDL_FALSE;
    }

    FONT = TTF_OpenFont("fonts/Roboto-Regular.ttf", 25);
    if(FONT == NULL) {
        fprintf(stderr, "Unable to initiate display: %s\n", TTF_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

void destroy_display() {
    TTF_CloseFont(FONT);	
    TTF_Quit();
    SDL_Quit();
}

SDL_bool create_window(SDL_Window** window, int w, int h) {
    *window = SDL_CreateWindow("The Game Of Life",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_HIDDEN);

    if(*window == NULL) {
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

void get_window_size(SDL_Window* window, int* w, int* h) {
    SDL_GetWindowSize(window, w, h);
}

SDL_bool set_window_fullscreen(SDL_Window* window) {
    if(SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
        fprintf(stderr, "Unable to set window fullscreen: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

void show_window(SDL_Window* window) {
    SDL_ShowWindow(window);
    SDL_RaiseWindow(window);
}

void hide_window(SDL_Window* window) {
    SDL_HideWindow(window);
}

void destroy_window(SDL_Window* window) {
    SDL_DestroyWindow(window);
}

SDL_bool create_renderer(SDL_Window* window, SDL_Renderer** renderer) {
    *renderer = SDL_CreateRenderer(window, -1, 0);

    if(*renderer == NULL) {
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

void refresh(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
}

void destroy_renderer(SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
}

SDL_bool fill(SDL_Renderer* renderer, SDL_Color color) {
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0
        || SDL_RenderClear(renderer) != 0)
    {
        fprintf(stderr, "Unable to fill renderer: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool draw_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color) {
    if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0 
        || SDL_RenderFillRect(renderer, &rect) != 0) {
        fprintf(stderr, "Unable to draw rect: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool draw_text(SDL_Renderer* renderer, char* text, SDL_Rect r, SDL_Color color) {
    SDL_Surface * surface = TTF_RenderText_Solid(FONT, text, color);
    if(surface == NULL) {
        fprintf(stderr, "Unable to draw text: %s\n", TTF_GetError());
        return SDL_FALSE;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL) {
        fprintf(stderr, "Unable to draw text: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    if(SDL_RenderCopy(renderer, texture, NULL, &r) != 0) {
        fprintf(stderr, "Unable to draw text: %s\n", SDL_GetError());
        return SDL_FALSE;
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void tick(int framerate) {
    SDL_Delay((int) (1000 / framerate));
}

SDL_bool point_in_rect(SDL_Point* point, SDL_Rect* rect) {
    return SDL_PointInRect(point, rect);
}

SDL_bool inverse_bool(SDL_bool b) {
    if(b)
        return SDL_FALSE;
    return SDL_TRUE;
}

void handle_events() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                EVENTS.quit = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT)
                    EVENTS.left_mouse = SDL_TRUE;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    EVENTS.right_mouse = SDL_TRUE;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT)
                    EVENTS.left_mouse = SDL_FALSE;
                else if(event.button.button == SDL_BUTTON_RIGHT)
                    EVENTS.right_mouse = SDL_FALSE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                    case 4: // SDL_SCANCODE_Q not working
                        EVENTS.quit = SDL_TRUE;
                        break;
                    case SDL_SCANCODE_G:
                        EVENTS.generation = inverse_bool(EVENTS.generation);
                        break;
                    case SDL_SCANCODE_P:
                        EVENTS.pause = inverse_bool(EVENTS.pause);
                        break;
                    case SDL_SCANCODE_C:
                        EVENTS.clear = SDL_TRUE;
                        break;
                }
                break;
        }
    }
}

SDL_bool do_quit() {
    return EVENTS.quit;
}

SDL_bool is_paused() {
    return EVENTS.pause;
}

SDL_bool do_show_generation() {
    return EVENTS.generation;
}

SDL_bool do_clear() {
    SDL_bool clear = EVENTS.clear;
    EVENTS.clear = SDL_FALSE;
    return clear;
}

SDL_bool set_show_generation(SDL_bool show) {
    EVENTS.generation = show;
}

SDL_bool is_left_mouse_pressed() {
    return EVENTS.left_mouse;
}

SDL_bool is_right_mouse_pressed() {
    return EVENTS.right_mouse;
}

SDL_bool is_mouse_pressed() {
    if(is_left_mouse_pressed())
        return SDL_TRUE;

    return is_right_mouse_pressed();
}

SDL_Point mouse_pos() {
    SDL_Point p = {0, 0};
    SDL_GetMouseState(&(p.x), &(p.y));
    return p;
}

