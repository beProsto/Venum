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

        void Draw(  );
        void Clean(  );
        void Clean( glm::vec3 _color );

    private:
        unsigned int CLEANING_ADDITIONAL_FLAGS;

};


}

#endif // RENDERERCLASS_HPP_INCLUDED
