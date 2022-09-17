
#include "Bitmap.h"
#include "Painter.h"
#include "Types.h"
#include <SDL2/SDL.h>
#include <string>

int main()
{
    constexpr int width = 1024;
    constexpr int height = 720;

    const ADS::String title = "OS LibGui Emulator";

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

    auto framebuffer = new uint32_t[width * height];
    ADS::memset(framebuffer, 255, width * height * sizeof(uint32_t));

    auto bitmap = GUI::Bitmap::createFrom(GUI::BitmapFormat::RGBA32, { width, height }, (char*)framebuffer);

    auto painter = new GUI::Painter(bitmap);

    painter->drawRectangle(0, 0, { 800, 600 }, Gui::Color(0x00, 0, 0, 0xff));
    painter->drawRectangle(0, 0, { 800, 20 }, Gui::Color(0, 0x00, 0xff, 0xff));
    painter->drawRectangle(780, 0, { 20, 20 }, Gui::Color(0xff, 0, 0, 0xff));

    bool leftMouseButtonDown = false;

    bool quit = false;
    while (!quit) {
        SDL_UpdateTexture(texture, nullptr, bitmap->data(), width * sizeof(uint32_t));

        SDL_Event event;
        SDL_WaitEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                leftMouseButtonDown = true;
        case SDL_MOUSEMOTION:
            if (leftMouseButtonDown) {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;
                bitmap->setPixel(mouseX, mouseY, Gui::Color { 0 });
            }
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    delete[] framebuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
