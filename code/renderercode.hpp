#ifndef RENDERERCODE_HPP_INCLUDED
#define RENDERERCODE_HPP_INCLUDED

ven::Renderer::Renderer(  )
: CLEANING_ADDITIONAL_FLAGS( 0 )
{  }

ven::Renderer::~Renderer(  )
{  }

void ven::Renderer::Clean(  )
{
    this->Clean( glm::vec3( 0.0f, 0.0f, 0.0f ) );
}

void ven::Renderer::Clean( glm::vec3 _color )
{
    GLCall( glClearColor( _color.x, _color.y, _color.z, 1.0f ) );
    GLCall( glClear( GL_COLOR_BUFFER_BIT | this->CLEANING_ADDITIONAL_FLAGS ) );
}

void ven::Renderer::PushCleaningFlags( unsigned int _ADD_CLEANING_FLAGS )
{
    this->CLEANING_ADDITIONAL_FLAGS = this->CLEANING_ADDITIONAL_FLAGS | _ADD_CLEANING_FLAGS;
}

void ven::Renderer::ChangeCleaningFlags( unsigned int _CLEANING_FLAGS )
{
    this->CLEANING_ADDITIONAL_FLAGS = _CLEANING_FLAGS;
}

#endif // RENDERERCODE_HPP_INCLUDED
