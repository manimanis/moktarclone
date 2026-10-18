/*
 * Engine initialization for Moktar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "settings.h"
#include "tituserror.h"
#include "backbuffer.h"
#include "fonts.h"
#include "original.h"
#include "audio.h"

int init_game() {
    int retval;

    retval = readconfig(OPENTITUS_CONFIG_FILE);
    if (retval < 0) {
        return retval;
    }

#ifdef AUDIO_ENABLED
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return TITUS_ERROR_SDL_ERROR;
    }
#else
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return TITUS_ERROR_SDL_ERROR;
    }
#endif

// Declarations for SDL2 and sdl12-compat integration
extern void* SDL12COMPAT_GetWindow(void);
extern void SDL_SetWindowSize(void *window, int w, int h);
extern void SDL_SetWindowPosition(void *window, int x, int y);
extern void SDL_SetWindowResizable(void *window, int resizable);
extern void SDL_SetWindowMinimumSize(void *window, int min_w, int min_h);

    // Native internal resolution of Moktar is 320x200 (Mode 13h)
    reswidth = 320;
    resheight = 200;

    int win_w = (window_width > 0) ? window_width : 960;
    int win_h = (window_height > 0) ? window_height : 600;

    switch (videomode) {
    case 0: // window mode
        screen = SDL_SetVideoMode(320, 200, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
        SDL_WM_SetCaption("Les Aventures de Moktar - Vol 1: La Zoubida (Linux C++)", 0);
        break;
    case 1: // fullscreen
        SDL_ShowCursor(SDL_DISABLE);
        screen = SDL_SetVideoMode(320, 200, 32, SDL_DOUBLEBUF | SDL_FULLSCREEN);
        break;
    }

    if (screen == NULL) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return TITUS_ERROR_SDL_ERROR;
    }

    void *win = SDL12COMPAT_GetWindow();
    if (win && videomode == 0) {
        SDL_SetWindowResizable(win, 1);
        SDL_SetWindowSize(win, win_w, win_h);
        SDL_SetWindowPosition(win, 0x2FFF0000 /* SDL_WINDOWPOS_CENTERED */, 0x2FFF0000);
        SDL_SetWindowMinimumSize(win, 320, 200);
    }

    SDL_EnableUNICODE(1);

#ifdef AUDIO_ENABLED
    initaudio();
#endif

    initoriginal();
    initcodes();
    initleveltitles();
    loadfonts();

    return 0;
}

void checkerror(void) {
    printf("%s\n", lasterror);
}
