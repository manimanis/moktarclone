/*   
 * Copyright (C) 2008 - 2011 The OpenTitus team
 *
 * Authors:
 * Eirik Stople
 *
 * "Titus the Fox: To Marrakech and Back" (1992) and
 * "Lagaf': Les Aventures de Moktar - Vol 1: La Zoubida" (1991)
 * was developed by, and is probably copyrighted by Titus Software,
 * which, according to Wikipedia, stopped buisness in 2005.
 *
 * OpenTitus is not affiliated with Titus Software.
 *
 * OpenTitus is  free software; you can redistribute  it and/or modify
 * it under the  terms of the GNU General  Public License as published
 * by the Free  Software Foundation; either version 3  of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
 * MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
 * General Public License for more details.
 */

/* common.c
 * Common functions
 *
 * Global functions:
 * int subto0(uint8 *number): Decreases a number until it reaches 0
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "definitions.h"
#include "common.h"
#include "audio.h"

#include "globals.h"

// Declarations for SDL2 and sdl12-compat integration
extern void* SDL12COMPAT_GetWindow(void);
extern int SDL_SetWindowFullscreen(void *window, uint32_t flags);
extern uint32_t SDL_GetWindowFlags(void *window);
extern SDL_Surface *screen;

int subto0(uint8 *number) {
    //Decreases a value with 1 until it's zero
    if (*number > 0) {
        *number = *number - 1;
    }
    return 0;
}

int titus_sleep() {
	//refreshaudio();
    SDL_Delay(1);
    return 0;
}

void toggle_fullscreen(void) {
    void *win = SDL12COMPAT_GetWindow();
    if (win) {
        uint32_t flags = SDL_GetWindowFlags(win);
        if (flags & (0x00000001 /* SDL_WINDOW_FULLSCREEN */ | 0x00001000 /* SDL_WINDOW_FULLSCREEN_DESKTOP */)) {
            SDL_SetWindowFullscreen(win, 0);
            printf("[Vidéo] Mode fenêtré activé\n");
        } else {
            SDL_SetWindowFullscreen(win, 0x00001001 /* SDL_WINDOW_FULLSCREEN_DESKTOP */);
            printf("[Vidéo] Plein écran activé\n");
        }
    } else if (screen) {
        SDL_WM_ToggleFullScreen(screen);
    }
}

void toggle_music(void) {
    AUDIOMODE++;
    if (AUDIOMODE > 1) {
        AUDIOMODE = 0;
    }
    if (AUDIOMODE == 1) {
#ifdef AUDIO_ENABLED
        if (is_music_finished()) {
            SELECT_MUSIC(last_song);
        }
#endif
        startmusic();
        printf("[Audio] Musique AdLib FM réactivée\n");
    } else {
        all_vox_zero();
        printf("[Audio] Musique AdLib FM coupée\n");
    }
}

