OBJS = GameState.o Gen.o GuiAux.o GuiGameModel.o GuiSaveLoadModel.o GuiSettingModel.o GuiStart.o MiniMax.o SaveLoadModel.o SettingState.o Undo.o Widgets.o Main.o

EXEC = chessprog
CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(EXEC)

$(EXEC): $(OBJS)
    gcc -o $(EXEC) $(OBJS) $(SDL_LIB)


Gen.o: Gen.h Gen.c
    gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c Gen.c

GameState.o: GameState.h GameState.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GameState.c

GuiAux.o: GuiAux.h GuiAux.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GuiAux.c

GuiGameModel.o: GuiGameModel.h GuiGameModel.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GuiGameModel.c

GuiSaveLoadModel.o: GuiSaveLoadModel.h GuiSaveLoadModel.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GuiSaveLoadModel.c

GuiSettingModel.o: GuiSettingModel.h GuiSettingModel.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GuiSettingModel.c

GuiStart.o: GuiStart.h GuiStart.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c GuiStart.c

Main.o: Main.h Main.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c Main.c

MiniMax.o: MiniMax.h MiniMax.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c MiniMax.c

SaveLoadModel.o: SaveLoadModel.h SaveLoadModel.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c SaveLoadModel.c

SettingState.o: SettingState.h SettingState.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c SettingState.c

Undo.o: Undo.h Undo.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c Undo.c

Widgets.o: Widgets.h Widgets.c
	gcc $(COMP_FLAG) $(SDL_COMP_FLAG) -c Widgets.c

clean:
    -rm -f $(EXEC) $(OBJS)