
#include "Array.h"
#include "Bitmap.h"
#include "Painter.h"
#include "Rect.h"
#include "Types.h"
#include "Vector.h"
#include "Window.h"
#include "WindowManager.h"
#include <SDL2/SDL.h>
#include <string>

int main()
{
    constexpr int width = 1024;
    constexpr int height = 720;

    TEST::Vector<int> test;
    test.pushBack(10);
    test.pushBack(20);
    test.pushBack(30);
    test.pushBack(40);
    test.popBack();
    test.popBack();
    test.pushBack(24);

    for (auto it : test) {
        std::cout << it << std::endl;
    }

    for (auto it = test.rbegin(); it != test.rend(); ++it) {
        std::cout << *it << std::endl;
    }

    ADS::Array<int, 5> array = { 10, 20, 30, 40 };
    for (auto it : array) {
        std::cout << it << std::endl;
    }

    TEST::Vector<int> vector = { 10, 20, 30, 60 };
    for (auto it : vector) {
        std::cout << it << std::endl;
    }

    const ADS::String title = "OS GUI Emulator";

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* sdlWindow = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(sdlWindow, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

    auto framebuffer = new uint32_t[width * height];
    auto framebufferBitmap = GUI::Bitmap::createFrom(GUI::BitmapFormat::RGBA32, { width, height }, (char*)framebuffer);

    GUI::Painter painter(*framebufferBitmap);

    GUI::Window win1 { GUI::IntRect { 10, 10, 800, 600 } };
    GUI::Window win2 { GUI::IntRect { 10, 10, 800, 600 } };

    GUI::WindowManager::the().add(win1);
    GUI::WindowManager::the().add(win2);

    bool leftMouseButtonDown = false;

    int startDownX = 0, startDownY = 0;

    bool quit = false;
    while (!quit) {
        framebufferBitmap->fill(GUI::Color { 255 });

        GUI::WindowManager::the().render(painter);

        SDL_UpdateTexture(texture, nullptr, framebufferBitmap->data(), width * sizeof(uint32_t));

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                GUI::WindowManager::the().onMouseUp(event.button.button, event.motion.x, event.motion.y);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    painter.drawLine(startDownX, startDownY, event.motion.x, event.motion.y, GUI::Color { 0xff, 0, 0, 0xff });
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                GUI::WindowManager::the().onMouseDown(event.button.button, event.motion.x, event.motion.y);
                if (event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    startDownX = event.motion.x;
                    startDownY = event.motion.y;
                }
            case SDL_MOUSEMOTION:
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                GUI::WindowManager::the().onMouseMove(mouseX, mouseY);
                if (leftMouseButtonDown) {
                    // framebufferBitmap->setPixel(mouseX, mouseY, GUI::Color { 0 });
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
