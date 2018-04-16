#include "GuiStart.h"

int action = 0; // Global state flag for the current state of the Gui
SDL_Renderer * StartRend; //A render for the start window


// init the gui function
void InitGui(){

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf(ERROR_SDL_Init ,SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        exit(EXIT_FAILURE);
    }
};


/**
 * param: none
 * return: void
 * action: start a loop for the state of the Gui
 */

void StartGui(){
    while(1){
        // if 0 then start the main window again after restarting the settings
        if (action == 0 ){
            SetDefault();
            InitBoard();
            action = StartWindow();
        }
        // if 1 go to setting window
        else if(action == 1){
            action = SettingWindow();
        }
        // if 2 go to load window
        else if(action == 2){
            action = LoadWindow();
        }
        // if 3 than quit the SDL and terminate the program
        else if(action == 3){
            QuitGui();
            return;
        }
        // if 4 than start the game window
        else if(action == 4){
            action = StartGameWindow();
        }
    }
};

/**
 * param: none
 * return: int ; 1 for new Game -> go to setting window
 *               2 for load Game -> go to load window
 *               3 for Quit -> Quit SDL
 * action: set and active the start window
 */

int StartWindow(){
    Widget * StartWindow;
    Widget * StartPanel;
    Widget * Buttons[3];
    SDL_Event e;
    int i;
    StartWindow = createWindow("Chess Game - Main Window" , SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,SCREEN_W,SCREEN_H,SDL_WINDOW_OPENGL, "UI/MainMenu/StartWindow.bmp");
    if(StartWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_WINDOW, NULL);
        QuitFailureGui();
    }
    Window * currWin = (Window *)StartWindow->data;
    StartRend = currWin->render;
    SDL_SetRenderDrawBlendMode(StartRend, SDL_BLENDMODE_BLEND);
    SDL_Rect  rect0 = {.x = 230, .y = 150, .w = 350, .h = 45};
    SDL_Rect  rect1 = {.x = 230, .y = 210, .w = 350, .h = 47};
    SDL_Rect  rect2 = {.x = 230, .y = 360, .w = 350, .h = 45};
    Buttons[0] = createButton(StartRend, 1 ,"UI/MainMenu/NewGameBtn.bmp" , rect0 ,NewGameBtn);
    if(Buttons[0] == NULL){
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        StartWindow->destroy(StartWindow);
        QuitFailureGui();
    }
    Buttons[1] = createButton(StartRend,2 , "UI/MainMenu/LoadGameBtn.bmp" ,rect1 ,LoadGameBtn);
    if (Buttons[1] == NULL) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        StartWindow->destroy(StartWindow);
        Buttons[0]->destroy(Buttons[0]);
        QuitFailureGui();
    }
    Buttons[2] = createButton(currWin->render,3, "UI/MainMenu/quitBtn.bmp",rect2 , QuitBtn);
    if (Buttons[2] == NULL) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        StartWindow->destroy(StartWindow);
        Buttons[0]->destroy(Buttons[0]);
        Buttons[1]->destroy(Buttons[1]);
        QuitFailureGui();
    }
    StartPanel = createPanel(&StartWindow,1,Buttons,3);
    if (StartPanel == NULL) {
        printf(ERROR_WIDGET_PANEL);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_PANEL, NULL);
        StartWindow->destroy(StartWindow);
        Buttons[0]->destroy(Buttons[0]);
        Buttons[1]->destroy(Buttons[1]);
        Buttons[2]->destroy(Buttons[2]);
        QuitFailureGui();
    }
    StartPanel->draw(StartPanel , currWin->render);
    while(1){
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                action = 3;
                return action;
            } else if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == SDL_BUTTON_LEFT)) {
                for (i = 0; i < 3 ; i++) {
                    Buttons[i]->handleEvent(Buttons[i], &e);
                    }
                if (action){
                    StartPanel->destroy(StartPanel);
                    return action;
                }
            }
        }
    }
}

/**
 * param: int
 * return: void
 * action: the new Game btn event. set the action to 1 (go to setting window)
 * and switch the color of the btn for indication of pressing
 */


void NewGameBtn(int i){
    SDL_Surface *marked = SDL_LoadBMP("UI/MainMenu/NewGameHB.bmp");
    if (marked == NULL){
        printf(ERROR_SDL_SURFACE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
    }
    else{
        SDL_Texture *mark = SDL_CreateTextureFromSurface(StartRend, marked);
        if (mark == NULL) {
            SDL_FreeSurface(marked);
            printf(ERROR_SDL_TEXTURE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
            printf(ERROR_WIDGET_BTN);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        } else {
            SDL_Rect rect0 = {.x = 230, .y = 150, .w = 350, .h = 45};
            SDL_FreeSurface(marked);
            SDL_RenderCopy(StartRend, mark, NULL, &rect0);
            SDL_RenderPresent(StartRend);
        }
    }
    SDL_Delay(100);
    action = i;
}

/**
 * param: int
 * return: void
 * action: the Load Game btn event. set the action to 2 (go to load window)
 * and switch the color of the btn for indication of pressing
 */

void LoadGameBtn(int i) {
    SDL_Surface *marked = SDL_LoadBMP("UI/MainMenu/LoadGameHB.bmp");
    if (marked == NULL) {
        printf(ERROR_SDL_SURFACE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX,ERROR_WIDGET_BTN, NULL);
    }
    else {
        SDL_Texture *mark = SDL_CreateTextureFromSurface(StartRend, marked);
        if (mark == NULL) {
            SDL_FreeSurface(marked);
            printf(ERROR_SDL_TEXTURE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
            printf(ERROR_WIDGET_BTN);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        }
        else {
            SDL_Rect rect1 = {.x = 230, .y = 210, .w = 350, .h = 47};
            SDL_FreeSurface(marked);
            SDL_RenderCopy(StartRend, mark, NULL, &rect1);
            SDL_RenderPresent(StartRend);
        }
    }
    SDL_Delay(100);
    action = i;
}

/**
 * param: int
 * return: void
 * action: the Quit Game btn event. set the action to 3 (Quit the SDL and terminate program)
 * and switch the color of the btn for indication of pressing
 */

void QuitBtn(int i) {
    SDL_Surface *marked = SDL_LoadBMP("UI/MainMenu/quitHB.bmp");
    if (marked == NULL) {
        printf(ERROR_SDL_SURFACE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
    }
    else {
        SDL_Texture *mark = SDL_CreateTextureFromSurface(StartRend, marked);
        if (mark == NULL) {
            SDL_FreeSurface(marked);
            printf(ERROR_SDL_TEXTURE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
            printf(ERROR_WIDGET_BTN);
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        } else {
            SDL_Rect rect2 = {.x = 230, .y = 360, .w = 350, .h = 45};
            SDL_FreeSurface(marked);
            SDL_RenderCopy(StartRend, mark, NULL, &rect2);
            SDL_RenderPresent(StartRend);
        }
    }
    SDL_Delay(100);
    action = i;
}


