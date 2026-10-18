/*
 * OPL Bridge for Moktar
 * Connects DOSBox DBOPL (Yamaha YM3812 / AdLib) synthesizer directly to SDL Audio
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include "dbopl.h"
#include "opl.h"

typedef void (*opl_callback_t)(void *data);

static Chip g_opl_chip;
static int g_opl_rate = 44100;
static opl_callback_t g_callback = NULL;
static void *g_callback_data = NULL;
static int g_samples_until_callback = 0;
static int g_delay_samples = 606; // ~13.75 ms at 44100 Hz (DOS timer ~72.7 Hz)
static int g_initialized = 0;

static Uint32 g_audio_thread_id = 0;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len) {
    g_audio_thread_id = SDL_ThreadID();
    int samples_needed = len / (2 * sizeof(Sint16)); // 16-bit stereo PCM frames
    Sint16 *out = (Sint16 *)stream;
    static Bit32s buf[2048];

    while (samples_needed > 0) {
        if (g_samples_until_callback <= 0) {
            if (g_callback) {
                g_callback(g_callback_data);
            }
            if (g_samples_until_callback <= 0) {
                g_samples_until_callback = g_delay_samples;
            }
        }

        int chunk = samples_needed;
        if (chunk > g_samples_until_callback) {
            chunk = g_samples_until_callback;
        }
        if (chunk > 2048) chunk = 2048;

        memset(buf, 0, chunk * sizeof(Bit32s));
        Chip__GenerateBlock2(&g_opl_chip, chunk, buf);

        for (int i = 0; i < chunk; i++) {
            Bit32s s = buf[i];
            if (s > 32767) s = 32767;
            else if (s < -32768) s = -32768;
            *out++ = (Sint16)s; // Left channel
            *out++ = (Sint16)s; // Right channel
        }

        samples_needed -= chunk;
        g_samples_until_callback -= chunk;
    }
}

int OPL_Init(unsigned int port_base) {
    DBOPL_InitTables();
    Chip__Setup(&g_opl_chip, g_opl_rate);

    SDL_AudioSpec wanted, obtained;
    memset(&wanted, 0, sizeof(wanted));
    wanted.freq = g_opl_rate;
    wanted.format = AUDIO_S16SYS;
    wanted.channels = 2;
    wanted.samples = 1024;
    wanted.callback = sdl_audio_callback;

    if (SDL_OpenAudio(&wanted, &obtained) < 0) {
        fprintf(stderr, "[OPL] SDL_OpenAudio failed: %s\n", SDL_GetError());
        return 0;
    }
    SDL_PauseAudio(0);
    g_initialized = 1;
    return 1;
}

void OPL_Shutdown(void) {
    if (g_initialized) {
        SDL_CloseAudio();
        g_initialized = 0;
    }
}

void OPL_SetSampleRate(unsigned int rate) {
    g_opl_rate = rate;
}

void OPL_WriteRegister(int reg, int value) {
    Chip__WriteReg(&g_opl_chip, reg, (Bit8u)value);
}

void OPL_SetCallback(unsigned int ms, opl_callback_t callback, void *data) {
    int in_cb = (SDL_ThreadID() == g_audio_thread_id);
    if (!in_cb) SDL_LockAudio();
    g_callback = callback;
    g_callback_data = data;
    if (ms == 0) {
        g_samples_until_callback = 0;
    } else {
        g_delay_samples = (int)((double)g_opl_rate * (double)ms / 1000.0);
        if (g_delay_samples < 1) g_delay_samples = 1;
        g_samples_until_callback = g_delay_samples;
    }
    if (!in_cb) SDL_UnlockAudio();
}

void OPL_ClearCallbacks(void) {
    int in_cb = (SDL_ThreadID() == g_audio_thread_id);
    if (!in_cb) SDL_LockAudio();
    g_callback = NULL;
    if (!in_cb) SDL_UnlockAudio();
}

void OPL_Lock(void) {
    if (g_audio_thread_id == 0 || SDL_ThreadID() != g_audio_thread_id) {
        SDL_LockAudio();
    }
}

void OPL_Unlock(void) {
    if (g_audio_thread_id == 0 || SDL_ThreadID() != g_audio_thread_id) {
        SDL_UnlockAudio();
    }
}

void OPL_Delay(unsigned int ms) {
    SDL_Delay(ms);
}

void OPL_SetPaused(int paused) {
    SDL_PauseAudio(paused);
}

int OPL_Detect(void) {
    return 1;
}

void OPL_InitRegisters(void) {
}
