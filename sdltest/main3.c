#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdlib.h>
#include<string.h>

char data[2*1024*1024]; /* 2M 缓存 */
int findex[11]; /* 索引 */


SDL_Surface* surface[10] = { NULL};

int cur = 0;

void loadimg() {
    IMG_Init(IMG_INIT_PNG);

    FILE *f = fopen("s.res","rb");
    
    int i = 0;
    int offset = 0;
    int len = 0;
    while (1)
    {
        findex[i++] =  offset;
        fread(&len,4,1,f);
        if( len == -1 ) break;
        fread(data+offset,len,1,f);
        offset += len;
    }

    for(int i = 0; i< 10; i++) {
        SDL_RWops * rw = SDL_RWFromMem(data+findex[i], findex[i+1]-findex[i]);
        surface[i] = IMG_Load_RW(rw,1); 

    }
    fclose(f);
}

int main(int argc,char* agrv[]) {
    SDL_Event event; 
    int isq = 0;
    SDL_Init(SDL_INIT_EVERYTHING);

    loadimg();

    SDL_Window *w = SDL_CreateWindow("Box",
                                    SDL_UNSUPPORTED,SDLK_UNDERSCORE,
                                    640,480,
                                    SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_ShowWindow(w);
	SDL_Surface *win_surf = SDL_GetWindowSurface(w);

    int tick = SDL_GetTicks();
    int flag = 0; 
    while (!isq)
    {
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                flag = 1;
                break;
            case SDL_QUIT: 
                isq =1;
                break;
            default:
                break;
            }
        }

        if(SDL_GetTicks() - tick > 200) {
            SDL_Rect rect = { 1,1, 640,480}; 
            SDL_FillRect(win_surf,&rect,0xff00ff00);           
            SDL_BlitSurface(surface[cur],NULL,win_surf,NULL);
            SDL_UpdateWindowSurface(w);
            if(flag) {
                cur ++;
            }
            if(cur==10) {
                flag = 0;
                cur = 0;
            }
            tick = SDL_GetTicks();
        }
        SDL_Delay(10);
    }
    
    SDL_DestroyWindow(w);
    return 0;
}
