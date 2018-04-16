#include "GuiSettingModel.h"

/**----- This Model contain function of the setting model and setting window ------
 */

int result;// Global variable for setting model state: 4 for start game
// 0 for main menu 3 to quit else stay in setting state
SDL_Renderer * SettingRend;
Widget *Buttons[11]; // the btn need for the setting window


/**
 * param: none
 * return : int -> 4 for start game 0 for main menu 3 for quiting
 * action : set and active the setting window
 *
 */
int SettingWindow() {
    result = 1;
    Widget *SettingWindow;
    Widget *SettingPanel;
    SDL_Event e;
    int i;
    SettingWindow = createWindow("Chess Game - Setting Window", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL,
                                 "UI/SettingWindow/SettingWindow.bmp");
    if (SettingWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX , ERROR_WIDGET_WINDOW , NULL);
        QuitFailureGui();
    }
    Window *currWin;
    currWin = (Window *) SettingWindow->data;
    SettingRend = currWin->render;
    i = InitSettingBtns();// setting the btns and check success for init all btns
    if (!i) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        FreeAllBtns(Buttons, 11);
        SettingWindow->destroy(SettingWindow);
        QuitFailureGui();
    }
    SettingPanel = createPanel(&SettingWindow, 1, Buttons, 11); // create the setting panel include window and btns
    if (SettingPanel == NULL) {
        printf(ERROR_WIDGET_PANEL);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_WIDGET_PANEL, SDL_GetError(), NULL);
        DestroyBtn(11);
        SettingWindow->destroy(SettingWindow);
        QuitFailureGui();
    }
    SettingPanel->draw(SettingPanel, SettingRend);
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                result = 3;
                return result;
            } else if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == SDL_BUTTON_LEFT)) {
                for (i = 0; i < 11; i++) {
                    Buttons[i]->handleEvent(Buttons[i], &e);
                }
                if (result!=1) {
                    SettingPanel->destroy(SettingPanel);
                    return result;
                }
            }
        }
    }
}

/**
 * param: none
 * return : int -> 1 if all btn initialize or 0 else
 * action : initializing all the btns as toggle btn
 *
 */

int InitSettingBtns() {
    int i;
    SDL_Rect rec1 = {.x = 250, .y = 170, .h = 50, .w = 130};
    SDL_Rect rec2 = {.x = 420, .y = 170, .h = 50, .w = 130};
    SDL_Rect rec3 = {.x = 100, .y = 292, .h = 50, .w = 110};
    SDL_Rect rec4 = {.x = 225, .y = 290, .h = 54, .w = 114};
    SDL_Rect rec5 = {.x = 350, .y = 290, .h = 50, .w = 110};
    SDL_Rect rec6 = {.x = 470, .y = 292, .h = 50, .w = 110};
    SDL_Rect rec7 = {.x = 590, .y = 294, .h = 47, .w = 110};
    SDL_Rect rec8 = {.x = 250, .y = 430, .h = 50, .w = 130};
    SDL_Rect rec9 = {.x = 420, .y = 430, .h = 50, .w = 130};
    SDL_Rect rec10 = {.x = 50, .y = 500, .h = 60, .w = 130};
    SDL_Rect rec11 = {.x = 620, .y = 500, .h = 60, .w = 130};
    Buttons[0] = createToggleButton(SettingRend, 1 , 1 , "UI/SettingWindow/player1.bmp",
                                    "UI/SettingWindow/player1m.bmp" , rec1 ,Player1Btn );
    Buttons[1] = createToggleButton(SettingRend, 0 , 2, "UI/SettingWindow/player2.bmp" ,
                                    "UI/SettingWindow/player2m.bmp", rec2 ,Player2Btn);
    Buttons[2] = createToggleButton(SettingRend, 0 , 1 , "UI/SettingWindow/AM.bmp",
                                    "UI/SettingWindow/AMm.bmp" , rec3 ,AmatureBtn);
    Buttons[3] = createToggleButton(SettingRend, 1, 2 ,"UI/SettingWindow/EA.bmp",
                                    "UI/SettingWindow/EAm.bmp" , rec4 ,EasyBtn);
    Buttons[4] = createToggleButton(SettingRend, 0, 3 ,"UI/SettingWindow/Mod.bmp",
                                    "UI/SettingWindow/Modm.bmp" , rec5 , ModerateBtn);
    Buttons[5] = createToggleButton(SettingRend, 0 ,4 ,"UI/SettingWindow/Hr.bmp",
                                    "UI/SettingWindow/Hrm.bmp" , rec6 ,HardBtn);
    Buttons[6] = createToggleButton(SettingRend, 0, 5, "UI/SettingWindow/Ex.bmp",
                                    "UI/SettingWindow/Exm.bmp" , rec7 ,ExpertBtn);
    Buttons[7] = createToggleButton(SettingRend, 1, 1 ,"UI/SettingWindow/White.bmp",
                                    "UI/SettingWindow/Whitem.bmp" , rec8 ,WhiteBtn);
    Buttons[8] = createToggleButton(SettingRend, 0 ,1 ,  "UI/SettingWindow/Black.bmp",
                                    "UI/SettingWindow/Blackm.bmp" , rec9 ,BlackBtn);
    Buttons[9] = createButton(SettingRend,0, "UI/SettingWindow/Back.bmp", rec10, BackBtn);
    Buttons[10] = createButton(SettingRend,4, "UI/SettingWindow/Start.bmp", rec11 ,StartBtn);
    for(i = 0 ; i < 11 ; i++){
        if(Buttons[i] == NULL) return 0;
    }
    return 1;
}

/**
 * param: int
 * return : void
 * action : set game mode to 1; activate all the other default btns
 *
 */


void Player1Btn(int j) {
    GameMode = j;
    ToggleButton *btn1 = (ToggleButton *) (Buttons[0]->data);
    ToggleButton *btn2 = (ToggleButton *) (Buttons[1]->data);
    if(btn2->i == 1){
        btn2->Active(btn2);
    }
    if(btn1->i == 0){
        btn1->Active(btn1);
    }
    Buttons[0]->draw(Buttons[0] , SettingRend);
    Buttons[1]->draw(Buttons[1] , SettingRend);
    SetDefault();
    ActiveSetting();
}


/**
 * param: int
 * return : void
 * action : set game mode to 2; deactivate all the other btns
 *
 */
void Player2Btn(int j){
    GameMode = j;
    ToggleButton *btn1 = (ToggleButton *) (Buttons[0]->data);
    ToggleButton *btn2 = (ToggleButton *) (Buttons[1]->data);
    if (btn2->i == 0) {
        btn2->Active(btn2);
    }
    if (btn1->i == 1) {
        btn1->Active(btn1);
    }
    Buttons[0]->draw(Buttons[0], SettingRend);
    Buttons[1]->draw(Buttons[1], SettingRend);
    DeActiveSetting();
}

/**
 * param: int
 * return : void
 * action : activate (draw the active image) the btns that has btn->i of 1;
 *
 */
void ActiveSetting(){
    int i;
    for (i = 2 ; i < 9  ; i++){
        ToggleButton * btn = (ToggleButton*)(Buttons[i]->data);
        if (i == 3 || i == 7){
            btn->i = 1;
            Buttons[i]->draw(Buttons[i], SettingRend);
            continue;
        }
        btn->i = 0;
        Buttons[i]->draw(Buttons[i], SettingRend);
    }
}

/**
 * param: int
 * return : void
 * action : deactivate (draw the deactivate image) the btns that has btn->i of 0;
 *
 */
void DeActiveSetting(){
    int i;
    for (i = 2; i < 9; i++) {
        ToggleButton *btn = (ToggleButton *)(Buttons[i]->data);
        btn->i = 0;
        Buttons[i]->draw(Buttons[i], SettingRend);
    }
}

/**
 * param: int
 * return : void
 * action : destroy all the btns (free memory of all btns)
 *
 */
void DestroyBtn(int j){
    int i;
    for (i = 0; i < j; i++) {
        Buttons[i]->destroy(Buttons[i]);
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global difficulty variable to 1 and active amature btn deactivate all difficulty btns
 *  if game == 0 do nothing
 */

void AmatureBtn(int j) {
    if(GameMode == 1){
        int i;
        Difficulty = j;
        for (i = 2; i < 7; i++) {
            ToggleButton *btn = (ToggleButton *)(Buttons[i]->data);
            if (i == 2) {
                if (btn->i == 0) {
                    btn->Active(btn);
                }
            } else {
                if (btn->i == 1) {
                    btn->Active(btn);
                    Buttons[i]->draw(Buttons[i], SettingRend);
                }
            }
            Buttons[i]->draw(Buttons[i], SettingRend);
        }
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global difficulty variable to 2 and active Easy btn deactivate all difficulty btns
 *  if game == 0 do nothing
 */
void EasyBtn(int j){
    if(GameMode == 1){
        int i;
        Difficulty = j;
        for (i = 2; i < 7; i++) {
            ToggleButton *btn = (ToggleButton *) (Buttons[i]->data);
            if (i == 3) {
                if (btn->i == 0) {
                    btn->Active(btn);
                }
            } else {
                if (btn->i == 1) {
                    btn->Active(btn);
                    Buttons[i]->draw(Buttons[i], SettingRend);
                }
            }
            Buttons[i]->draw(Buttons[i], SettingRend);
        }
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global difficulty variable to 3 and active Moderate btn deactivate all difficulty btns
 *  if game == 0 do nothing
 */
void ModerateBtn(int j){
    if(GameMode == 1){
        int i;
        Difficulty = j;
        for (i = 2; i < 7; i++) {
            ToggleButton *btn = (ToggleButton *) (Buttons[i]->data);
            if (i == 4) {
                if (btn->i == 0) {
                    btn->Active(btn);
                }
            }
            else {
                if (btn->i == 1) {
                    btn->Active(btn);
                    Buttons[i]->draw(Buttons[i], SettingRend);
                }
            }
            Buttons[i]->draw(Buttons[i], SettingRend);
        }
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global difficulty variable to 4 and active Hard btn deactivate all difficulty btns
 *  if game == 0 do nothing
 */
void HardBtn (int j){
    if (GameMode == 1){
        int i;
        Difficulty = j;
        for (i = 2; i < 7; i++) {
            ToggleButton *btn = (ToggleButton *) (Buttons[i]->data);
            if (i == 5) {
                if (btn->i == 0) {
                    btn->Active(btn);
                }
            }
            else {
                if (btn->i == 1) {
                    btn->Active(btn);
                    Buttons[i]->draw(Buttons[i], SettingRend);
                }
            }
            Buttons[i]->draw(Buttons[i], SettingRend);
        }
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global difficulty variable to 3 and active Expert btn deactivate all difficulty btns
 *  if game == 0 do nothing
 */
void ExpertBtn(int j){
    if(GameMode == 1){
        int i;
        Difficulty = j;
        for (i = 2; i < 7; i++) {
            ToggleButton *btn = (ToggleButton *)(Buttons[i]->data);
            if (i == 6) {
                if (btn->i == 0) {
                    btn->Active(btn);
                }
            }
            else {
                if (btn->i == 1) {
                    btn->Active(btn);
                    Buttons[i]->draw(Buttons[i], SettingRend);
                }
            }
            Buttons[i]->draw(Buttons[i], SettingRend);
        }
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global Usercolor variable to White and active White btn deactivate Black btn
 *  if game == 0 do nothing
 */
void WhiteBtn(int j) {
    if(GameMode == j){
        UserColor = WHITE;
        ToggleButton *btn1 = (ToggleButton *) (Buttons[7]->data);
        ToggleButton *btn2 = (ToggleButton *) (Buttons[8]->data);
        if (btn1->i == 0) {
            btn1->Active(btn1);
        }
        if (btn2->i == 1) {
            btn2->Active(btn2);
        }
        Buttons[7]->draw(Buttons[7] , SettingRend);
        Buttons[8]->draw(Buttons[8], SettingRend);
    }
}

/**
 * param: int
 * return : void
 * action : if game==1 set the global Usercolor variable to Black and active Black btn deactivate White btn
 *  if game == 0 do nothing
 */

void BlackBtn(int j){
    if(GameMode == j){
        UserColor = BLACK;
        ToggleButton *btn1 = (ToggleButton *) (Buttons[7]->data);
        ToggleButton *btn2 = (ToggleButton *) (Buttons[8]->data);
        if (btn1->i == 1) {
            btn1->Active(btn1);
        }
        if (btn2->i == 0) {
            btn2->Active(btn2);
        }
        Buttons[7]->draw(Buttons[7], SettingRend);
        Buttons[8]->draw(Buttons[8], SettingRend);
    }
}

/**
 * param: int
 * return : void
 * action : set action state flag to 4 -> Start Game state
 */
void StartBtn(int i){
    result = i;
}

/**
 * param: int
 * return : void
 * action : set action state flag to 0 -> Main Window state
 */
void BackBtn(int i){
    result = i;
}

