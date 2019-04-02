#ifndef KEYBOARDCODE_HPP_INCLUDED
#define KEYBOARDCODE_HPP_INCLUDED

ven::Keyboard::Keyboard(  )
{}

ven::Keyboard::~Keyboard(  )
{}

bool ven::Keyboard::GetKeyState( Uint32 _scancode )
{
    const Uint8* keycode = SDL_GetKeyboardState( nullptr );

    if( keycode[_scancode] )
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // KEYBOARDCODE_HPP_INCLUDED
