#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdlib.h>
#include<string.h>
int len;

char data[1024*1024];

SDL_Surface*surface = NULL;
int cur = 0;

void loadimg() {
    IMG_Init(IMG_INIT_PNG);

    FILE *f = fopen("png/651-1.png","rb");
    int i = 0;
    char c = fgetc(f);
    while (!feof(f))
    {
        data[i++] = c;
        c = fgetc(f);
    }
    len = i;
    printf("len = %d\n",len);
    SDL_RWops * rw = SDL_RWFromMem(data, len);
    surface = IMG_Load_RW(rw,1);

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
            SDL_BlitSurface(surface,NULL,win_surf,NULL);
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
