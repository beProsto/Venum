#ifndef WINDOWCLASS_HPP_INCLUDED
#define WINDOWCLASS_HPP_INCLUDED


namespace ven
{

class Window
{
    public:
        Window(  );
        Window( const std::string& _title, unsigned int _width = 640, unsigned int _height = 360, unsigned int _flags = 0 );
        ~Window(  );

        bool Construct( const std::string& _title, unsigned int _width, unsigned int _height, unsigned int _flags );

        bool Running(  );
        bool PollEvent( SDL_Event* _event );

        glm::vec2 GetSize(  );
        SDL_Window* GetWindow(  );
        SDL_GLContext GetContext(  );

        void Close(  );

        void Swapb(  );

    private:
        SDL_Window*   window;
        SDL_GLContext context;
        bool          running;
};


}

#endif // WINDOWCLASS_HPP_INCLUDED
