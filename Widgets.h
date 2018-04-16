#ifndef FINAL_PROJECT_WIDGETS_H
#define FINAL_PROJECT_WIDGETS_H

#include "GuiAux.h"

/**
 * Defining all the widgets structure for the Gui
 * In order to create different widgets we use the method we saw in class to mimic general programing in c
 */


typedef struct widget_t Widget; // Generic widget
typedef struct button_t Button; // simple btn that do one function
typedef struct toggle_button_t ToggleButton; // toggle btn that change the image of the btn (active or disabled)
typedef struct piece_button_t PieceButton; // represent the pieces btns of the gui
typedef struct window_t Window; // window widget
typedef struct panel_t Panel; // panel widget that contain other widgets


/**
 * A general structure for Widget
 */

struct widget_t {
    void (*draw)(Widget *, SDL_Renderer *); // draw the general widget
    void (*handleEvent)(Widget *, SDL_Event *); // handle event for the general widget
    void (*destroy)(Widget *); // release all memory of the general widget
    void *data; // the type of the widget-> simple btn, toggle btn , piece btn , window or panel
};


/**
 * This struc represent a Simple Btn Widget -> once press active the action
 */
struct button_t {
    int i; // the index of the btn in an array of btns or the value to update the global state
    SDL_Texture *texture; // the image of the btn
    SDL_Rect location; // the location on window to render
    void (*action)(int); // the function to activate after pressing the btn
    };

/**
 * This struc represent a toggle Btn Widget -> once press active the action or do nothing
 *  and set active or disabled image (depends on btn->i)
 */

struct toggle_button_t {
    int i; // active or disabled state
    int j; // the index of the btn in an array of btns or the value to update the global state
    SDL_Texture *Activetexture; // active image
    SDL_Texture *DeActivetexture; // disable image
    SDL_Rect location;// the location on window to render
    void (*action)(int );// the function to activate after pressing the btn
    void (*Active)(ToggleButton*); // toggle the i field.
};

/**
 * This struc represent the 64 Squares btn on the Gui board.
 * perhaps the name should be Square btn but we clarify that now that it represent a square in the Gui board.
 */


struct piece_button_t {
    int imageflag; // flag 1 iff the global board contain a piece at the same index as for the Gui board
    int indexflag; // the index of the Square btn at the Squares array (0-63)
    int pressflag; // flag 1 iff the user press on btn in the current turn
    int labelflag; // flag 1 iff the curr player can move the desired piece to that square
    SDL_Texture *texture; // the image of the square => consistent with the global board variable
    SDL_Rect location; // the place on the gui board
    void (*leftAction)(int index); // left mouse click function
    void (*rightAction)(int index); // right mouse click function
};


/**
 * This struc represent a window Widget
 */

struct window_t {
    SDL_Renderer *render; // render for the window
    SDL_Texture *texture; // background image
    SDL_Window *win; // the SDL_Window* of the Widget* Window
};

/**
 * This struc represent a Panel widget-> this is implementation of the UI tree.
 * the left one should come in draw before the right one
 */
struct panel_t{
    Widget * (*left); // left child array
    int numOfLeft; // num of elements in the left array
    Widget * (*right); // right child array
    int numOfRight; // num of elements in the right array
};


// Btn func
Widget *createButton(
        SDL_Renderer *renderer,
        int i,
        const char *image,
        SDL_Rect location,
        void (*action)(int)
        );

void destroyButton(Widget *src);

void handleButtonEvent(Widget *src, SDL_Event *event);

void drawButton(Widget *, SDL_Renderer *);

Widget *createToggleButton(
        SDL_Renderer *renderer,
        int i,
        int j,
        const char *image1,
        const char *image2,
        SDL_Rect location,
        void (*action)(int k)
        );

void destroyToggleButton(Widget *src);

void handleToggleButtonEvent(Widget *src, SDL_Event *event);

void drawToggleButton(Widget *, SDL_Renderer *);

Widget *createPieceButton(
        SDL_Renderer *renderer,
        int imageflag,
        int indexflag,
        int pressflag,
        int labelflag,
        const char *image,
        SDL_Rect location,
        void (*leftAction)(int index),
        void (*rightAction)(int index)
);

void destroyPieceButton(Widget *src);

void handlePieceButtonEvent(Widget *src, SDL_Event *event);

void drawPieceButton(Widget *, SDL_Renderer *);

//Window function
Widget *createWindow(char *title, int x , int y , int w , int h ,
                     Uint32 flag , char* BackgroundImage);

void destroyWindow(Widget *src);

void drawWindow(Widget * , SDL_Renderer *);

// Panel func
Widget *createPanel(Widget **left , int numOfLeft , Widget **right , int numOfRight);

void destroyPanel(Widget *src);

void drawPanel(Widget *, SDL_Renderer *);

void FreeAllBtns(Widget **arr, int m);

void destroyWidget(Widget *src);
#endif //FINAL_PROJECT_WIDGETS_H
