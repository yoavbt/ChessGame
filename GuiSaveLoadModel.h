#ifndef FINAL_PROJECT_GUILOAD_H
#define FINAL_PROJECT_GUILOAD_H

#include "Widgets.h"
#include <errno.h>
#include "SaveLoadModel.h"

#define NUM_OF_SLOTS 5
#define NUM_OF_SLOTS_IN_PAGE  5


int LoadWindow();

void SaveWindow();

int InitSaveLoadBtn(int k);

void LeftBtn(int i);

void RightBtn(int i);

void BackLSBtn(int i);

void SaveBtn(int i);

void CheckEvents(SDL_Event e ,int j);

void DestroyAll(int j);

void DrawAll(Widget **wid, int n ,int m);


#endif //FINAL_PROJECT_GUILOAD_H
