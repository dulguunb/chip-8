#include <iostream>
#include <fstream>
#include <string>
#include "chip8.h"
#include <SDL.h>
#include "res_path.h"
chip8 cpu;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}


/*SDL_Texture *loadTexture(const std::string &file,SDL_Renderer *ren){
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    if (loadedImage != nullptr){
        texture = SDL_CreateTextureFromSurface(ren,loadedImage);
        SDL_FreeSurface(loadedImage);
        if(texture == nullptr){
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }

    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return tex;
}*/
int main(){


    // SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    // if (win == nullptr){
    //     std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    //     SDL_Quit();
    //     return 1;
    // }

    // SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // if (ren == nullptr){
    //     SDL_DestroyWindow(win);
    //     std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    //     SDL_Quit();
    //     return 1;
    // }

    // std::string imagePath = "hello.bmp";
    // SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    // if (bmp == nullptr){
    //     SDL_DestroyRenderer(ren);
    //     SDL_DestroyWindow(win);
    //     std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    //     SDL_Quit();
    //     return 1;
    // }

   
    // //A sleepy rendering loop, wait for 3 seconds and render and present the screen each time

    // for (int i=0;i<10;i++){
    //     //First clear the renderer

    //     SDL_RenderClear(ren);
    //     //Draw the texture

    //     SDL_RenderCopy(ren, tex, NULL, NULL);
    //     //Update the screen

    //     SDL_RenderPresent(ren);
    //     //Take a quick break after all that hard work

    //     SDL_Delay(1000);
    // }
    // SDL_DestroyTexture(tex);
    // SDL_DestroyRenderer(ren);
    // SDL_DestroyWindow(win);
    // SDL_Quit();
    // SDL_Quit();
    std::string fileName = "roms/hello.rom";
    cpu.loadGameFromFile(fileName);
    while(!cpu.finished()){
        cpu.emulateCycle();
    }
    cpu.dumpRegisters();
    cpu.dumpMemory();
}
