#ifndef DISPLAY_H
#define DISPLAY_H

extern const SDL_Color BLACK_COLOR;
extern const SDL_Color WHITE_COLOR;
extern const SDL_Color TEXT_COLOR;

SDL_bool init_display();
void destroy_display();

SDL_bool create_window(SDL_Window** window, int w, int h);
void get_window_size(SDL_Window* window, int* w, int* h);
SDL_bool set_window_fullscreen(SDL_Window* window);
void show_window(SDL_Window* window);
void hide_window(SDL_Window* window);
void destroy_window(SDL_Window* window);

SDL_bool create_renderer(SDL_Window* window, SDL_Renderer** renderer);
void refresh(SDL_Renderer* renderer);
void destroy_renderer(SDL_Renderer* renderer);

SDL_bool fill(SDL_Renderer* renderer, SDL_Color color);
SDL_bool draw_rect(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color);
SDL_bool draw_text(SDL_Renderer* renderer, char* text, SDL_Rect r, SDL_Color color);

void tick(int framerate);
SDL_bool point_in_rect(SDL_Point* point, SDL_Rect* rect);

void handle_events();
SDL_bool do_quit();
SDL_bool is_paused();
SDL_bool do_clear();
SDL_bool do_show_generation();
SDL_bool set_show_generation(SDL_bool show);
SDL_bool is_left_mouse_pressed();
SDL_bool is_right_mouse_pressed();
SDL_bool is_mouse_pressed();
SDL_Point mouse_pos();

#endif