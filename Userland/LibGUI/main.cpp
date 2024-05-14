
#include "Array.h"
#include "Bitmap.h"
#include "Rect.h"
#include "Screen.h"
#include "Vector.h"
#include "Window.h"
#include "WindowManager.h"
#include <SDL2/SDL.h>

int main()
{
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

    GUI::Button* button = new GUI::Button([]() {
        std::cout << "Button clicked" << std::endl;
    });

    button->setRect(GUI::IntRect { 0, 0, 50, 20 });

    GUI::Window win1;
    win1.setRect(GUI::IntRect { 50, 50, 600, 400 });
    win1.setCentralWidget(*button);
    win1.setTitle("Windows 1");
    win1.show();

    GUI::Window win2;
    win2.setRect({ GUI::IntRect { 100, 100, 600, 400 } });
    win2.setTitle("Windows 2");
    win2.show();

    bool leftMouseButtonDown = false;

    int startDownX = 0, startDownY = 0;

    bool quit = false;
    while (!quit) {
        GUI::Screen::the().fill(GUI::Color { 255 });

        GUI::WindowManager::the().paint();

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
                    // painter.drawLine(startDownX, startDownY, event.motion.x, event.motion.y, GUI::Color { 0xff, 0, 0, 0xff });
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

        GUI::Screen::the().update();
    }
}
