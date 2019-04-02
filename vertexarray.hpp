#ifndef VERTEXARRAY_HPP_INCLUDED
#define VERTEXARRAY_HPP_INCLUDED

#include "gl/glcall.hpp"

namespace ven
{

class VertexArray
{
    public:
        VertexArray(  )
        {
            GLCall(glGenVertexArrays(1, &this->RendererID));
            GLCall(glBindVertexArray(this->RendererID));
        }

        ~VertexArray(  )
        {
            GLCall(glDeleteVertexArrays(1, &this->RendererID));
        }

        void Bind(  ) const
        {
            GLCall(glBindVertexArray(this->RendererID));
        }

        void UnBind(  ) const
        {
            GLCall(glBindVertexArray(0));
        }

        void PushAtrib( unsigned int _index, unsigned int _size, unsigned int _type, GLboolean _normalized, unsigned int _stride, const void* _offset )
        {
            this->Bind(  );

            /* passing attributes into the buffer */
            glEnableVertexAttribArray(_index);
            glVertexAttribPointer(
                _index,                   /* layout in shader */
                _size,                    /* size */
                _type,                    /* type */
                _normalized,              /* normalized? */
                _stride,                  /* stride */
                _offset                   /* array buffer offset */
            );
        }

        unsigned int GetRendererID(  ) const
        { return this->RendererID; }

    private:
        unsigned int RendererID;
};


}


#endif // VERTEXARRAY_HPP_INCLUDED
