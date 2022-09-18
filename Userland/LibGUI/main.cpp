
#include "Bitmap.h"
#include "Painter.h"
#include "Types.h"
#include "Window.h"
#include <SDL2/SDL.h>
#include <string>

int main()
{
    constexpr int width = 1024;
    constexpr int height = 720;

    const ADS::String title = "OS LibGui Emulator";

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* sdlWindow = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

    auto framebuffer = new uint32_t[width * height];
    ADS::memset(framebuffer, 255, width * height * sizeof(uint32_t));

    auto framebufferBitmap = GUI::Bitmap::createFrom(GUI::BitmapFormat::RGBA32, { width, height }, (char*)framebuffer);

    auto painter = new GUI::Painter(framebufferBitmap);

    auto window = new GUI::Window({ 800, 600 });

    bool leftMouseButtonDown = false;

    int startDownX = 0, startDownY = 0;

    bool quit = false;
    while (!quit) {
        framebufferBitmap->clear(GUI::Color { 255 });

        painter->drawLine(900, 500, 900, 0, GUI::Color{0xff, 0, 0, 0xff});
        painter->drawLine(1000, 500, 700, 500, GUI::Color{0xff, 0, 0, 0xff});

        window->render(*painter);

        SDL_UpdateTexture(texture, nullptr, framebufferBitmap->data(), width * sizeof(uint32_t));

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    painter->drawLine(startDownX, startDownY, event.motion.x, event.motion.y, GUI::Color { 0xff, 0, 0, 0xff });
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    startDownX = event.motion.x;
                    startDownY = event.motion.y;
                }
            case SDL_MOUSEMOTION:
                if (leftMouseButtonDown) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    //framebufferBitmap->setPixel(mouseX, mouseY, GUI::Color { 0 });
                }
                break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    delete[] framebuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}
