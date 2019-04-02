#ifndef MOUSECLASS_HPP_INCLUDED
#define MOUSECLASS_HPP_INCLUDED

#define MOUSE_POSITION_TYPE_GL_BASED 0
#define MOUSE_POSITION_TYPE_WINDOW_BASED 1

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_MIDDLE 1
#define MOUSE_BUTTON_RIGHT 3

namespace ven
{

class Mouse
{
    public:
        Mouse(  );
        Mouse( ven::Window* _windowPointer );
        ~Mouse(  );

        void AttachWindowPointer( ven::Window* _windowPointer );

        glm::vec2 GetPosition( GLenum MOUSE_POSITION_TYPE );
        void SetPosition( GLenum MOUSE_POSITION_TYPE, glm::vec2 _position );

        bool GetButtonDown( GLenum MOUSE_BUTTON );
        bool GetButtonUp( GLenum MOUSE_BUTTON );

    private:
        ven::Window* windowPointer;

};

}

#endif // MOUSECLASS_HPP_INCLUDED
