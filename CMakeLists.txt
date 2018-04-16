cmake_minimum_required(VERSION 3.7)
project(Final_Project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Gen.c
        Gen.h
        GameState.c GameState.h SettingState.c SettingState.h Main.c Main.h SaveLoadModel.c SaveLoadModel.h GuiStart.c GuiStart.h Widgets.c Widgets.h GuiSettingModel.c GuiSettingModel.h GuiAux.c GuiAux.h GuiSaveLoadModel.c GuiSaveLoadModel.h GuiGameModel.c GuiGameModel.h Undo.c Undo.h MiniMax.c MiniMax.h)

add_executable(Final_Project ${SOURCE_FILES})


include_directories(${PROJECT_SOURCE_DIR}/include/SDL2)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} mingw32 SDL2main SDL2)
