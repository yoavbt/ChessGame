#ifndef FINAL_PROJECT_GUISTART_H
#define FINAL_PROJECT_GUISTART_H

#include "Gen.h"
#include "GuiSettingModel.h"
#include "GuiSaveLoadModel.h"
#include "GuiGameModel.h"


void InitGui();

void StartGui();

void QuitGui();

void QuitFailureGui();

int StartWindow();

void QuitBtn(int i);

void NewGameBtn(int i);

void LoadGameBtn(int i);


#endif //FINAL_PROJECT_GUISTART_H
