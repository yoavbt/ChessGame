#include <stdlib.h>
#include <stdio.h>
#include "Widgets.h"

/**----- This Model is the implementation of all widget structures defined in Widget.h ----
 *
 */



/** param: SDL_Render , int, char* , rect , function
 *  return: Widget * if btn created or NULL else
 *  action: create the simple btn widget
 */

Widget *createButton(
        SDL_Renderer *renderer,
        int i,
        const char *image,
        SDL_Rect location,
        void (*action)(int k)
        ) {
    // allocate data
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        return NULL;
    }
    Button *data = (Button *) malloc(sizeof(Button));
    if (data == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(res);
        return NULL;
    }

    SDL_Surface *surface = SDL_LoadBMP(image);
    if (surface == NULL) {
        printf(ERROR_SDL_SURFACE , SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf(ERROR_SDL_TEXTURE , SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        SDL_FreeSurface(surface);
        return NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface);

    // store button & widget details
    data->i = i;
    data->texture = texture;
    data->location = location;
    data->action = action;
    res->destroy = destroyButton;
    res->draw = drawButton;
    res->handleEvent = handleButtonEvent;
    res->data = data;
    return res;
}

/** param: Widget * src
 *  return: void
 *  action: release all memory related to simple btn widget
 */
void destroyButton(Widget *src) {
    Button *button = (Button *) src->data;
    SDL_DestroyTexture(button->texture);
    free(button);
    free(src);
}

/** param: Widget *src , SDL_Event e
 *  return: void
 *  action: if the user press the btn with the left mouse key than activate the action function
 */
void handleButtonEvent(Widget *src, SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        Button *button = (Button *) src->data;
        SDL_Point point = {.x = e->button.x, .y = e->button.y};
        if (SDL_PointInRect(&point, &button->location)) {
            button->action(button->i);
        }
    }
}

/** param: Widget * src , SDL_Renderer rend
 *  return: void
 *  action: render the simple btn to the window
 */
void drawButton(Widget *src, SDL_Renderer *render) {
    Button *button = (Button *) src->data;
    SDL_RenderCopy(render, button->texture, NULL, &button->location);
    SDL_RenderPresent(render);
}

/** param: ToggleButton * btn
 *  return: void
 *  action: toggle the value btn->i;
 */

void Active(ToggleButton *btn) {
    btn->i = (btn->i == 1 ? 0 : 1);
}

/** param: SDL_Render , int, int , char1* ,char2* , rect , function
 *  return: Widget * if toggle btn created or NULL else
 *  action: create the toggle btn widget
 */

Widget *createToggleButton(
        SDL_Renderer *renderer,
        int i,
        int j,
        const char *image1,
        const char *image2,
        SDL_Rect location,
        void (*action)(int k)
) {
    // allocate data
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);

        return NULL;
    }
    ToggleButton *data = (ToggleButton*)malloc(sizeof(ToggleButton));
    if (data == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(res);
        return NULL;
    }

    SDL_Surface *surface = SDL_LoadBMP(image1);
    if (surface == NULL) {
        printf(ERROR_SDL_SURFACE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        printf(ERROR_SDL_TEXTURE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        SDL_FreeSurface(surface);
        free(res);
        free(data);
        return NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface);

    SDL_Surface *surface2 = SDL_LoadBMP(image2);

    if (surface2 == NULL) {
        printf(ERROR_SDL_SURFACE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        SDL_DestroyTexture(texture);
        free(res);
        free(data);
        return NULL;
    }

    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    if (texture2 == NULL) {
        printf(ERROR_SDL_TEXTURE, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        SDL_FreeSurface(surface);
        SDL_FreeSurface(surface2);
        return NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface2);

    // store button & widget details
    data->i = i;
    data->j = j;
    data->Activetexture = texture2;
    data->DeActivetexture = texture;
    data->location = location;
    data->Active = Active;
    data->action = action;
    res->destroy = destroyToggleButton;
    res->draw = drawToggleButton;
    res->handleEvent = handleToggleButtonEvent;
    res->data = data;
    return res;
}

/** param: Widget * src
 *  return: void
 *  action: release all memory related to toggle btn widget
 */

void destroyToggleButton(Widget *src) {
    ToggleButton *button = (ToggleButton *) src->data;
    SDL_DestroyTexture(button->DeActivetexture);
    SDL_DestroyTexture(button->Activetexture);
    free(button);
    free(src);
}

/** param: Widget *src , SDL_Event e
 *  return: void
 *  action: if the user press the btn with the left mouse key than activate the action function
 */
void handleToggleButtonEvent(Widget *src, SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT) {
        ToggleButton *button = (ToggleButton *) src->data;
        SDL_Point point = {.x = e->button.x, .y = e->button.y};
        if (SDL_PointInRect(&point, &button->location)) {
            button->action(button->j);
        }
    }
}

/** param: Widget * src , SDL_Renderer rend
 *  return: void
 *  action: render (image2 if btn->i ==1  else render image1) to window
 */

void drawToggleButton(Widget *src, SDL_Renderer *render) {
    ToggleButton *button = (ToggleButton *) src->data;
    if(button->i == 1){
        SDL_RenderCopy(render, button->Activetexture, NULL, &button->location);
        SDL_RenderPresent(render);
    }
    else{
        SDL_RenderCopy(render, button->DeActivetexture, NULL, &button->location);
        SDL_RenderPresent(render);
    }

}

/** param: SDL_Render , int, int , int ,int , char* , rect , function1 , function2
 *  return: Widget * if piece btn created or NULL else
 *  action: create the toggle btn widget
 */
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
) {
    // allocate data
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        return NULL;
    }
    PieceButton* data = (PieceButton *) malloc(sizeof(PieceButton));
    if (data == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(res);
        return NULL;
    }
    if(image!=NULL){
        // we use the surface as a temp var
        SDL_Surface *surface = SDL_LoadBMP(image);
        if (surface == NULL) {
            printf(ERROR_SDL_SURFACE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
            free(res);
            free(data);
            return NULL;
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            printf(ERROR_SDL_TEXTURE, SDL_GetError());
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
            SDL_FreeSurface(surface);
            free(res);
            free(data);
            return NULL;
        }

        // surface is not needed after texture is created
        data->texture = texture;
        SDL_FreeSurface(surface);
    }
    // store button & widget details
    data->imageflag = imageflag;// if contain an image
    data->indexflag = indexflag;// what is the index in array
    data->labelflag = labelflag;// if the btn was labeled
    data->pressflag = pressflag;// if the btn was pressed
    if (imageflag == 0) {
        data->texture = NULL;
    }
    data->location = location;
    data->leftAction = leftAction;
    data->rightAction = rightAction;
    res->destroy = destroyPieceButton;
    res->draw = drawPieceButton;
    res->handleEvent = handlePieceButtonEvent;
    res->data = data;
    return res;
}

/** param: Widget * src
 *  return: void
 *  action: release all memory related to piece btn widget
 */
void destroyPieceButton(Widget *src) {
    PieceButton *button = (PieceButton *) src->data;
    SDL_DestroyTexture(button->texture);
    free(button);
    free(src);
}

/** param: Widget *src , SDL_Event e
 *  return: void
 *  action: if the user press the btn with the left key mouse than activate function1
 *  else if the user press the btn with the right key mouse than activate function2
 */
void handlePieceButtonEvent(Widget *src, SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        PieceButton *button = (PieceButton *) src->data;
        SDL_Point point = {.x = e->button.x, .y = e->button.y};
        if (SDL_PointInRect(&point, &button->location)) {
            if(e->button.button == SDL_BUTTON_LEFT) {
                button->leftAction(button->indexflag);
            }
            else{
                button->rightAction(button->indexflag);
            }
        }
    }
}

/** param: Widget * src , SDL_Renderer rend
 *  return: void
 *  action: render to the screen only of button->imageflag ==1
 */
void drawPieceButton(Widget *src, SDL_Renderer *render) {
    PieceButton *button = (PieceButton *) src->data;
    if (button->imageflag == 1) {
        SDL_RenderCopy(render, button->texture, NULL, &button->location);
        SDL_RenderPresent(render);
    }
}


/** param: char * , int ,int ,int ,int ,Uint32 , char *
 *  return: Widget * if window widget created or NULL else
 *  action: create the window widget
 */


Widget *createWindow(char *title , int x, int y, int w, int h, Uint32 flag ,
                     char *BackgroundImage){
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        return NULL;
    }
    Window *data = (Window*) malloc(sizeof(Button));
    if (data == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(res);
        return NULL;
    }
    SDL_Window *win = SDL_CreateWindow(title, x, y, w, h, flag);
    if (win == NULL) {
        printf(ERROR_SDL_WINDOW, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        return NULL;
    }

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rend == NULL) {
        printf(ERROR_SDL_RENDER, SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        SDL_DestroyWindow(win);
        free(res);
        free(data);
        return NULL;
    }
    SDL_Surface *surface = SDL_LoadBMP(BackgroundImage);
    if (surface == NULL) {
        printf(ERROR_SDL_SURFACE , SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(rend, surface);
    if (texture == NULL) {
        printf(ERROR_SDL_TEXTURE , SDL_GetError());
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, SDL_GetError(), NULL);
        free(res);
        free(data);
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        return NULL;
    }

    // surface is not needed after texture is created
    SDL_FreeSurface(surface);

    data->win = win;
    data->render = rend;
    data->texture = texture;
    res->handleEvent = NULL;
    res->draw = drawWindow;
    res->destroy = destroyWindow;
    res->data = data;
    return res;
}

/** param: Widget * src
 *  return: void
 *  action: release all memory related to window widget
 */

void destroyWindow(Widget *src) {
    Window *win = (Window *) src->data;
    SDL_DestroyTexture(win->texture);
    SDL_DestroyRenderer(win->render);
    SDL_DestroyWindow(win->win);
    free(win);
    free(src);
}

/** param: Widget * src , SDL_Renderer rend
 *  return: void
 *  action: render the background pic to the window
 */
void drawWindow(Widget *src , SDL_Renderer * renderer) {
    Window *win = (Window *) src->data;
    SDL_RenderCopy(win->render,win->texture,NULL,NULL);
    SDL_RenderPresent(win->render);
}


/** param: Widget ** , int ,Widget ** , int
 *  return: Widget * if Panel created or NULL else
 *  action: create the panel using left array widget and right array widget
 */


Widget *createPanel(Widget **left, int numOfLeft, Widget **right, int numOfRight){
    Widget *res = (Widget *) malloc(sizeof(Widget));
    if (res == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        return NULL;
    }
    Panel *data = (Panel*) malloc(sizeof(Button));
    if (data == NULL) {
        printf(MEMORY_ERROR);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_BOX, MEMORY_ERROR, NULL);
        free(res);
        return NULL;
    }

    data->numOfLeft = numOfLeft;
    data->numOfRight = numOfRight;
    data->left = left;
    data->right = right;
    res->data = data;
    res->draw = drawPanel;
    res->destroy = destroyPanel;
    return res;

}

/** param: Widget * src
 *  return: void
 *  action: release all memory related to the right array widget and than to the left array widget
 */

void destroyPanel(Widget *src) {
    int i;
    Panel *panel = (Panel*)src->data;
    for(i = 0 ; i < panel->numOfRight ; i++){
            destroyWidget(panel->right[i]);
    }
    for (i = 0; i < panel->numOfLeft; i++) {
        destroyWidget(panel->left[i]);
    }
    free(panel);
    free(src);
}

/** param: Widget * src , SDL_Renderer rend
 *  return: void
 *  action: render first the left array widget and than the right array widget
 */
void drawPanel(Widget *src, SDL_Renderer *renderer) {
    int i;
    Panel *panel = (Panel *) src->data;
    for (i = 0; i < panel->numOfLeft; i++) {
        panel->left[i]->draw(panel->left[i] , renderer);
    }
    for (i = 0; i < panel->numOfRight; i++) {
        panel->right[i]->draw(panel->right[i] , renderer);
    }
}

/** param: Widget ** arr , int m
 *  return: void
 *  action: free all widgets memory in an array of widgets at size m
 */

void FreeAllBtns(Widget **arr, int m) {
    int i;
    for (i = 0; i < m; i++) {
        destroyWidget(arr[i]);
    }
}

/** param: Widget * src
 *  return: void
 *  action: SAVE MEMORY RELEASE : free a widget memory iff src!= NULL
 */
void destroyWidget(Widget *src) {
    if (src != NULL)
        src->destroy(src);
}
