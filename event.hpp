#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include "keyboard.hpp"
#include "mouse.hpp"

namespace ven
{

struct Event
{
    SDL_Event Event;
    ven::Keyboard Keyboard;
    ven::Mouse Mouse;
};


}

#endif // EVENT_HPP_INCLUDED
