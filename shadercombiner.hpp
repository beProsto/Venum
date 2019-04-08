#ifndef SHADERCOMBINER_HPP_INCLUDED
#define SHADERCOMBINER_HPP_INCLUDED

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

        void SetUniform1f( const std::string& _name, float _var )
        {
            (*this).Bind(  );
            GLCall( glUniform1fv( glGetUniformLocation( this->RendererID, _name.c_str(  ) ), 1, &_var ) );
            (*this).UnBind(  );
        }

        void SetUniform2f( const std::string& _name, const glm::vec2& _var )
        {
            (*this).Bind(  );
            GLCall( glUniform2fv( glGetUniformLocation( this->RendererID, _name.c_str(  ) ), 1, glm::value_ptr(_var) ) );
            (*this).UnBind(  );
        }

        void SetUniform3f( const std::string& _name, const glm::vec3& _var )
        {
            (*this).Bind(  );
            GLCall( glUniform3fv( glGetUniformLocation( this->RendererID, _name.c_str(  ) ), 1, glm::value_ptr(_var) ) );
            (*this).UnBind(  );
        }

        void SetUniform4f( const std::string& _name, const glm::vec4& _var )
        {
            (*this).Bind(  );
            GLCall( glUniform4fv( glGetUniformLocation( this->RendererID, _name.c_str(  ) ), 1, glm::value_ptr(_var) ) );
            (*this).UnBind(  );
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
