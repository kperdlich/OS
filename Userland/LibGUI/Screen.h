//
// Created by n3dry on 13.05.24.
//

#pragma once

#include "Bitmap.h"
#include "Types.h"
#include <SDL2/SDL.h>

namespace GUI {

class Screen final {
public:
    static Screen& instance()
    {
        static Screen instance;
        return instance;
    }

    void fill(GUI::Color color);
    void setPixel(int x, int y, GUI::Color color);

    void present();

    inline int width() const { return m_framebuffer->size().width(); }
    inline int height() const { return m_framebuffer->size().height(); };

private:
    Screen();
    ~Screen();

private:
    ADS::UniquePtr<Bitmap> m_framebuffer;
    SDL_Window* m_window { nullptr };
    SDL_Renderer* m_renderer { nullptr };
    SDL_Texture* m_texture { nullptr };
};

} // GUI
