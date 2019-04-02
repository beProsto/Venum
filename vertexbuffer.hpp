#ifndef VERTEXBUFFER_HPP_INCLUDED
#define VERTEXBUFFER_HPP_INCLUDED

#include "gl/glcall.hpp"

namespace ven
{

class VertexBuffer
{
    public:
        VertexBuffer(  )
        : RendererID( 0 )
        {  }

        VertexBuffer( const VertexBuffer& _other )
        : RendererID( 0 )
        {
            this->RendererID = _other.RendererID;
        }

        VertexBuffer( const void* _vertex_buffer_data, unsigned int _size )
        : RendererID( 0 )
        {
            GLCall(glGenBuffers(1, &this->RendererID));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->RendererID));
            GLCall(glBufferData(GL_ARRAY_BUFFER, _size, _vertex_buffer_data, GL_STATIC_DRAW));
        }

        ~VertexBuffer(  )
        {
            this->Delete(  );
        }

        void Delete(  )
        {
            GLCall(glDeleteBuffers( 1, &this->RendererID ));
        }

        void Bind(  ) const
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, this->RendererID));
        }

        void UnBind(  ) const
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        unsigned int GetRendererID(  ) const
        { return this->RendererID; }

    private:
        unsigned int RendererID;
};

}

#endif // VERTEXBUFFER_HPP_INCLUDED
