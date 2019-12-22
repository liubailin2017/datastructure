#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#include<string.h>

SDL_Surface*surface[15];
int cur = 0;

void loadimg() {
    IMG_Init(IMG_INIT_PNG);
    for(int i = 1; i<= 10;i++) {
        char fname[128] = {"png/651-"};
        if(i <= 9) {
            fname[8]='0'+i;
            fname[9]='\0';
        }else if(i < 20) {
            fname[8]= '1';
            fname[9]='0'+i-10;
            fname[10]= '\0';
        }
        strcat(fname,".png");
        surface[i-1] = IMG_Load(fname);
        if(surface[i-1] == NULL) {
            printf("%d load failure\n",i);
        }
    }
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
