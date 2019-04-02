#ifndef MOUSECODE_HPP_INCLUDED
#define MOUSECODE_HPP_INCLUDED

ven::Mouse::Mouse(  )
{

}

ven::Mouse::Mouse( ven::Window* _windowPointer )
{
    this->AttachWindowPointer( _windowPointer );
}

void ven::Mouse::AttachWindowPointer( ven::Window* _windowPointer )
{
    this->windowPointer = _windowPointer;
}

ven::Mouse::~Mouse(  )
{

}

glm::vec2 ven::Mouse::GetPosition( GLenum MOUSE_POSITION_TYPE )
{
	int x, y;
    SDL_GetMouseState( &x, &y );

    glm::vec2 position;

	if( MOUSE_POSITION_TYPE == MOUSE_POSITION_TYPE_WINDOW_BASED )
    {
        position.x = (float)x / (float)windowPointer->GetSize(  ).x;
        position.y = (float)y / (float)windowPointer->GetSize(  ).y;
    }
    else if( MOUSE_POSITION_TYPE == MOUSE_POSITION_TYPE_GL_BASED )
    {
        position.x = ( (float)x  / (float)windowPointer->GetSize(  ).x - 0.5f ) * 2.0f;
        position.y = -( (float)y / (float)windowPointer->GetSize(  ).y - 0.5f ) * 2.0f;
    }

    return position;
}

void ven::Mouse::SetPosition( GLenum MOUSE_POSITION_TYPE, glm::vec2 _position )
{
    if( MOUSE_POSITION_TYPE == MOUSE_POSITION_TYPE_WINDOW_BASED )
    {
        SDL_WarpMouseInWindow( this->windowPointer->GetWindow(  ), _position.x * (float)this->windowPointer->GetSize(  ).x, _position.y * (float)this->windowPointer->GetSize(  ).y );
    }
    else if( MOUSE_POSITION_TYPE == MOUSE_POSITION_TYPE_GL_BASED )
    {
        SDL_WarpMouseInWindow( this->windowPointer->GetWindow(  ), ( ( _position.x + 1.0f ) * 0.5f ) * (float)this->windowPointer->GetSize(  ).x , ( ( -_position.y + 1.0f ) * 0.5f ) * (float)this->windowPointer->GetSize(  ).y );
    }
}

bool ven::Mouse::GetButtonDown( GLenum MOUSE_BUTTON )
{
    if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_LEFT ) && MOUSE_BUTTON == MOUSE_BUTTON_LEFT )
    {
        return true;
    }
    else if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_MIDDLE ) && MOUSE_BUTTON == MOUSE_BUTTON_MIDDLE )
    {
        return true;
    }
    else if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_RIGHT ) && MOUSE_BUTTON == MOUSE_BUTTON_RIGHT )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ven::Mouse::GetButtonUp( GLenum MOUSE_BUTTON )
{
    if( !this->GetButtonDown( MOUSE_BUTTON ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}


#endif // MOUSECODE_HPP_INCLUDED
