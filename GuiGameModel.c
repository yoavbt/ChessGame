#include "GuiGameModel.h"

/**----- This Model is implementing the Game Window and Game function related to Gui ----
 *
 */


int GameWindowAction; // global state of the game window
int restart; // restart flag
int end = 0; // end flag ( end == 1 if checkmate or draw ; else 0)


/**
 * Gloabal widgets for the game window
 */

SDL_Renderer *rend; // the global render
Widget *widgets[7]; // the global btns in the right part of the window
Widget *color[2]; // the colors indicators logos for who's turn is
Widget *save[2]; // the save indicators logos for whether the current game is save or no
Widget *Squares[64]; // the 64 squares btn of the Gui Board
Widget *MainPanel; // the main panel of the game window -> contain GamePanel and Squares array
Widget *GameWindow;// the game window
Widget *GamePanel;// the game panel -> contains game window and widgets array


/**
 * Global variables for the current turn and prev turn squares
 */

PieceMoves *moves;
Widget* Prevpress;
Widget* currPress;
cell * cellCurrPress;


/** param: none
 *  return: int; 3 for quit , 0 for main menu
 *  action: start the main loop for the game window
 */

int StartGameWindow() {
    GameWindowAction = -2;
    restart = 1;
    while(restart) {
        InitMovesHistory();
        end = 0;
        GameWindowAction = SetGameWindow();
    }
    FreeMoveHistory();
    return GameWindowAction;
}

/** param: none
 *  return: int; 3 for quit , 0 for main menu
 *  action: set and start the game window
 */

int SetGameWindow() {
    int GameStatusResult;
    GameWindow = createWindow("Game mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_W, SCREEN_H,
                              SDL_WINDOW_OPENGL, "UI/GameWindow/GameBoard.bmp");
    if (GameWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_WINDOW, NULL);
        destroyAllResourceAndQuit(1);
    }
    rend = ((Window *) GameWindow->data)->render;
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SetBoard();
    InitBtn();
    GamePanel = createPanel(&GameWindow, 1, widgets, 7);
    if(GamePanel == NULL){
        printf(ERROR_WIDGET_PANEL);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_PANEL, NULL);
        destroyAllResourceAndQuit(1);
    }
    MainPanel = createPanel(&GamePanel, 1, Squares, 64);
    if (MainPanel == NULL) {
        printf(ERROR_WIDGET_PANEL);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_PANEL, NULL);
        destroyAllResourceAndQuit(1);
    }
    DisplayWindow();
    UpdateColor();
    while (1) {
        if (GameMode == 1) {
            if (strcmp(CurrPlayerColor, UserColor) == 0) {
                GameWindowAction = GuiUserTurn();
            }
            else {
                GameWindowAction = GuiComputerTurn();
            }
        }
        else {
            GameWindowAction = GuiUserTurn();
        }
        if (GameWindowAction != -1) {
            destroyAllResourceAndQuit(0);
            return GameWindowAction;
        }
        GameStatusResult = GameStatus(CurrPlayerColor);
        switch (GameStatusResult) {
            // Nothing (go to next turn)
            case 0:
                break;
                // Tie (pop msg and disable board)
            case 1:
                end = 1;
                PrintTieMassage();
                break;
                // Memory allocation error (free gameZone and exit game)
            case 2:
                printf(MEMORY_ERROR);
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                destroyAllResourceAndQuit(1);
                break;
                // Check (pop msg and go back to screen)
            case 3:
                if(strcmp(CurrPlayerColor , WHITE) == 0){
                    PrintCheckMassage(1);
                }
                else {
                    PrintCheckMassage(0);
                }
                break;
                // Mate (pop msg and disable board)
            case 4:
                end = 1;
                if (strcmp(CurrPlayerColor, WHITE) == 0) {
                    PrintMateMassage(0);
                }
                else {
                    PrintMateMassage(1);
                }
                break;
            default:
                break;
        }
        initGuiBoard();
        DisplayWindow();
        CurrPlayerColor = strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK;
        UpdateColor();
    }
}


/** param: none
 *  return: int; -1 for continue looping
 *  action: set the computer move and render it to the window. in case of memory error the function quit the gui mode
 *  and terminate the program after releasing all resources
 */

int GuiComputerTurn(){
    GameWindowAction = -2;
    PieceMoves *AITurn = GetAIMove(Board, CurrPlayerColor);
    if (AITurn == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        destroyAllResourceAndQuit(1);
        exit(EXIT_FAILURE);
    }
    UpdateBoard(Board, AITurn->Cell, AITurn->listOfMoves->Cell);
    UpdateMovesHistory(Board, AITurn->Cell, AITurn->listOfMoves->Cell);
    FreePieceMoves(AITurn);
    initGuiBoard();
    DisplayWindow();
    return -1;
}

/** param: none
 *  return: int; 3 for quit , 0 for main menu
 *  action: start the main loop for the user turn. ends when a user press a btn
 */

int GuiUserTurn() {
    GameWindowAction = -2;
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                restart = 0;
                return 3;
            } else if (((e.type == SDL_MOUSEBUTTONDOWN)|| (e.type == SDL_MOUSEBUTTONUP))) {
                ActiveWidgets(e);
                ActiveSquares(e);
                if (GameWindowAction != -2) {
                    return GameWindowAction;
                }
            }
        }
    }
}

/** param: SDL_Event e
 *  return: void
 *  action: check if one of the widgets array btns had been pressed if so active the action function
 */


void ActiveWidgets(SDL_Event e){
    int i;
    for (i = 0; i < 7; i++) {
        widgets[i]->handleEvent(widgets[i], &e);
    }
}

/** param: SDL_Event e
 *  return: void
 *  action: check if one of the Squares array btns had been pressed if so active the action function
 */

void ActiveSquares(SDL_Event e){
    int i;
    for (i = 0; i < 64; i++) {
        Squares[i]->handleEvent(Squares[i], &e);
    }
}

/** param: void
 *  return: void
 *  action: set the Gui board to be the same as the Global Board variable; the Gui board is represent
 *  using the Sqaures array
 */
void SetBoard() {
    int i, j;
    int cnt = 0;
    char * type;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect rec;
            SetPosition(&rec, i, j);
            type = getPieceImageByType(Board[i][j], GetSquareColor(i, j));
            int k;
            k = type == NULL ? 0 : 1;
            Squares[cnt] = createPieceButton(rend,k ,cnt,0,0, type, rec, LeftClickHandler , RightClickHandler);
            if (Squares[cnt] == NULL) {
                printf(ERROR_WIDGET_BTN);
                destroyAllResourceAndQuit(1);
            }
            cnt++;
        }
    }
}

/** param: void
 *  return: void
 *  action: init all the widgets btn; if a critical btn failed to init than remove all resources and quit.
 */

void InitBtn() {
    SDL_Rect rec = {.x = 667, .y = 72, .h = 50, .w = 50};
    SDL_Rect rec1 = {.x = 641, .y = 148, .h = 33, .w = 107};
    SDL_Rect rec2 = {.x = 644, .y = 198, .h = 30, .w = 100};
    SDL_Rect rec3 = {.x = 640, .y = 248, .h = 36, .w = 107};
    SDL_Rect rec4 = {.x = 641, .y = 348, .h = 33, .w = 107};
    SDL_Rect rec5 = {.x = 641, .y = 400, .h = 33, .w = 107};
    SDL_Rect rec6 = {.x = 664, .y = 450, .h = 33, .w = 60};
    SDL_Rect rec7 = {.x = 667, .y = 530, .h = 50, .w = 50};
    SDL_Rect rec8 = {.x = 641, .y = 300, .h = 30, .w = 107};
    color[0] = createButton(rend, 0, "UI/GameWindow/Black.bmp", rec, EmptyFunction);
    color[1] = createButton(rend, 1, "UI/GameWindow/White.bmp", rec, EmptyFunction);
    save[0] = createButton(rend, 0, "UI/GameWindow/SaveBtn.bmp", rec7, EmptyFunction);
    save[1] = createButton(rend, 1, "UI/GameWindow/UnSaveBtn.bmp", rec7, EmptyFunction);
    widgets[0] = createButton(rend, 1, "UI/GameWindow/RestartBtn.bmp", rec1, RestartGameBtn);
    widgets[1] = createButton(rend, 4, "UI/GameWindow/SaveGameBtn.bmp", rec2, SaveGamesBtn);
    widgets[2] = createButton(rend, 2, "UI/GameWindow/LoadGameBtn.bmp", rec3, LoadGamesBtn);
    widgets[3] = createButton(rend, -1, "UI/GameWindow/UndoBtn.bmp", rec4, UndoGameBtn);
    widgets[4] = createButton(rend, 0, "UI/GameWindow/MainMenuBtn.bmp", rec5, MainMenuBtn);
    widgets[5] = createButton(rend, 3, "UI/GameWindow/QuitBtn.bmp", rec6, QuitGameBtn);
    widgets[6] = createButton(rend, 3, "UI/GameWindow/HelpBtn.bmp", rec8, HelpBtn);
    if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL || widgets[3] == NULL ||
        widgets[4] == NULL || widgets[5] == NULL|| widgets[6] == NULL) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_BTN, NULL);
        destroyAllResourceAndQuit(1);
    }
    // not Critical
    if (color[0] == NULL || color[1] == NULL) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, "No Current color Display due to an Error\n", NULL);
        printf("No Current color Display due to an Error\n");
    }
    // not Critical
    if (save[0] == NULL || save[1] == NULL) {
        printf(ERROR_WIDGET_BTN);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, "No Save Indicator Display due to an Error\n", NULL);
        printf("No Save Indicator Display due to an Error\n");
    }
}

/** param: void
 *  return: void
 *  action: Updating all the squares array according to the current global Board variable
 */

void initGuiBoard() {
    int i, j;
    int cnt = 0;
    char *type;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            type = getPieceImageByType(Board[i][j], GetSquareColor(i, j));
            PieceButton *tempBtn = (PieceButton *) Squares[cnt]->data;
            if (type == NULL) {
                tempBtn->imageflag = 0;
                tempBtn->texture = NULL;
            } else {
                tempBtn->imageflag = 1;
                SDL_Surface *surf = SDL_LoadBMP(type);
                if (surf == NULL) {
                    printf(ERROR_SDL_SURFACE, SDL_GetError());
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX,SDL_GetError(), NULL);
                    destroyAllResourceAndQuit(1);
                }
                SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surf);
                if (texture == NULL) {
                    printf(ERROR_SDL_TEXTURE, SDL_GetError());
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
                    destroyAllResourceAndQuit(1);
                }
                tempBtn->texture = texture;
            }
            cnt++;
        }
    }
}

/** param: void
 *  return: void
 *  action: set the Help window.
 */
void setHelpWindow(){
    SDL_Event e;
    Widget *GameWindow = createWindow("Help Window - How to play", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 660,
                              SDL_WINDOW_OPENGL, "UI/GameWindow/helpPage.bmp");
    if (GameWindow == NULL) {
        printf(ERROR_WIDGET_WINDOW);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, ERROR_WIDGET_WINDOW, NULL);
        return;
    }
    GameWindow->draw(GameWindow , ((Window*)GameWindow->data)->render);
    while (1) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
                    GameWindow->destroy(GameWindow);
                    return;
                }
            }
        }
    }
}

/** param: int index , int result[2]
 *  return: void
 *  action: return the global Board index <x,y> from the Squres array (rep the Gui board)
 */

void getBoardIndexesFromSquare(int index, int result[2]) {
    int i, j;
    int cnt = 0;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            if (cnt == index) {
                result[0] = i;
                result[1] = j;
                return;
            }
            cnt++;
        }
    }
}


/** param: int i , int j
 *  return: int
 *  action: return the the Squres array (rep the Gui board) index from the global Board index <x,y>
 */

int getGuiBoardIndexfromBoardIndexes(int i , int j) {
    int m , n;
    int cnt = -1;
    for (m = 0; m <= i; m++) {
        if(m == i) {
            for (n = 0; n <= j; n++) {
                cnt++;
            }
        } else{
            for (n = 0; n < BOARD_SIZE; n++) {
                cnt++;
            }
        }
    }
    return cnt;
}

/** param: none
 *  return: void
 *  action: reset all piece btn label flag
 */


void Resetlabel(){
    int i;
    for (i = 0 ; i < 64 ; i++){
        PieceButton * button = (PieceButton*)Squares[i]->data;
        button->labelflag = 0;
    }
}

/** param:  char type , char * color
 *  return: char *
 *  action: return the correct piece image address
 */

char *getPieceImageByType(char type, char *color) {
    switch (type) {
        case WHITE_PAWN:
            return strcmp(color, WHITE) == 0 ? WHITEPAWNW : WHITEPAWNB;
        case WHITE_BISHOP:
            return strcmp(color, WHITE) == 0 ? WHITEBISHOPW : WHITEBISHOPB;
        case WHITE_KNIGHT:
            return strcmp(color, WHITE) == 0 ? WHITEKNIGHTW : WHITEKNIGHTB;
        case WHITE_ROOK:
            return strcmp(color, WHITE) == 0 ? WHITEROOKW : WHITEROOKB;
        case WHITE_QUEEN:
            return strcmp(color, WHITE) == 0 ? WHITEQUEENW : WHITEQUEENB;
        case WHITE_KING:
            return strcmp(color, WHITE) == 0 ? WHITEKINGW : WHITEKINGB;
        case BLACK_PAWN:
            return strcmp(color, WHITE) == 0 ? BLACKPAWNW : BLACKPAWNB;
        case BLACK_BISHOP:
            return strcmp(color, WHITE) == 0 ? BLACKBISHOPW : BLACKBISHOPB;
        case BLACK_KNIGHT:
            return strcmp(color, WHITE) == 0 ? BLACKKNIGHTW : BLACKKNIGHTB;
        case BLACK_ROOK:
            return strcmp(color, WHITE) == 0 ? BLACKROOKW : BLACKROOKB;
        case BLACK_QUEEN:
            return strcmp(color, WHITE) == 0 ? BLACKQUEENW : BLACKQUEENB;
        case BLACK_KING:
            return strcmp(color, WHITE) == 0 ? BLACKKINGW : BLACKKINGB;
        default:
            return NULL;
    }
}

/** param: int i , int j
 *  return: char *
 *  action: if the square at board <i,j> is black return "black" else return "white"
 */
char *GetSquareColor(int i, int j) {
    return ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1)) ? WHITE : BLACK;
}


/** param: SDL_Rect *  rect ,int i , int j
 *  return: void
 *  action: update the rect to be the position on the window of the board square <i,j>
 */
void SetPosition(SDL_Rect* rect , int i , int j){
    // offsets from board panel
    int yOffset = 45;
    int xOffset = 43;

    // set position
    rect->y = (i * 64 + yOffset) + i;
    rect->x = (xOffset + j * 64);

    // Fix position (the squares are not on the same size...)
    if (j < 4)
        rect->y += 2;

    // Set square's size
    rect->w = 63;
    rect->h = 63;
}

/** param: none
 *  return: void
 *  action: draw to the window the curr player color
 */

void UpdateColor(){
    if(color[0]!=NULL && color[1]!=NULL){
        if (strcmp(CurrPlayerColor ,  BLACK) == 0) {
            color[0]->draw(color[0], rend);
        } else {
            color[1]->draw(color[1], rend);
        }
    }
}

/** param: none
 *  return: void
 *  action: draw to the window the save logo if curr game is saved else draw unsave logo
 */

void UpdateSaveBtn() {
    if (save[0] != NULL && save[1] != NULL) {
        if (isSave == 0) {
            save[1]->draw(save[1], rend);
        } else {
            save[0]->draw(save[0], rend);
        }
    }
}

/** param: none
 *  return: void
 *  action: rendering all the components for the game window
 */


void DisplayWindow() {
    MainPanel->draw(MainPanel, rend);
    UpdateColor();
    UpdateSaveBtn();
}

/** param:int i
 *  return: void
 *  action: release all memory for the game window and if i == 1 then also quit
 */


void destroyAllResourceAndQuit(int j) {
    int i;
    for (i = 0; i < 7; i++) {
        destroyWidget(widgets[i]);
        widgets[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        destroyWidget(color[i]);
        color[i] = NULL;
    }
    for (i = 0; i < 64; i++) {
        destroyWidget(Squares[i]);
        Squares[i] = NULL;
    }
    SDL_DestroyRenderer(rend);
    destroyWidget(GameWindow);
    GameWindow = NULL;
    destroyWidget(GamePanel);
    GamePanel = NULL;
    destroyWidget(MainPanel);
    MainPanel = NULL;
    if(j){
        QuitFailureGui();
    }
}



//// warning prints///


/** param: none
 *  return: void
 *  action: print tie massage to the user
 */


void PrintTieMassage() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Draw",
                             GUI_DRAW,
                             NULL);
}



/** param: int i
 *  return: void
 *  action: if i == 0 print white check else black check
 */

void PrintCheckMassage(int i) {
    if (i) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Check",
                                 GUI_BLACK_CHECK,
                                 NULL);
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Check",
                                 GUI_WHITE_CHECK,
                                 NULL);
    }

}


/** param: int i
 *  return: void
 *  action: if i == 0 print white mate else black check
 */
 void PrintMateMassage(int i) {
    if (i) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Mate",
                                 GUI_BLACK_MATE,
                                 NULL);
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                 "Mate",
                                 GUI_WHITE_MATE,
                                 NULL);

    }
}


/** param: none
 *  return: int
 *  action: pop a massage for the user to save the game;
 *  if "no" -> return 0
 *  if "yes" -> return 1
 *  if "cancel" -> return 2
 */
int PrintSaveMessage(){
    const SDL_MessageBoxButtonData buttons[] = {
            { /* .flags, .buttonid, .text */        0,                                       0, "no"},
            {                                       SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes"},
            {                                       SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel"},
    };
    const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_WARNING, /* .flags */
            NULL, /* .window */
            "Current Game is not saved", /* .title */
            "Would you wish to save the Current Game?", /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            NULL /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        return -1;
    }

    return buttons[buttonid].buttonid;
}


/// Buttons Handler ///


/** param: int i -> the index of the piece btn that been pressed
 *  return: void
 *  action: responsible for the regular move;
 *  after pressing the left btn on piece square all the legal squares to move to will be blue;
 *  pressing on a blue square will execute the move;
 */

void LeftClickHandler(int k) {
    restart = 0;
    int pair[2];
    int isMoveLegal;
    getBoardIndexesFromSquare(k, pair);
    cellCurrPress = InitCellByBoardIndex(pair[0], pair[1]);
    if (cellCurrPress == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        destroyAllResourceAndQuit(1);
    }
    PieceButton *button = (PieceButton *) Squares[k]->data;
    if (!end) {
        if (button->imageflag == 1) {
            if (button->pressflag == 0) {
                if (button->labelflag == 1) {
                    Resetlabel();
                    PieceButton *Prev = (PieceButton *) Prevpress->data;
                    Prev->pressflag = 0;
                    getBoardIndexesFromSquare(Prev->indexflag, pair);
                    cell *Cell = InitCellByBoardIndex(pair[0], pair[1]);
                    if (Cell == NULL) {
                        printf(MEMORY_ERROR);
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                        free(cellCurrPress);
                        destroyAllResourceAndQuit(1);
                    }
                    UpdateBoard(Board, Cell, cellCurrPress);
                    UpdateMovesHistory(Board, Cell, cellCurrPress);
                    free(Cell);
                    free(cellCurrPress);
                    currPress = Squares[k];
                    Prevpress = currPress;
                    DisplayWindow();
                    isSave = 0;
                    GameWindowAction = -1;
                } else {
                    if (strcmp(GetColorOfCell(Board, cellCurrPress), CurrPlayerColor) == 0 ||
                        strcmp(GetColorOfCell(Board, cellCurrPress), "") == 0) {
                        DisplayWindow();
                        if (Prevpress != NULL) {
                            PieceButton *Prev = (PieceButton *) Prevpress->data;
                            Prev->pressflag = 0;
                        }
                        button->pressflag = 1;
                        currPress = Squares[k];
                        Prevpress = currPress;
                        moves = GetPieceMoves(Board, cellCurrPress);
                        if (moves == NULL) {
                            printf(MEMORY_ERROR);
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                            free(cellCurrPress);
                            destroyAllResourceAndQuit(1);
                        }
                        move *curr = moves->listOfMoves;
                        Resetlabel();
                        while (curr != NULL) {
                            int i;
                            cell *copyofFrom = InitCellByBoardIndex(cellCurrPress->xBoard, cellCurrPress->yBoard);
                            if (copyofFrom == NULL) {
                                printf(MEMORY_ERROR);
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                                FreePieceMoves(moves);
                                destroyAllResourceAndQuit(1);
                            }
                            isMoveLegal = IsMoveLegal(Board, copyofFrom, curr->Cell);
                            if (isMoveLegal == 2) {
                                printf(MEMORY_ERROR);
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                                free(copyofFrom);
                                FreePieceMoves(moves);
                            }
                            if (isMoveLegal) {
                                i = getGuiBoardIndexfromBoardIndexes(curr->Cell->xBoard, curr->Cell->yBoard);
                                PieceButton *currBtn = (PieceButton *) Squares[i]->data;
                                currBtn->labelflag = 1;
                                SDL_SetRenderDrawColor(rend, 0, 0, 255, 120);
                                SDL_RenderFillRect(rend, &(currBtn->location));
                                // present changes to user
                                SDL_RenderPresent(rend);
                            }
                            free(copyofFrom);
                            curr = curr->next;
                        }
                        FreePieceMoves(moves);
                    }
                    else{
                        free(cellCurrPress);
                    }
                }
            }
        } else {
            if (button->labelflag == 1) {
                Resetlabel();
                PieceButton *Prev = (PieceButton *) Prevpress->data;
                Prev->pressflag = 0;
                getBoardIndexesFromSquare(Prev->indexflag, pair);
                cell *Cell = InitCellByBoardIndex(pair[0], pair[1]);
                if (Cell == NULL) {
                    printf(MEMORY_ERROR);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                    free(cellCurrPress);
                    destroyAllResourceAndQuit(1);
                }
                UpdateBoard(Board, Cell, cellCurrPress);
                UpdateMovesHistory(Board, Cell, cellCurrPress);
                free(Cell);
                free(cellCurrPress);
                currPress = Squares[k];
                Prevpress = currPress;
                DisplayWindow();
                isSave = 0;
                GameWindowAction = -1;
            }
            else{
                free(cellCurrPress);
            }
        }
    }
    else{
        free(cellCurrPress);
    }
}

/** param: int i -> the index of the piece btn that been pressed
 *  return: void
 *  action: responsible for the get_move;
 *  after pressing the right btn on piece square all the legal squares to move to will be marked;
 *  once you press the piece there will be 4 possible color for the squares.
 *  Red Square: if after the move, youre piece will be threatened by an opponent piece.
 *  Green Square: if there is an opponent piece at that square.
 *  Yellow Squre: if a square is green and red.
 *  Blue Square:if a square is neither green or red.
 */

void RightClickHandler(int k) {
    restart = 0;
    int pair[2];
    int isMoveLegal;
    getBoardIndexesFromSquare(k, pair);
    cellCurrPress = InitCellByBoardIndex(pair[0], pair[1]);
    if (cellCurrPress == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        destroyAllResourceAndQuit(1);
    }
    moves = GetPieceMoves(Board, cellCurrPress);
    if (moves == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(cellCurrPress);
        destroyAllResourceAndQuit(1);
    }
    PieceButton *button = (PieceButton *) Squares[k]->data;
    if (!end) {
        if (button->imageflag == 1) {
            int moveCapture;
            int moveThreatened;
            DisplayWindow();
            if (Prevpress != NULL) {
                PieceButton *Prev = (PieceButton *) Prevpress->data;
                Prev->pressflag = 0;
            }
            currPress = Squares[k];
            Prevpress = currPress;
            move *curr = moves->listOfMoves;
            Resetlabel();
            while (curr != NULL) {
                int i;
                i = getGuiBoardIndexfromBoardIndexes(curr->Cell->xBoard, curr->Cell->yBoard);
                cell *copyofFrom = InitCellByBoardIndex(cellCurrPress->xBoard, cellCurrPress->yBoard);
                if (copyofFrom == NULL) {
                    printf(MEMORY_ERROR);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                    FreePieceMoves(moves);
                    destroyAllResourceAndQuit(1);
                }
                isMoveLegal = IsMoveLegal(Board, copyofFrom, curr->Cell);
                moveThreatened = IsMoveThreatened(curr, cellCurrPress);
                moveCapture = IsMoveCapturePiece(curr);
                if (isMoveLegal == 2 || moveThreatened == 2) {
                    printf(MEMORY_ERROR);
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
                    free(copyofFrom);
                    FreePieceMoves(moves);
                    destroyAllResourceAndQuit(1);
                }
                if (isMoveLegal == 1) {
                    PieceButton *labelbutton = (PieceButton *) Squares[i]->data;
                    if (moveCapture) {
                        if (moveThreatened) {
                            SDL_SetRenderDrawColor(rend, 255, 255, 0, 120);
                            SDL_RenderFillRect(rend, &(labelbutton->location));
                            // present changes to user
                            SDL_RenderPresent(rend);

                        } else {
                            SDL_SetRenderDrawColor(rend, 0, 255, 0, 120);
                            SDL_RenderFillRect(rend, &(labelbutton->location));
                            // present changes to user
                            SDL_RenderPresent(rend);
                        }
                    } else if (moveThreatened) {
                        SDL_SetRenderDrawColor(rend, 255, 0, 0, 120);
                        SDL_RenderFillRect(rend, &(labelbutton->location));
                        // present changes to user
                        SDL_RenderPresent(rend);
                    } else {
                        SDL_SetRenderDrawColor(rend, 0, 0, 255, 120);
                        SDL_RenderFillRect(rend, &(labelbutton->location));
                        // present changes to user
                        SDL_RenderPresent(rend);
                    }
                }
                free(copyofFrom);
                curr = curr->next;
            }
        }
    }
    FreePieceMoves(moves);
}

/**
 *  for btn who are disabled always;
*/
void EmptyFunction(int i){

}

/** param: int i
 *  return: void
 *  action: if game not save pop massage ; after update the state variable to be 3;
*/

void QuitGameBtn(int i){
    int s;
    if (!isSave && !end) {
        s = PrintSaveMessage();
        if (s == 1) {
            restart = 0;
            GameWindowAction = i;
            SaveWindow();
        }
        if (s == 0) {
            restart = 0;
            GameWindowAction = i;
        }
    }
    else{
        restart = 0;
        GameWindowAction = i;
    }
}

/** param: int i
 *  return: void
 *  action: set restart to 1 and init Gui board; start a new game;
*/

void RestartGameBtn(int i) {
    Prevpress = NULL;
    CurrPlayerColor = WHITE;
    isSave = 0;
    end = 0;
    InitBoard();
    restart = i;
    GameWindowAction = i;
    SDL_Delay(100);
}

/** param: int i
 *  return: void
 *  action: Undo the game and continue the game; if no more moves to undo than btn is disabled
*/
void UndoGameBtn(int i) {
    end = 0;
    UndoLastMove();
    CurrPlayerColor = strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK;
    restart = 0;
    GameWindowAction = i;
}

/** param: int i
 *  return: void
 *  action: open the save game btn and update the save btn logo
*/
void SaveGamesBtn(int i) {
    if (!end) {
        SaveWindow();
        UpdateSaveBtn();
    }
}

/** param: int i
 *  return: void
 *  action: set the load game window
*/
void LoadGamesBtn(int i) {
    int res;
    res = LoadWindow();
    if(res == 4){
        FreeMoveHistory();
        InitMovesHistory();
        CurrPlayerColor = strcmp(CurrPlayerColor, BLACK) == 0 ? WHITE : BLACK;
        GameWindowAction = -1;
    }
    else{
        GameWindowAction = -2;
    }
    restart = 0;
}

/** param: int i
 *  return: void
 *  action: activate the help btn action
*/
void HelpBtn(int i) {
    setHelpWindow();
}

/** param: int i
 *  return: void
 *  action: if game not save pop massage ; after update the state variable to be 0;
*/
void MainMenuBtn(int i) {
    int s;
    if (!isSave && !end) {
        s = PrintSaveMessage();
        if (s == 1) {
            Prevpress = NULL;
            SaveWindow();
            isSave = 0;
            restart = 0;
            GameWindowAction = i;
        }
        else if(s == 0){
            Prevpress = NULL;
            isSave = 0;
            restart = 0;
            GameWindowAction = i;
        }
    }
    else {
        Prevpress = NULL;
        SetDefault();
        isSave = 0;
        restart = 0;
        GameWindowAction = i;
    }
}



