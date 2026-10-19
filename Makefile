# Makefile for Moktar (Windows & Linux C++ Replica)

CC ?= gcc
CXX ?= g++

# Platform Detection
ifeq ($(OS),Windows_NT)
    PLATFORM = Windows
else
    UNAME_S := $(shell uname -s 2>/dev/null || echo Linux)
    ifeq ($(findstring Linux,$(UNAME_S)),Linux)
        PLATFORM = Linux
    else
        PLATFORM = Windows
    endif
endif

ifeq ($(PLATFORM),Windows)
    TARGET = bin/moktar.exe
    INCLUDE = -I./include $(shell sdl-config --cflags 2>/dev/null || echo -I./sdl_compat_local/usr/include)
    CFLAGS  = -std=gnu17 -O2 -g -fcommon -fpermissive -w $(INCLUDE)
    CXXFLAGS = -std=c++17 -O2 -g -fcommon -w $(INCLUDE)
    LDFLAGS = -mconsole -lSDL -lSDL2 -lm -static-libgcc -static-libstdc++
else
    TARGET = bin/moktar
    INCLUDE = -I./include -I./sdl_compat_local/usr/include
    CFLAGS  = -std=gnu17 -O2 -g -fcommon -fpermissive -w $(INCLUDE)
    CXXFLAGS = -std=c++17 -O2 -g -fcommon -w $(INCLUDE)
    LDFLAGS = -L./lib -lSDL -lSDL2 -lm -lpthread '-Wl,-rpath,$$ORIGIN/../lib:$$ORIGIN/lib' -Wl,-rpath-link,./lib
endif

OBJDIR = obj

C_SRCS = \
	src/audio.c \
	src/common.c \
	src/draw.c \
	src/elevators.c \
	src/enemies.c \
	src/engine.c \
	src/fonts.c \
	src/gates.c \
	src/keyboard.c \
	src/level.c \
	src/menu.c \
	src/objects.c \
	src/original.c \
	src/player.c \
	src/reset.c \
	src/scroll.c \
	src/settings.c \
	src/sprites.c \
	src/sqz.c \
	src/tile_animation.c \
	src/viewimage.c \
	src/dbopl.c \
	src/opl_bridge.c \
	src/init.c

CXX_SRCS = \
	src/main.cpp

C_OBJS = $(patsubst src/%.c, $(OBJDIR)/%.o, $(C_SRCS))
CXX_OBJS = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(CXX_SRCS))
ALL_OBJS = $(C_OBJS) $(CXX_OBJS)

all: dirs $(TARGET)

dirs:
	@mkdir -p bin $(OBJDIR)

$(OBJDIR)/%.o: src/%.c
	@echo "  CC    $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/%.cpp
	@echo "  CXX   $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(ALL_OBJS)
	@echo "  LINK  $@"
	@$(CXX) -o $@ $(ALL_OBJS) $(LDFLAGS)
ifeq ($(PLATFORM),Windows)
	@which cygpath >/dev/null 2>&1 && for dll in SDL.dll SDL2.dll libwinpthread-1.dll; do \
		p=$$(which $$dll 2>/dev/null); \
		if [ -n "$$p" ] && [ -f "$$p" ]; then cp -u "$$p" bin/ 2>/dev/null || true; fi; \
	done || true
endif
	@echo "==> Moktar compilé avec succès dans $(TARGET) !"

clean:
	@rm -rf $(OBJDIR) bin/moktar bin/moktar.exe titus.conf music_mok.bin
	@echo "==> Nettoyage terminé."

run: all
ifeq ($(PLATFORM),Windows)
	@$(TARGET)
else
	@./run_moktar.sh
endif
