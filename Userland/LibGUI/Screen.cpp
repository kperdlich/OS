//
// Created by n3dry on 13.05.24.
//

#include "Screen.h"

namespace GUI {

static const int internalWidth = 1024;
static const int internalHeight = 720;

Screen::Screen()
{
    const ADS::String title = "OS GUI Emulator";

    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, internalWidth, internalHeight, 0);

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    m_texture = SDL_CreateTexture(m_renderer,
        SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, internalWidth, internalHeight);

    auto framebuffer = new uint32_t[internalWidth * internalHeight];
    m_framebuffer = GUI::Bitmap::createFrom(GUI::BitmapFormat::RGBA32, { internalWidth, internalHeight }, (char*)framebuffer);
}

Screen::~Screen()
{
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Screen::fill(GUI::Color color)
{
    m_framebuffer->fill(color);
}

void Screen::setPixel(int x, int y, GUI::Color color)
{
    m_framebuffer->setPixel(x, y, color);
}

void Screen::update()
{
    SDL_UpdateTexture(m_texture, nullptr, m_framebuffer->data(), m_framebuffer->size().width() * sizeof(uint32_t));

    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

} // GUI