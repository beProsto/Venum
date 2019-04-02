#ifndef GLCALL_HPP_INCLUDED
#define GLCALL_HPP_INCLUDED

#include <iostream>
#include <GL/glew.h>

/* GLCall definition, calls OpenGL function and checks errors */
#define GLCall(x) ven::GLClearError(  ); x; assert(ven::GLLogCall(  ));

namespace ven
{

/* function clearing the errors */
void GLClearError(  )
{
    while( glGetError(  ) != GL_NO_ERROR );
}

/* function checking errors message */
bool GLLogCall(  )
{
    /* when glGetError returns true */
    while( GLenum Error = glGetError(  ) )
    {
        /* output the error */
        std::cerr << "[GL ERROR]" << "(" << Error << ")" << '\n';
        /* return false */
        return false;
    }
    /* return true if the error hasn't been seen */
    return true;
}


}

#endif // GLCALL_HPP_INCLUDED
