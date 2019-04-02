#ifndef INDEXBUFFER_HPP_INCLUDED
#define INDEXBUFFER_HPP_INCLUDED

#include "gl/glcall.hpp"

namespace ven
{

class IndexBuffer
{
    public:
        IndexBuffer( const void* _index_buffer_data, unsigned int _count )
        {
            this->Count = _count;
            GLCall(glGenBuffers(1, &this->RendererID));
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->RendererID));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Count * sizeof( unsigned int ), _index_buffer_data, GL_STATIC_DRAW));
        }

        ~IndexBuffer(  )
        {
            GLCall(glDeleteBuffers( 1, &this->RendererID ));
        }

        void Bind(  ) const
        {
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->RendererID));
        }

        void UnBind(  ) const
        {
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

        unsigned int GetRendererID(  ) const
        { return this->RendererID; }

        unsigned int GetCount(  ) const
        { return this->Count; }

    private:
        unsigned int RendererID;
        unsigned int Count;
};

}

#endif // INDEXBUFFER_HPP_INCLUDED
