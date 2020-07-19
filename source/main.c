/* Scrungus.nro -- based on Mini SDL Demo
 * featuring SDL2 + SDL2_mixer + SDL2_image
 * on Nintendo Switch using libnx
 *
 * Copyright 2018 carsten1ns
 *           2020 WinterMute
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <time.h>
#include <unistd.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <switch.h>

// some switch buttons

#define SCREEN_W 1280
#define SCREEN_H 720


int main(int argc, char** argv) {

    romfsInit();
    chdir("romfs:/");

    int exit_requested = 0;
    int trail = 0;
    int wait = 25;

    SDL_Texture *scrungus_tex =  NULL;
    SDL_Rect pos = { 0, 0, 0, 0 }, sdl_pos = { 0, 0, 0, 0 };
    Mix_Music *music = NULL;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);
    Mix_Init(MIX_INIT_MP3);
    IMG_Init(IMG_INIT_PNG);
    

    SDL_Window* window = SDL_CreateWindow("sdl2+mixer+image demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    // load logos from file
    SDL_Surface *scrungus = IMG_Load("data/scrungus.png");
    if (scrungus) {
        sdl_pos.w = scrungus->w;
        sdl_pos.h = scrungus->h;
        scrungus_tex = SDL_CreateTextureFromSurface(renderer, scrungus);
        SDL_FreeSurface(scrungus);
    }

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_AllocateChannels(5);
    Mix_OpenAudio(48000, AUDIO_S16, 2, 4096);

    // load music and sounds from files
    music = Mix_LoadMUS("data/background.mp3");
    
    if (music) {
        Mix_PlayMusic(music, -1);

        // set position and bounce on the walls
        
        // put logos on screen
        if (scrungus_tex)
            SDL_RenderCopy(renderer, scrungus_tex, NULL, &sdl_pos);

        SDL_RenderPresent(renderer);

        SDL_Delay(wait);
    }
    
    while (!exit_requested
        && appletMainLoop()
        ) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                exit_requested = 1;
        }
        }
    // stop sounds and free loaded data
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music);

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
    romfsExit();
    return 0;
}
