#ifndef FINAL_PROJECT_GUIGAMEMODEL_H
#define FINAL_PROJECT_GUIGAMEMODEL_H

#include "GuiSaveLoadModel.h"
#include "GameState.h"


#define GUI_WHITE_MATE "Checkmate! white player wins the game"
#define GUI_BLACK_MATE "Checkmate! black player wins the game"
#define GUI_WHITE_CHECK "Check: white king is threatened\n"
#define GUI_BLACK_CHECK "Check: black king is threatened\n"
#define GUI_DRAW "The game ends in a draw"

#define WHITEPAWNW  "UI/GameWindow/whitePawnW.bmp"
#define WHITEBISHOPW  "UI/GameWindow/whiteBishopW.bmp"
#define WHITEKNIGHTW  "UI/GameWindow/whiteKnightW.bmp"
#define WHITEROOKW  "UI/GameWindow/whiteRookW.bmp"
#define WHITEQUEENW  "UI/GameWindow/whiteQueenW.bmp"
#define WHITEKINGW  "UI/GameWindow/whiteKingW.bmp"
#define BLACKPAWNW  "UI/GameWindow/blackPawnW.bmp"
#define BLACKBISHOPW  "UI/GameWindow/blackBishopW.bmp"
#define BLACKKNIGHTW  "UI/GameWindow/blackKnightW.bmp"
#define BLACKROOKW "UI/GameWindow/blackRookW.bmp"
#define BLACKQUEENW  "UI/GameWindow/blackQueenW.bmp"
#define BLACKKINGW  "UI/GameWindow/blackKingW.bmp"
#define WHITEPAWNB  "UI/GameWindow/whitePawnB.bmp"
#define WHITEBISHOPB  "UI/GameWindow/whiteBishopB.bmp"
#define WHITEKNIGHTB  "UI/GameWindow/whiteKnightB.bmp"
#define WHITEROOKB "UI/GameWindow/whiteRookB.bmp"
#define WHITEQUEENB  "UI/GameWindow/whiteQueenB.bmp"
#define WHITEKINGB  "UI/GameWindow/whiteKingB.bmp"
#define BLACKPAWNB  "UI/GameWindow/blackPawnB.bmp"
#define BLACKBISHOPB  "UI/GameWindow/blackBishopB.bmp"
#define BLACKKNIGHTB  "UI/GameWindow/blackKnightB.bmp"
#define BLACKROOKB "UI/GameWindow/blackRookB.bmp"
#define BLACKQUEENB  "UI/GameWindow/blackQueenB.bmp"
#define BLACKKINGB  "UI/GameWindow/blackKingB.bmp"

extern int isSave;

int StartGameWindow();

void QuitGameBtn(int i);

void SetBoard();

void SetPosition(SDL_Rect *rect, int i, int j);

char *GetSquareColor(int i, int j);

char *getPieceImageByType(char type, char *color);

void LeftClickHandler(int k);

void RightClickHandler(int k);

int SetGameWindow();

void InitBtn();

void UpdateColor();

void QuitGameBtn(int i);

void RestartGameBtn(int i);

void UndoGameBtn(int i);

void UndoGameBtn(int i);

void SaveGamesBtn(int i);

void LoadGamesBtn(int i);

void MainMenuBtn(int i);

void EmptyFunction(int i);

void HelpBtn(int i);

int GuiUserTurn();

int GuiComputerTurn();

void getBoardIndexesFromSquare(int index, int result[2]);

int getGuiBoardIndexfromBoardIndexes(int i, int j);

void DisplayWindow();

void Resetlabel();

void initGuiBoard();

void ActiveWidgets(SDL_Event e);

void ActiveSquares(SDL_Event e);

void destroyAllResourceAndQuit(int j);

void PrintTieMassage();

void PrintCheckMassage(int i);

void PrintMateMassage(int i);

void setHelpWindow();
#endif //FINAL_PROJECT_GUIGAMEMODEL_H
