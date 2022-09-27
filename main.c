#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_mixer.h>

#include <enet/enet.h>

#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT    720

#define RECT_WIDTH_SIZE    5 
#define RECT_HEIGHT_SIZE   5 
#define RECT_SPEED       400

#define FREQUENCY      44100
#define CHUNKSIZE      2048
#define LOOPS          5

int main(int argc, char *argv[])
{
    Uint32 sdl_flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (SDL_Init(sdl_flags) != 0) {
        fprintf(stderr, "Initializing SDL error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    Uint32 img_flags = IMG_INIT_JPG;
    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        fprintf(stderr, "Initializing IMG error: %s\n", IMG_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window =     NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *surface =   NULL;
    SDL_Texture *texture =   NULL;
    Mix_Music *music =       NULL;
    SDL_Event event;
    SDL_Rect rect;

    window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (window == NULL) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Delay(100);

    Mix_OpenAudio(
        FREQUENCY,
        MIX_DEFAULT_FORMAT,
        MIX_DEFAULT_CHANNELS,
        CHUNKSIZE
    );

    music = Mix_LoadMUS("music/anime.mp3");
    if (music == NULL) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    Mix_PlayMusic(music, LOOPS);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    surface = IMG_Load("./image/anime.jpg");
    if (surface == NULL) {
        fprintf(stderr, "Load image error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

    rect.w /= RECT_WIDTH_SIZE;
    rect.h /= RECT_HEIGHT_SIZE;

    rect.x = (WINDOW_WIDTH - rect.w) / 2;
    rect.y = (WINDOW_HEIGHT - rect.h) / 2;

    bool is_close = false;
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
                            rect.y -= RECT_SPEED / 30;
                            break;

                        case SDL_SCANCODE_A:
                        case SDL_SCANCODE_LEFT:
                            rect.x -= RECT_SPEED / 30;
                            break;

                        case SDL_SCANCODE_S:
                        case SDL_SCANCODE_DOWN:
                            rect.y += RECT_SPEED / 30;
                            break;

                        case SDL_SCANCODE_D:
                        case SDL_SCANCODE_RIGHT:
                            rect.x += RECT_SPEED / 30;
                            break;

                        default:
                            break;
                    }
                    break;
            }
        }

        if (rect.x + rect.w > WINDOW_WIDTH) {
            rect.x = WINDOW_WIDTH - rect.w;
        }

        if (rect.x < 0) {
            rect.x = 0;
        }

        if (rect.y + rect.h > WINDOW_HEIGHT) {
            rect.y = WINDOW_HEIGHT - rect.h;
        }

        if (rect.y < 0) {
            rect.y = 0;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        SDL_RenderPresent(renderer);

        Uint32 middle = (WINDOW_WIDTH + WINDOW_HEIGHT) / 2;
        SDL_Delay(middle / 60);
    }

    SDL_DestroyTexture(texture);

    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
