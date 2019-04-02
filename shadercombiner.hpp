#ifndef SHADERCOMBINER_HPP_INCLUDED
#define SHADERCOMBINER_HPP_INCLUDED

#include <string>
#include "gl/glcall.hpp"
#include "shader.hpp"

namespace ven
{

class ShaderCombiner
{
    public:
        ShaderCombiner(  )
        { GLCall( this->RendererID = glCreateProgram(  ) ); }

        ~ShaderCombiner(  )
        { GLCall( glDeleteProgram( this->RendererID ) ); }

        void AttachShader( const Shader& _shader )
        {
            GLCall( glAttachShader( this->RendererID, _shader.GetRendererID() ) );
        }

        void Compile(  )
        {
            GLCall( glLinkProgram( this->RendererID ) );
            GLCall( glValidateProgram( this->RendererID ) );
        }

        void Bind(  ) const
        {
            glUseProgram( this->RendererID );
        }

        void UnBind(  ) const
        {
            glUseProgram( 0 );
        }

        inline unsigned int GetRendererID(  ) const
        { return this->RendererID; }

    private:
        unsigned int RendererID;

};


}

#endif // SHADERCOMBINER_HPP_INCLUDED
