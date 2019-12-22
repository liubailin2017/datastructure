#include<stdio.h>
#include<SDL2/SDL.h>
//#include<SDL2/SDL_image.h>

int main(int argc,char* agrv[]) {
    SDL_Event event; 
    int isq = 0;
    SDL_Init(SDL_INIT_EVERYTHING);
//    IMG_Init(IMG_INIT_PNG);

    SDL_Window *w = SDL_CreateWindow("Box",
                                    SDL_UNSUPPORTED,SDLK_UNDERSCORE,
                                    320,240,
                                    SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_ShowWindow(w);
	
    while (!isq)
    {
        while(SDL_PollEvent(&event)) {
            switch (event.type)
            {
            case SDL_KEYDOWN:
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
    }
    
    SDL_DestroyWindow(w);
    return 0;
}
