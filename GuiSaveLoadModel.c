#include "GuiSaveLoadModel.h"

// Global flags
int result; // The state of the load and save window: if 1 then stay in window ,
            // 0 for returning to main window , 4 to start the loading game

int isSave; // Global flag for indicating whether a current game is saved

// Global Widgets
Widget* backBtn;// back btn
Widget* saveloadWindow; // load window
SDL_Renderer *SaveLoadRend; // the render for both save and load windows
Widget *LoadButtons[NUM_OF_SLOTS]; // the array of load buttons slots;
Widget *SaveButtons[NUM_OF_SLOTS]; // the array of save buttons slots;
Widget *Buttons[2];// left arrow and right arrow for paging
SDL_Texture * saveLogo; // adjusting the load window image to save window image

// Global variables for paging the window
int numOfAllfullPages;
int numOfSlotsLastPage;
int currPageNumber;

SDL_Rect rects[5] = {{.x = 20, .y = 160, .h = 60, .w = 750} ,  // An array of rects for the Slots
                     {.x = 20, .y = 160+84, .h = 60, .w = 750},
                     {.x = 20, .y = 160+84*2, .h = 60, .w = 750},
                     {.x = 20, .y = 160+84*3, .h = 60, .w = 750},
                     {.x = 20, .y = 160+84*4, .h = 60, .w = 750}};

/*
 * In order to change the number of slots simply:
 * 1. change the NUM_OF_SLOTS Macro in the GuiSaveLoadModel.h
 * 2. Add or remove images from the images/imagesDisabled array below
 * 3. Add or remove filename from the fileName array below
 * * important : the code will work for any number of slots iff NUM_OF_SLOT == length(images)
 * ** == length(imagesDisabled) == length(fileName) and file name must be "gamei" where i is the number of the slot
 */

// Array of images to present if filename exists
char *images[NUM_OF_SLOTS] = {"UI/LoadPanel/Slot1.bmp", "UI/LoadPanel/Slot2.bmp", "UI/LoadPanel/Slot3.bmp",
                              "UI/LoadPanel/Slot4.bmp", "UI/LoadPanel/Slot5.bmp"};


// Array of images to present if file does not exist (disable)
char *imagesDisabled[NUM_OF_SLOTS] = {"UI/LoadPanel/Slot1D.bmp", "UI/LoadPanel/Slot2D.bmp","UI/LoadPanel/Slot3D.bmp",
"UI/LoadPanel/Slot4D.bmp", "UI/LoadPanel/Slot5D.bmp"};

// Fixed file Name
char *fileName[NUM_OF_SLOTS] = {"game1.txt" , "game2.txt", "game3.txt"
,"game4.txt", "game5.txt"};





/**---------- Load Window segment ---------
 *
 */



/**
 * param: i (the index of the slot btn in LoadButtons array)
 * return : void
 * action : load the file name from the nameFile array and set the Game window (set result to 4)
 *
 */

void LoadBtn(int i){
    FILE * file;
    file = fopen(fileName[i] , "r");
    if(file != NULL){
        isSave = 1;
        fclose(file);
        LoadPrevGame(fileName[i]);
        result = 4;
        ToggleButton * btn = (ToggleButton*) LoadButtons[i]->data;
        btn->Active(btn);
    }
}



/**
 * param: None
 * return : int,  3 to Quit ,  0 for main window, 4 to start the load Game
 * action : Start the load window and wait for the user event
 *
 */

int LoadWindow(){

    numOfAllfullPages = NUM_OF_SLOTS/5; // setting the number of full pages (5 slots in a page)
    numOfSlotsLastPage = NUM_OF_SLOTS%5; // getting the number of remaining slots
    currPageNumber = 0; // set the current page

    isSave = 0; // set isSave to 0 due to the fact that we can go from this page to main window
    result = 1;
    Widget *LoadWindow;
    SDL_Event e;
    int i;

    // defining the widgets for the load window
    LoadWindow = createWindow("Chess Game - Load Game", SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL,
                                 "UI/LoadPanel/LoadWindow.bmp");

    if (LoadWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_WINDOW, NULL);
        QuitFailureGui();
    }

    saveloadWindow = LoadWindow;
    Window *currWin;
    currWin = (Window *)LoadWindow->data;
    SaveLoadRend = currWin->render;
    i = InitSaveLoadBtn(1);
    if (!i){
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        FreeAllBtns(LoadButtons , NUM_OF_SLOTS);
        FreeAllBtns(Buttons , 2);
        destroyWidget(backBtn);
        destroyWidget(LoadWindow);
        QuitFailureGui();
    }

    DrawAll(LoadButtons , NUM_OF_SLOTS_IN_PAGE , 1); // rendering the first num of slots in page

    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT){
                DestroyAll(1); // release all memory
                result = 3;
                return result;
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                    DestroyAll(1); // release all memory
                    result = 3;
                    return result;;
                }
            }
            else if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == SDL_BUTTON_LEFT)) {
                CheckEvents(e , 1);
                if (result != 1) {
                    DestroyAll(1); // release all memory
                    return result;
                }
            }
        }
    }
}




/**---------- Save Window segment ---------
 *
 */

/**
 * param: int , get the index of the save button slot in SaveButtons Array
 * return : none
 * action : Save the current game in the chosen slot
 *
 */
void SaveBtn(int i) {
    SaveCurrGame(fileName[i]);
    isSave = 1; // change the flag to 1 as the game is been saved
    result = 2; // set result != 1 to continue for the game mode
}

/**
 * param: None
 * return : void
 * action : Start the Window window and wait for the user event;
 * if the slot was pressed then the game saved in slot start;
 * for exit press the x button on window
 *
 */

void SaveWindow() {

    numOfAllfullPages = NUM_OF_SLOTS / 5; // setting the number of full pages (5 slots in a page)
    numOfSlotsLastPage = NUM_OF_SLOTS % 5; // getting the number of remaining slots
    currPageNumber = 0; // set the current page
    result = 1;
    Widget *SaveWindow;
    SDL_Event e;
    int i;


    SaveWindow = createWindow("Chess Game - Save Game", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL,
                              "UI/LoadPanel/LoadWindow.bmp");
    if (SaveWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_WINDOW, NULL);
        QuitFailureGui();
    }
    saveloadWindow = SaveWindow;
    Window *currWin;
    currWin = (Window *) SaveWindow->data;
    SaveLoadRend = currWin->render;
    i = InitSaveLoadBtn(0);
    // check success for initializing the Buttons
    if (!i) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        FreeAllBtns(SaveButtons, NUM_OF_SLOTS);
        FreeAllBtns(Buttons, 2);
        destroyWidget(backBtn);
        destroyWidget(SaveWindow);
        QuitFailureGui();
    }
    // render the window
    DrawAll(SaveButtons, NUM_OF_SLOTS_IN_PAGE , 0);
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                    result = 3;
                }
            }
            else if ((e.type == SDL_MOUSEBUTTONUP) && (e.button.button == SDL_BUTTON_LEFT)) {
                for (i = 0; i < NUM_OF_SLOTS + 1; i++) {
                    CheckEvents(e , 0);
                }
            }
            if (result != 1) {
                DestroyAll(0);
                return;
            }
        }
    }
}

/** ------------------ General function for both load and save window ------
 *
 */


/**
 * param: int : 1 for load window or 0 for save window
 * return : int , 1 if success in creating the Buttons else 0
 * action : Creating the All the Load Buttons or Save Buttons, back btn and left right btn for paging
 *
 */

int InitSaveLoadBtn(int k) {
    int i;
    SDL_Rect rec6 = {.x = 330, .y = 20, .h = 50, .w = 130};
    SDL_Rect rec0 = {.x = 360, .y = 560, .h = 30, .w = 30};
    SDL_Rect rec1 = {.x = 410, .y = 560, .h = 30, .w = 30};
    if (k) {
        backBtn = createButton(SaveLoadRend, 0, "UI/SettingWindow/Back.bmp", rec6, BackLSBtn);
        if (backBtn == NULL)
            return 0;
        for (i = 0; i < NUM_OF_SLOTS; i++) {
            int j;
            j = fopen(fileName[i], "r") == NULL ? 0 : 1;
            LoadButtons[i] = createToggleButton(SaveLoadRend, j, i, imagesDisabled[i], images[i], rects[i % 5],
                                                LoadBtn);
            if (LoadButtons[i] == 0) return 0;
        }
        Buttons[0] = createButton(SaveLoadRend, 1, "UI/SettingWindow/left.bmp", rec0, LeftBtn);
        if (Buttons[0] == NULL)
            return 0;
        Buttons[1] = createButton(SaveLoadRend, 1, "UI/SettingWindow/right.bmp", rec1, RightBtn);
        if (Buttons[1] == NULL)
            return 0;
    } else {
        SDL_Surface *surf = SDL_LoadBMP("UI/LoadPanel/saveLogo.bmp");
        if (surf == NULL) {
            printf(ERROR_SDL_SURFACE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        }
        saveLogo = SDL_CreateTextureFromSurface(SaveLoadRend, surf);
        if (saveLogo == NULL) {
            printf(ERROR_SDL_TEXTURE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        }
        SDL_FreeSurface(surf);
        for (i = 0; i < NUM_OF_SLOTS; i++) {
            SaveButtons[i] = createButton(SaveLoadRend, i, images[i], rects[i % 5], SaveBtn);
            if (SaveButtons[i] == 0) return 0;
        }
        Buttons[0] = createButton(SaveLoadRend, 0, "UI/SettingWindow/left.bmp", rec0, LeftBtn);
        if (Buttons[0] == NULL)
            return 0;
        Buttons[1] = createButton(SaveLoadRend, 0, "UI/SettingWindow/right.bmp", rec1, RightBtn);
        if (Buttons[1] == NULL)
            return 0;
    }
    return 1;
}


/**
 * param: i (always i == 0)
 * return : void
 * action : set result to 0 for returning to main window
 *
 */

void BackLSBtn(int i) {
    result = i;
}


/**
 * param: event e from the LoadWindow function , int : 0 for save window or 1 for load window
 * return : void
 * action : Finds the btn the user had been pressed and start the btn handler
 *
 */
void CheckEvents(SDL_Event e, int j) {
    int i;
    if (j) {
        for (i = 0; i < NUM_OF_SLOTS; i++) {
            LoadButtons[i]->handleEvent(LoadButtons[i], &e);
        }
        backBtn->handleEvent(backBtn, &e);
    } else {
        for (i = 0; i < NUM_OF_SLOTS; i++) {
            SaveButtons[i]->handleEvent(SaveButtons[i], &e);
        }
    }
    for (i = 0; i < 2; i++) {
        Buttons[i]->handleEvent(Buttons[i], &e);
    }

}



/**----------------------- Paging functions --------------------------
 * As number of slots is changing we chose to implement paging to show all the slots
 * the right btn display the next slots
 * the left btn display the prev slots
 * the number of slots for a page is constant and equal to 5.
 * the number of total slots can be modified and is equal to 5 by default
 */


/**
 * param: int i -> 1 for load window or 0 for save window
 * return : void
 * action : render to the window the next 5 slots in LoadButtons array
 *
 */

void RightBtn(int i){
    currPageNumber++;
    // for a pages with 5 slots
    if(0 <= currPageNumber && currPageNumber < numOfAllfullPages){
        int k , j;
        Widget** temp = (Widget**)malloc(sizeof(Widget*)*5);
        for(k = 0 ; k < 5 ; k++){
            j = currPageNumber*5+k;
            if(i){
                temp[k] = LoadButtons[j];
            }
            else{
                temp[k] = SaveButtons[j];
            }
        }
        if(i){
            DrawAll(temp, 5, 1);
        }
        else{
            DrawAll(temp, 5, 0);
        }
        free(temp);
    }
    // for the last page contain 0-4 slots
    else if (currPageNumber == numOfAllfullPages){
        if(numOfSlotsLastPage != 0) {
            int k , j;
            Widget **temp = (Widget **) malloc(sizeof(Widget *) * numOfSlotsLastPage);
            for (k = 0; k < numOfSlotsLastPage; k++) {
                j = numOfAllfullPages*5+k;
                if(i) {
                    temp[k] = LoadButtons[j];
                }
                else{
                    temp[k] = SaveButtons[j];
                }
            }
            if(i){
                DrawAll(temp, numOfSlotsLastPage, 1);
            }
            else{
                DrawAll(temp, numOfSlotsLastPage, 0);
            }
            free(temp);
        }
    }
    if(currPageNumber > numOfAllfullPages){
        currPageNumber--;
    }
}


/**
 * param: int i -> 1 for load window or 0 for save window
 * return : void
 * action : render to the window the prev 5 slots in LoadButtons array
 *
 */
void LeftBtn(int i) {
    currPageNumber--;
    // can render only pages with 5 slots
    if (0 <= currPageNumber && currPageNumber < numOfAllfullPages) {
        int k, j;
        Widget **temp = (Widget **) malloc(sizeof(Widget *) * 5);
        for (k = 0; k < 5; k++) {
            j = currPageNumber * 5 + k;
            if (i) {
                temp[k] = LoadButtons[j];
            }
            else{
                temp[k] = SaveButtons[j];
            }
        }
        if(i)
            DrawAll(temp, 5 ,1);
        else
            DrawAll(temp, 5, 0);
        free(temp);
    }
    if(currPageNumber == -1){
        currPageNumber++;
    }
}

/** ------------Destroy and Draw ------------
 *
 */


/**
 * param: int : 1 for load window or 0 for save window
 * return : void
 * action : Free all widgets memory in the load/save window
 *
 */
void DestroyAll(int j){
    int i;
    if(j){
        for (i = 0; i < NUM_OF_SLOTS ; i++) {
            LoadButtons[i]->destroy(LoadButtons[i]);
        }
        backBtn->destroy(backBtn);
    }
    else{
        for(i = 0 ; i < NUM_OF_SLOTS ; i++){
            SaveButtons[i]->destroy(SaveButtons[i]);
        }
        SDL_DestroyTexture(saveLogo);
    }
    Buttons[0]->destroy(Buttons[0]);
    Buttons[1]->destroy(Buttons[1]);
    saveloadWindow->destroy(saveloadWindow);
}

/**
 * param: widget** int n - the size of the widget array ; int m - 0 for save and 1 for load
 * return : void
 * action : render to the window the relevant slots
 *
 */

void DrawAll(Widget ** wid , int n , int m){
    int i;
    saveloadWindow->draw(saveloadWindow,SaveLoadRend);
    for(i = 0 ; i < n ; i++){
        wid[i]->draw(wid[i] , SaveLoadRend);
    }
    if(m){
        backBtn->draw(backBtn , SaveLoadRend);
    }
    else{
        SDL_Rect rec = {.x = 210, .y = 85, .h = 40, .w = 380};
        SDL_RenderCopy(SaveLoadRend , saveLogo , NULL ,&rec);
        SDL_RenderPresent(SaveLoadRend);
    }
    Buttons[0]->draw(Buttons[0], SaveLoadRend);
    Buttons[1]->draw(Buttons[1], SaveLoadRend);
}
