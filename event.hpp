#ifndef EVENT_HPP_INCLUDED
#define EVENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include "keyboard.hpp"
#include "mouse.hpp"

namespace ven
{

/* Easy to use Event struct containing: */
 /* SDL_Event named "Event"   */
 /* Keyboard named "Keyboard" */
 /* Mouse named "Mouse"       */
struct Event
{
    SDL_Event Event;
    ven::Keyboard Keyboard;
    ven::Mouse Mouse;
};


}

#endif // EVENT_HPP_INCLUDED
