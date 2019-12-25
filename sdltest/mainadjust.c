#include<stdio.h>
#include<SDL2/SDL.h>
#include<stdlib.h> 

#define CNT_W 32
#define CNT_H 24

#define delay 300

enum DIRECT{
    S_UP = 0,S_RIGHT,S_DOWN,S_LEFT
};

struct Food {
    int x;
    int y;
    int flag;
};

struct Node {
    int x;
    int y;
    struct Node *next;
};

struct Scene {
    struct Node* snake;
    enum DIRECT curDir;
    struct Food* food;
    int score;
    int cntw;
    int cnth;
    Uint32 bg;
    Uint32 snakeColor;
    Uint32 foodColor;
};

void resetFood(struct Scene *scene) {
    struct Food* food = scene->food;
    int maxw = scene ->cntw;
    int maxh = scene ->cnth;
    int x = rand() % maxw;
    int y = rand() % maxh;
    food ->x = x;
    food ->y = y;
    food->flag = 0;
}

void init(struct Scene *scene) {
    if(scene == NULL) {
        printf("init err\n");
        exit(1);
    }
    scene->cnth = CNT_H;
    scene->cntw = CNT_W;
    scene->curDir = S_LEFT;
    scene->score = 0;
    struct Node *head = (struct Node *) malloc(sizeof(struct Node));
    head->x = 10;
    head->y = 10;
    head->next = NULL;
    
    struct Node *tmp = head;
    for(int i = 0; i < 3; i++) {
        tmp->next = malloc(sizeof(struct Node));
        tmp->next->next =NULL;
        tmp->next->x = tmp->x+1;
        tmp->next->y = tmp->y;
        tmp= tmp->next;
    }
    scene->snake = head;
    scene->food = (struct Food*) malloc(sizeof(struct Food));
    resetFood(scene);
    scene->bg = 0x00110300;
    scene->foodColor = 0x00777777;
    scene->snakeColor = 0x00665566;
}

void draw(struct Scene *scene,SDL_Surface *surface) {
    int w = surface->w;
    int h = surface->h;
    int cw = w/scene->cntw;
    int ch = h/scene->cnth;
    SDL_Rect rect = {0,0,w,h};
    SDL_Rect rect2 = {1,1,w-2,h-2};
    SDL_FillRect(surface,&rect,0x00000000);
    SDL_FillRect(surface,&rect2,scene->bg);
    struct Node *it = scene->snake;
    int ish = 1;
    while (it)
    {
        rect.x = cw * it->x;
        rect.y = ch * it->y;
        rect.w = cw;
        rect.h = ch;
        SDL_Rect tmp = rect;
        tmp.x ++;
        tmp.y ++;
        tmp.w-=2;
        tmp.h-=2;
        if(ish) {
            ish = 0;
            SDL_FillRect(surface,&rect,0x00000000);
        }else{
            SDL_FillRect(surface,&rect,0x66666666);
        }
        SDL_FillRect(surface,&tmp,scene->snakeColor);
        it = it->next;
    }
    
    rect.x = scene->food->x * cw;
    rect.y = scene->food->y * ch;
    rect.w = cw;
    rect.h = ch;
    SDL_FillRect(surface,&rect,scene->foodColor);    
}

void move(struct Scene *scene) {
    struct Node *head = scene->snake;
    struct Node *rear = head->next;
    struct Node *prear = head;
    
    enum DIRECT dir = scene->curDir;
 

    while(prear->next->next) {
        prear= prear->next;
    }

    if(scene->food->x == scene->snake->x && scene->food->y == scene->snake->y) {
        rear = malloc(sizeof(struct Node));
        resetFood(scene);
        scene->score ++;
        printf("%d\n",scene->score);
    }else {
        rear = prear->next;
        prear->next = NULL;
    }

    switch (dir)
    {
    case S_UP:
        rear->x = head->x;
        rear->y = head->y-1;
        rear->next = head;
        break;
    case S_RIGHT:
        rear->x = head->x+1;
        rear->y = head->y;
        rear->next = head;
        break;
    case S_DOWN:
        rear->x = head->x;
        rear->y = head->y+1;
        rear->next = head;
        break;
    case S_LEFT:
        rear->x = head->x-1;
        rear->y = head->y;
        rear->next = head;
        break;
    default:
        break;
    }

    scene->snake = rear;
}

void chageDirect(struct Scene *scene,enum DIRECT dir) {
   switch (dir)
    {
    case S_UP:
        if(scene->curDir == S_DOWN) return ;
        break;
    case S_RIGHT:
        if(scene->curDir == S_LEFT) return ;
        break;
    case S_DOWN:
        if(scene->curDir == S_UP) return ;
        break;
    case S_LEFT:
        if(scene->curDir == S_RIGHT) return ;
        break;
    }
    scene ->curDir = dir;
    move(scene);
}

int isStrikeWall(struct Scene *scene) {
    int x = scene->snake->x;
    int y = scene->snake->y;
    int w = scene->cntw;
    int h = scene->cnth;

    int isEatSelf = 0, isStrike = 0;

    if(x >= 0 && x < w && y >= 0 && y < h) {
        isStrike = 0;
    }else {
        isStrike = 1;
    }
    
    struct Node *it = scene->snake->next;
    int ish = 1;
    while (it)
    {
        if( x == it->x && y == it->y) {
            isEatSelf = 1;
            break;
        }
        it = it->next;
    }

    return isEatSelf || isStrike;
}

void Destory(struct Scene* scene) {
    struct Node *tmp = scene->snake;

    while(tmp) {
        struct Node *tmp2;
        tmp2 = tmp->next;
        free(tmp);
        tmp = tmp2;
    }

    free(scene->food);
    scene->snake = NULL;
    scene->food = NULL;
}

int main(int argc,char* agrv[]) {
    char title[256] = {0};
    SDL_Event event; 
    int isq = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    struct Scene scene;
  

    SDL_Window *w = SDL_CreateWindow("贪食蛇",
                                    SDL_UNSUPPORTED,SDLK_UNDERSCORE,
                                    640,480,
                                    SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Surface *win_surface = SDL_GetWindowSurface(w);
    int tick = SDL_GetTicks();
    int isgameover = 0; /*  0正在游戏；1表示游戏结束，但是还没有重新开始; 2重新开始 */ 
    while (!isq)
    {
/* isq */
    init(&scene);
    isgameover = 0;
    while (isgameover != 2 && !isq)
    {
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_KEYDOWN:
            if(isgameover == 1 || isgameover == 2) {
                isgameover = 2; 
            }else { /* ------------------- */
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        chageDirect(&scene,S_UP);
                        tick = SDL_GetTicks();                 
                        break;
                    case SDLK_RIGHT:
                        chageDirect(&scene,S_RIGHT);
                        tick = SDL_GetTicks();              
                        break;
                    case SDLK_DOWN:
                        chageDirect(&scene,S_DOWN); 
                        tick = SDL_GetTicks();                   
                        break;
                    case SDLK_LEFT:
                        chageDirect(&scene,S_LEFT);
                        tick = SDL_GetTicks();                 
                    break;
                }
            }/* ---------------------*/

                break;
            case SDL_KEYUP:
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_QUIT: 
                isq =1;
                break;
            default:
                break;
            }
        }

        if(!isgameover && SDL_GetTicks() - tick > delay) {
            move(&scene);
            tick = SDL_GetTicks();
        }
        
        if(!isgameover && isStrikeWall(&scene)) {
             isgameover = 1;
        }
        
        draw(&scene,win_surface);
        SDL_UpdateWindowSurface(w);
        sprintf(title,"贪食蛇 分数 : %5d",scene.score);
        SDL_SetWindowTitle(w,title);
        SDL_Delay(30);
    }
    Destory(&scene);
/* isq */
    }
    SDL_DestroyWindow(w);
    return 0;
}

