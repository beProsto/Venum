#ifndef RENDERERCLASS_HPP_INCLUDED
#define RENDERERCLASS_HPP_INCLUDED

namespace ven
{

class Renderer
{
    public:
        Renderer(  );
        ~Renderer(  );

        void PushCleaningFlags( unsigned int _ADD_CLEANING_FLAGS );
        void ChangeCleaningFlags( unsigned int _CLEANING_FLAGS );

        void DrawIndices( const ShaderCombiner& _shader, const VertexArray& _vao, const VertexBuffer& _vbo, const IndexBuffer& _ibo, unsigned int _type = GL_TRIANGLES );

        void Clean(  );
        void Clean( glm::vec3 _color );

    private:
        unsigned int CLEANING_ADDITIONAL_FLAGS;

};


}

#endif // RENDERERCLASS_HPP_INCLUDED
