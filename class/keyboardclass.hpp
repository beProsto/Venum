#ifndef KEYBOARDCLASS_HPP_INCLUDED
#define KEYBOARDCLASS_HPP_INCLUDED

namespace ven
{

class Keyboard
{
    public:
        Keyboard(  );
        ~Keyboard(  );

        bool GetKeyState( Uint32 _scancode );

};

}

#endif // KEYBOARDCLASS_HPP_INCLUDED
