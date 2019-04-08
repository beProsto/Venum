#ifndef WINDOWCODE_HPP_INCLUDED
#define WINDOWCODE_HPP_INCLUDED

ven::Window::Window(  )
{
    this->running = true;
}

ven::Window::Window( const std::string& _title, unsigned int _width, unsigned int _height, unsigned int _flags )
{
    this->Construct( _title, _width, _height, _flags );
    this->running = true;
}

ven::Window::~Window(  )
{
    SDL_GL_DeleteContext( this->context );
    SDL_DestroyWindow( this->window );
    SDL_Quit(  );
}

void ven::Window::Swapb(  )
{
    SDL_GL_SwapWindow( this->window );
}

glm::vec2 ven::Window::GetSize(  )
{
    glm::vec2 sizeofwindow;
    int x, y;
    SDL_GetWindowSize( this->window, &x, &y );
    sizeofwindow.x = x;
    sizeofwindow.y = y;
    return sizeofwindow;
}

SDL_Window* ven::Window::GetWindow(  )
{
    return this->window;
}

SDL_GLContext ven::Window::GetContext(  )
{
    return this->context;
}

bool ven::Window::PollEvent( SDL_Event* _event )
{
    if( SDL_PollEvent( _event ) )
    {
        if( _event->type == SDL_QUIT )
        {
            this->running = false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool ven::Window::Running(  )
{
    return this->running;
}

void ven::Window::Close(  )
{
    this->running = false;
}

bool ven::Window::Construct( const std::string& _title, unsigned int _width, unsigned int _height, unsigned int _flags )
{
    SDL_Init( SDL_INIT_EVERYTHING );

    this->window = SDL_CreateWindow( _title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL | _flags );
    this->context = SDL_GL_CreateContext( this->window );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );

    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    glewExperimental = GL_TRUE;
    if( glewInit(  ) != GLEW_NO_ERROR )
    {
        std::cerr << "Cannot initialize GLEW ! \n";
        return false;
    }

    return true;
}

#endif // WINDOWCODE_HPP_INCLUDED
