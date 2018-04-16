#ifndef FINAL_PROJECT_GUISETTINGMODE_H
#define FINAL_PROJECT_GUISETTINGMODE_H

#include "SettingState.h"
#include "Widgets.h"


int SettingWindow();

int InitSettingBtns();

void Player1Btn(int j);

void Player2Btn(int j);

void ActiveSetting();

void DeActiveSetting();

void DestroyBtn(int j);

void AmatureBtn(int j);

void EasyBtn(int j);

void ModerateBtn(int j);

void HardBtn(int j);

void ExpertBtn(int j);

void WhiteBtn(int j);

void BlackBtn(int j);

void StartBtn(int i);

void BackBtn(int i);

void Active(ToggleButton *btn);

#endif //FINAL_PROJECT_GUISETTINGMODE_H
