#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <string>
#include "gl/glcall.hpp"

namespace ven
{

class Shader
{
    public:
        Shader( unsigned int _type, const std::string& _code )
        {
            this->Type = _type;
            this->Code = _code;

            GLCall( this->RendererID = glCreateShader( this->Type ) );
            const char * src = this->Code.c_str(  );
            GLCall( glShaderSource( this->RendererID, 1, &src, nullptr ); )
        }

        ~Shader(  )
        { GLCall( glDeleteShader( this->RendererID ) ); }


        inline unsigned int GetRendererID(  ) const
        { return this->RendererID; }

        inline std::string GetCode(  ) const
        { return this->Code; }

    private:
        unsigned int RendererID;
        unsigned int Type;
        std::string  Code;

};


}

#endif // SHADER_HPP_INCLUDED
