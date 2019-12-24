#include<stdio.h>
#include<SDL2/SDL.h>
#include<stdlib.h> 

#define CNT_W 20
#define CNT_H 20

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
    scene->bg = 0x0000ff00;
    scene->foodColor = 0x00ff00ff;
    scene->snakeColor = 0x00000000;
}

void draw(struct Scene *scene,SDL_Surface *surface) {
    int w = surface->w;
    int h = surface->h;
    int cw = w/scene->cntw;
    int ch = h/scene->cnth;
    SDL_Rect rect = {0,0,w,h};
    SDL_FillRect(surface,&rect,scene->bg);
    struct Node *it = scene->snake;
    while (it)
    {
        rect.x = cw * it->x;
        rect.y = ch * it->y;
        rect.w = cw;
        rect.h = ch;
        SDL_Rect tmp = rect;
        tmp.x++;
        tmp.y++;
        tmp.w-=2;
        tmp.h-=2;

        SDL_FillRect(surface,&rect,scene->snakeColor);
        SDL_FillRect(surface,&tmp,0xffffffff);
        it = it->next;
    }
    
    rect.x = scene->food->x * cw;
    rect.y = scene->food->y * ch;
    rect.w = cw;
    rect.h = ch;
    SDL_FillRect(surface,&rect,scene->foodColor);    
}


void move(struct Scene *scene,enum DIRECT dir) {
    struct Node *head = scene->snake;
    struct Node *rear = head->next;
    struct Node *prear = head;

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

int isStrikeWall(struct Scene *scene) {
    int x = scene->snake->x;
    int y = scene->snake->y;
    int w = scene->cntw;
    int h = scene->cnth;
    if(x >= 0 && x < w && y >= 0 && y < h) {
        return 0;
    }else {
        return 1;
    }
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
    SDL_Event event; 
    int isq = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
    struct Scene scene;
  

    SDL_Window *w = SDL_CreateWindow("Box",
                                    SDL_UNSUPPORTED,SDLK_UNDERSCORE,
                                    640,640,
                                    SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Surface *win_surface = SDL_GetWindowSurface(w);
    int tick = SDL_GetTicks();
    int isgameover = 0;
    while (!isq)
    {
/* isq */
    
    init(&scene);
    isgameover = 0;
    while (!isgameover && !isq)
    {
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_KEYDOWN:
            
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                    if(scene.curDir != S_DOWN) {
                        scene.curDir = S_UP;  
                        move(&scene,scene.curDir);                 
                    }
                        break;
                    case SDLK_RIGHT:
                    if(scene.curDir != S_LEFT) {
                        scene.curDir = S_RIGHT; 
                        move(&scene,scene.curDir);                 
                    }
                        break;
                    case SDLK_DOWN:
                    if(scene.curDir != S_UP) {
                        scene.curDir = S_DOWN;
                        move(&scene,scene.curDir);                    
                    }
                        break;
                    case SDLK_LEFT:
                    if(scene.curDir != S_RIGHT) {
                        scene.curDir = S_LEFT;
                        move(&scene,scene.curDir);                 
                    }
                    break;
                }
                if(isStrikeWall(&scene)) {
                    printf("strike\n");
                }
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

        
        if(SDL_GetTicks() - tick > delay) {
            move(&scene,scene.curDir);
            tick = SDL_GetTicks();
        }
        
        if(isStrikeWall(&scene)) {
            isgameover = 1;
        }

        draw(&scene,win_surface);
        SDL_UpdateWindowSurface(w);
        SDL_Delay(30);
    }

    Destory(&scene);
/* isq */
    }
    SDL_DestroyWindow(w);
    return 0;
}

/* todo
1.控制方向
2.吃到食物增长， 计分
3.游戏结束，重新开始(包含销毁蛇)
*/