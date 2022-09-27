#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "Initializing SDL error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    Uint32 img_flags = IMG_INIT_JPG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        fprintf(stderr, "Initializing IMG error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Event event;
    SDL_Rect rect;

    int speed = 300;
    bool is_close = false;

    window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000,
        1000,
        SDL_WINDOW_OPENGL
    );
    if (window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    surface = IMG_Load("./image/anime.jpg");
    if (surface == NULL) {
        fprintf(stderr, "Load image error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    rect.w /= 6;
    rect.h /= 6;

    rect.x = (1000 - rect.w) / 2;
    rect.y = (1000 - rect.h) / 2;

    while (!is_close) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    is_close = true;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                        case SDL_SCANCODE_UP:
                            rect.y -= speed / 30;
                            break;

                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            rect.x -= speed / 30;
                            break;

                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            rect.y += speed / 30;
                            break;

                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            rect.x += speed / 30;
                            break;

                        default:
                            break;
                    }
                    break;
            }
        }

        if (rect.x + rect.w > 1000) {
            rect.x = 1000 - rect.w;
        }

        if (rect.x < 0) {
            rect.x = 0;
        }

        if (rect.y + rect.h > 1000) {
            rect.y = 1000 - rect.h;
        }

        if (rect.y < 0) {
            rect.y = 0;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
