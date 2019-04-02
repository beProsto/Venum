#ifndef GLCALL_HPP_INCLUDED
#define GLCALL_HPP_INCLUDED

#include <iostream>
#include <GL/glew.h>

#define GLCall(x) ven::GLClearError(  ); x; assert(ven::GLLogCall(  ));

namespace ven
{

void GLClearError(  )
{
    while( glGetError(  ) != GL_NO_ERROR );
}

bool GLLogCall(  )
{
    while( GLenum Error = glGetError(  ) )
    {
        std::cerr << "[GL ERROR]" << "(" << Error << ")" << '\n';
        return false;
    }
    return true;
}


}

#endif // GLCALL_HPP_INCLUDED
