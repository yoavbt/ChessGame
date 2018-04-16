#ifndef FINAL_PROJECT_GUIAUX_H
#define FINAL_PROJECT_GUIAUX_H

#include "SDL.h"
#include "SDL_video.h"
#include <stdlib.h>
#include <stdio.h>



#define ERROR_SDL_WINDOW "ERROR: unable to create Window: %s\n"
#define ERROR_SDL_RENDER "ERROR: unable to create Renderer: %s\n"
#define ERROR_SDL_TEXTURE "ERROR: unable to create Texture: %s\n"
#define ERROR_SDL_SURFACE "ERROR: unable to create Surface: %s\n"
#define MEMORY_ERROR "ERROR: Memory Allocation Error\n"
#define SCREEN_W 800
#define SCREEN_H 600
#define ERROR_SDL_Init "ERROR: SDL_Init had failed : %s\n"
#define ERROR_WIDGET_WINDOW "ERROR: unable to create Window Widget\n"
#define ERROR_WIDGET_BTN "ERROR: unable to create Button Widget\n"
#define ERROR_WIDGET_PANEL "ERROR: unable to create Panel Widget\n"
#define ERROR_BOX "Error!"

extern int GameMode;
extern char *CurrPlayerColor;
extern int Difficulty;
extern char *UserColor;
extern char *CompColor;

void QuitGui();
void QuitFailureGui();


#endif //FINAL_PROJECT_GUIAUX_H
