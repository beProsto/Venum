#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#ifdef Mixer
#include <SDL2/SDL_mixer.h>
#endif // Mixer

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <string>
#include <fstream>
#include <strstream>
#include <cstdlib>
#include <ctime>
#include <memory>

std::string LoadStringFromFile(const std::string& a_Filename) {
    std::ifstream f_File;
    f_File.open(a_Filename);

    if(f_File.good()) {
        std::string f_Result = "";

        std::string f_Line;
        while(std::getline(f_File, f_Line)) {
            f_Result += f_Line;
            f_Result += '\n';
        }
        #ifdef VENUM_DEBUG_EXTREME
        std::cout << "Loaded file: '" << a_Filename << "' Using 'LoadStringFromFile' Function! " << std::endl;
        #endif // VENUM_DEBUG_EXTREME

        return f_Result;
    }
    else {
        std::cerr << "Unable to load file from string: '" << a_Filename << "'!" << std::endl;
        return "";
    }
}
template <typename T> T Map(T a_Value, T a_Start1, T a_Stop1, T a_Start2, T a_Stop2) {
	return ((a_Value - a_Start1) / (a_Stop1 - a_Start1)) * (a_Stop2 - a_Start2) + a_Start2;
}
template <typename T> T Constrain(T a_Value, T a_Start, T a_Stop) {
    if(a_Value < a_Start) {
        return a_Start;
    }
    else if(a_Value > a_Stop) {
        return a_Stop;
    }
    else {
        return a_Value;
    }
}

enum class API {
    OpenGL = 0,
};
class RendererAPI {
    public:
        static API GetAPI() {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "'GetAPI()' Used! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
            return s_API;
        }

    private:
        static API s_API;
};
API RendererAPI::s_API = API::OpenGL;

class ViewportClass {
    public:
        void Update() {
            if(RendererAPI::GetAPI() == API::OpenGL) {
                glViewport(Position.x, Position.y, Size.x, Size.y);
            }
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Viewport set to: " << Position.x << " : " << Position.y << " ; " << Size.x << " : " << Size.y << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        void Update(glm::vec2 a_Position, glm::vec2 a_Size) {
            Position = a_Position;
            Size = a_Size;
            if(RendererAPI::GetAPI() == API::OpenGL) {
                glViewport(Position.x, Position.y, Size.x, Size.y);
            }
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Viewport set to: " << Position.x << " : " << Position.y << " ; " << Size.x << " : " << Size.y << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        float GetAspect() {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "'GetAspect()' Used! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
            return Size.x / Size.y;
        }

    public:
        glm::vec2 Position;
        glm::vec2 Size;
};
ViewportClass Viewport;

class WindowClass {
    public:
        WindowClass() {
            m_Running = true;
        }
        ~WindowClass() {
            if(RendererAPI::GetAPI() == API::OpenGL) {
                SDL_GL_DeleteContext(m_Context);
            }
            SDL_DestroyWindow(m_Window);
            #ifdef Mixer
            Mix_Quit();
            #endif // Mixer
            IMG_Quit();
            SDL_Quit();
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Window Destructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        bool Create(const std::string& a_Title = "Venum Test", unsigned int a_Width = 640, unsigned int a_Height = 360, unsigned int a_Flags = 0) {
            SDL_Init(SDL_INIT_EVERYTHING);
            IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
            #ifdef Mixer
            Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPUS);
            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                return false;
            }
            #endif // Mixer

            m_Window = SDL_CreateWindow(a_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_Width, a_Height, (RendererAPI::GetAPI() == API::OpenGL ? SDL_WINDOW_OPENGL : SDL_WINDOW_VULKAN) | a_Flags);

            if(RendererAPI::GetAPI() == API::OpenGL) {
                m_Context = SDL_GL_CreateContext(m_Window);

                SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
                SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
                SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
                SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
                SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                glewExperimental = GL_TRUE;
                if(glewInit(  ) != GLEW_OK) {
                    std::cerr << "Can't initialize GLEW!" << std::endl;
                    return false;
                }
            }
            Viewport.Update(glm::vec2(0, 0), glm::vec2(GetWidth(), GetHeight()));

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Window Created! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            return true;
        }

        bool Running() {
            return m_Running;
        }
        void Close() {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Window Closed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
            m_Running = false;
        }

        bool PollEvent(SDL_Event& a_Event) {
            if(SDL_PollEvent(&a_Event)) {
                if(a_Event.type == SDL_QUIT) {
                    Close();
                }
                return true;
            }
            else {
                return false;
            }
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Event Polled! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        void SwapBuffers() {
            SDL_GL_SwapWindow(m_Window);
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "Window Buffer and Main Buffer Swapped! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        unsigned int GetWidth() {
            int f_Width;
            int f_Height;
            SDL_GetWindowSize(m_Window, &f_Width, &f_Height);
            return static_cast<unsigned int>(f_Width);
        }
        unsigned int GetHeight() {
            int f_Width;
            int f_Height;
            SDL_GetWindowSize(m_Window, &f_Width, &f_Height);
            return static_cast<unsigned int>(f_Height);
        }
        SDL_Window* GetWindow() {
            return m_Window;
        }

    private:
        SDL_Window* m_Window;
        SDL_GLContext m_Context;
        bool m_Running;
};
WindowClass Window;

class FPSLimiterClass {
    public:
        FPSLimiterClass(unsigned int a_FPS = 60) {
            SetFPSLimit(a_FPS);
        }
        ~FPSLimiterClass() {

        }

        void UpdateStart() {
            m_Cycle = SDL_GetTicks();
        }
        void UpdateEnd() {
            unsigned int f_CycleEnd = SDL_GetTicks();
            unsigned int f_CycleDifference = f_CycleEnd - m_Cycle;
            float f_Delay = 1000.0f / m_FPSLimit - f_CycleDifference;
            if(f_Delay > 0.0f) {
                SDL_Delay(f_Delay);
            }
        }

        void SetFPSLimit(unsigned int a_FPS) {
            m_FPSLimit = a_FPS;
        }

    private:
        unsigned int m_Cycle;
        unsigned int m_FPSLimit;
};
FPSLimiterClass FPSLimiter;
class KeyboardClass {
    public:
        bool GetKeyState(Uint32 a_ScanCode) {
            const Uint8* f_Keystate = SDL_GetKeyboardState(NULL);
            return f_Keystate[a_ScanCode];
        }
};
KeyboardClass Keyboard;
class MouseClass {
    public:
        MouseClass() {
            m_Position = glm::vec2(0, 0);
            m_LastPosition = glm::vec2(0, 0);
            m_Visible = true;
        }
        ~MouseClass() {

        }

        glm::vec2 GetPosition() {
            m_LastPosition = m_Position;

            int f_MousePosition[2];
            SDL_GetMouseState(&f_MousePosition[0], &f_MousePosition[1]);

            m_Position = glm::vec2(f_MousePosition[0], f_MousePosition[1]);

            return m_Position;
        }
        glm::vec2 GetPositionRelative() {
            m_LastPosition = m_Position;

            int f_MousePosition[2];
            SDL_GetMouseState(&f_MousePosition[0], &f_MousePosition[1]);

            m_Position = glm::vec2(f_MousePosition[0], f_MousePosition[1]);

            return m_Position - m_LastPosition;
        }
        bool GetVisibility() {
            return m_Visible;
        }

        void SetPosition(glm::vec2 a_Position) {
            SDL_WarpMouseInWindow(Window.GetWindow(), a_Position.x, a_Position.y);
        }
        void SetVisibility(bool a_Visible) {
            SDL_ShowCursor(a_Visible);
            m_Visible = a_Visible;
        }

    private:
        glm::vec2 m_Position;
        glm::vec2 m_LastPosition;
        bool m_Visible;
};
MouseClass Mouse;

class Timer {
    public:
        Timer() {
        }
        ~Timer() {

        }

        void UpdateStart() {
            m_StartTicks = SDL_GetTicks();
        }
        void UpdateEnd() {
            m_ElapsedTicks = SDL_GetTicks() - m_StartTicks;
            m_DeltaTime = (float)m_ElapsedTicks * 0.001f;
        }

        float GetDeltaTime() {
            return m_DeltaTime;
        }

    private:
        unsigned int m_StartTicks;
        unsigned int m_ElapsedTicks;
        float m_DeltaTime;
};

#ifdef Mixer
class Music {
    public:
        Music() {
            m_Sample = NULL;
        }
        Music(const std::string& a_Filename) {
            m_Sample = NULL;
            LoadMusicFromFile(a_Filename);
        }
        ~Music() {
            Mix_FreeMusic(m_Sample);
        }

        void LoadMusicFromFile(const std::string& a_Filename) {
            if(m_Sample != NULL) {
                Mix_FreeMusic(m_Sample);
            }
            m_Sample = Mix_LoadMUS(a_Filename.c_str());
        }
        void Play(unsigned int a_Loops = 0) {
            Mix_PlayMusic(m_Sample, a_Loops);
        }
        void Pause() {
            Mix_PauseMusic();
        }
        void Resume() {
            Mix_ResumeMusic();
        }
        void Stop() {
            Mix_HaltMusic();
        }

        bool IsPlayed() const {
            return Mix_PlayingMusic();
        }
        bool IsPaused() const {
            return Mix_PausedMusic();
        }
        bool IsStopped() const {
            return !IsPlayed();
        }
        int GetVolume() const {
            return Mix_VolumeMusic(-1);
        }

        void SetVolume(int a_Volume) {
            Mix_VolumeMusic(a_Volume);
        }

    private:
        Mix_Music* m_Sample;
};
class Sound {
    public:
        Sound(int a_Channel = 0) {
            m_Sample = NULL;
            m_Channel = a_Channel;
        }
        Sound(const std::string& a_Filename, int a_Channel = 0) {
            m_Sample = NULL;
            LoadSoundFromFile(a_Filename);
            m_Channel = a_Channel;
        }
        ~Sound() {
            Mix_FreeChunk(m_Sample);
        }

        void LoadSoundFromFile(const std::string& a_Filename) {
            if(m_Sample != NULL) {
                Mix_FreeChunk(m_Sample);
            }
            m_Sample = Mix_LoadWAV(a_Filename.c_str());
        }
        void Play(unsigned int a_Loops = 0) {
            Mix_PlayChannel(m_Channel, m_Sample, a_Loops);
        }
        void Pause() {
            Mix_Pause(m_Channel);
        }
        void Resume() {
            Mix_Resume(m_Channel);
        }
        void Stop() {
            Mix_HaltChannel(m_Channel);
        }

        bool IsPlayed() const {
            return Mix_Playing(m_Channel);
        }
        bool IsPaused() const {
            return Mix_Paused(m_Channel);
        }
        bool IsStopped() const {
            return !IsPlayed();
        }
        int GetVolume() const {
            return Mix_Volume(m_Channel, -1);
        }
        int GetChannel() const {
            return m_Channel;
        }

        void SetVolume(int a_Volume) {
            Mix_Volume(m_Channel, a_Volume);
        }
        void SetChannel(int a_Channel) {
            m_Channel = a_Channel;
        }

    private:
        Mix_Chunk* m_Sample;
        int m_Channel;
};
#endif // Mixer


class Transform3D {
    public:
        Transform3D(glm::vec3 a_Position = glm::vec3(0.0f), glm::vec3 a_Rotation = glm::vec3(0.0f), glm::vec3 a_Scale = glm::vec3(1.0f)) {
            Position = a_Position;
            Rotation = a_Rotation;
            Scale = a_Scale;
        }
        ~Transform3D() {

        }

        glm::mat4 GetPositionMatrix() const {
            return glm::translate(glm::mat4(1.0f), Position);
        }
        glm::mat4 GetRotationMatrix() const {
            return glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                    glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        glm::mat4 GetScaleMatrix() const {
            return glm::scale(glm::mat4(1.0f), Scale);
        }

        glm::mat4 GetMatrix() const {
            return GetPositionMatrix() * GetRotationMatrix() * GetScaleMatrix();
        }

    public:
        glm::vec3 Position, Rotation, Scale;
};
class Camera {
    public:
        glm::vec3 GetForward() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
        }
        glm::vec3 GetBackward() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        }
        glm::vec3 GetUpside() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        glm::vec3 GetDownside() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(0.0f, -1.0f, 0.0f, 1.0f));
        }
        glm::vec3 GetLeft() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
        }
        glm::vec3 GetRight() const {
            return glm::normalize(Transform.GetRotationMatrix() * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        virtual glm::mat4 GetMatrix() const {
            return glm::mat4(1.0f);
        }

    public:
        Transform3D Transform;
};
class Camera3D: public Camera {
    public:
        Camera3D(const Transform3D& a_Transform = Transform3D(), float a_FOV = 90.0f, float a_Near = 0.01f, float a_Far = 1000.0f) {
            Transform = a_Transform;
            FOV = a_FOV;
            Near = a_Near;
            Far = a_Far;
        }
        ~Camera3D() {

        }

        glm::mat4 GetMatrix() const {
            return glm::perspective(glm::radians(FOV), Viewport.GetAspect(), Near, Far) * glm::lookAt(Transform.Position, Transform.Position + GetForward(), GetUpside());
        }

    public:
        float FOV;
        float Near, Far;
};
class Camera2D: public Camera {
    public:
        Camera2D(Transform3D a_Transform = Transform3D(), glm::vec2 a_LeftDown = glm::vec2(-1.0f, -1.0f), glm::vec2 a_RightTop = glm::vec2(1.0f, 1.0f), float a_Near = 0.01f, float a_Far = 1000.0f) {
            Transform = a_Transform;
            LeftDown = a_LeftDown;
            RightTop = a_RightTop;
            Near = a_Near;
            Far = a_Far;
        }
        ~Camera2D() {

        }

        glm::mat4 GetMatrix() const {
            return glm::ortho(LeftDown.x, RightTop.x, LeftDown.y, RightTop.y, Near, Far) * glm::lookAt(Transform.Position, Transform.Position + GetForward(), GetUpside());
        }

    public:
        glm::vec2 LeftDown;
        glm::vec2 RightTop;
        float Near, Far;
};

class Vertex3D {
    public:
        Vertex3D(glm::vec3 a_Position, glm::vec2 a_TexCoord, glm::vec3 a_Normal) {
            Position = a_Position;
            TexCoord = a_TexCoord;
            Normal = a_Normal;
        }
        Vertex3D(float a_PositionX, float a_PositionY, float a_PositionZ, float a_TexCoordX, float a_TexCoordY, float a_NormalX, float a_NormalY, float a_NormalZ) {
            Position = glm::vec3(a_PositionX, a_PositionY, a_PositionZ);
            TexCoord = glm::vec2(a_TexCoordX, a_TexCoordY);
            Normal = glm::vec3(a_NormalX, a_NormalY, a_NormalZ);
        }
        ~Vertex3D() {

        }

    public:
        glm::vec3 Position;
        glm::vec2 TexCoord;
        glm::vec3 Normal;
};
class VertexBuffer {
    public:
        static VertexBuffer* Create();
        static VertexBuffer* Create(const std::vector<Vertex3D>& a_Data);
        static VertexBuffer* Create(Vertex3D* a_Data, unsigned int a_Count);
        virtual ~VertexBuffer() {

        }

        virtual void BufferData(const std::vector<Vertex3D>& a_Data) {

        }
        virtual void BufferData(Vertex3D* a_Data, unsigned int a_Count) {

        }

        virtual void Bind() const {

        }

        virtual unsigned int GetCount() const {

        }
};
class OpenGLVertexBuffer: public VertexBuffer {
    public:
        OpenGLVertexBuffer() {
            glCreateVertexArrays(1, &m_VARendererID);
            glBindVertexArray(m_VARendererID);

            glCreateBuffers(1, &m_VBRendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(glm::vec2)));
            glEnableVertexAttribArray(2);

            m_Count = 0;

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        OpenGLVertexBuffer(const std::vector<Vertex3D>& a_Data) {
            glCreateVertexArrays(1, &m_VARendererID);
            glBindVertexArray(m_VARendererID);

            glCreateBuffers(1, &m_VBRendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(glm::vec2)));
            glEnableVertexAttribArray(2);

            BufferData(a_Data);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        OpenGLVertexBuffer(Vertex3D* a_Data, unsigned int a_Count) {
            glCreateVertexArrays(1, &m_VARendererID);
            glBindVertexArray(m_VARendererID);

            glCreateBuffers(1, &m_VBRendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), reinterpret_cast<void*>(sizeof(glm::vec3) + sizeof(glm::vec2)));
            glEnableVertexAttribArray(2);

            BufferData(a_Data, a_Count);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        ~OpenGLVertexBuffer() {
            glDeleteBuffers(1, &m_VBRendererID);
            glDeleteVertexArrays(1, &m_VARendererID);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Destructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        void BufferData(const std::vector<Vertex3D>& a_Data) {
            Bind();
            m_Count = a_Data.size();
            glBufferData(GL_ARRAY_BUFFER, a_Data.size() * sizeof(Vertex3D), &a_Data[0], GL_STATIC_DRAW);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Send Data! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        void BufferData(Vertex3D* a_Data, unsigned int a_Count) {
            Bind();
            m_Count = a_Count;
            glBufferData(GL_ARRAY_BUFFER, a_Count * sizeof(Vertex3D), a_Data, GL_STATIC_DRAW);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Send Data! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        void Bind() const {
            glBindVertexArray(m_VARendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Bound! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        unsigned int GetCount() const {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " Count Given! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            return m_Count;
        }

        unsigned int GetVertexBufferRendererID() const {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " VertexBuffer RendererID Given! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            return m_VBRendererID;
        }
        unsigned int GetVertexArrayRendererID() const {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL VertexBuffer: " << m_VBRendererID << " : " << m_VARendererID << " VertexArray RendererID Given! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            return m_VARendererID;
        }

    private:
        unsigned int m_VBRendererID;
        unsigned int m_Count;
        unsigned int m_VARendererID;
};
VertexBuffer* VertexBuffer::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLVertexBuffer();
    }
}
VertexBuffer* VertexBuffer::Create(const std::vector<Vertex3D>& a_Data) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLVertexBuffer(a_Data);
    }
}
VertexBuffer* VertexBuffer::Create(Vertex3D* a_Data, unsigned int a_Count) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLVertexBuffer(a_Data, a_Count);
    }
}
void LoadVerticesFromOBJFile(const std::string& a_Filename, std::vector<Vertex3D>& a_Data) {
    std::fstream f_File(a_Filename);

    if(!f_File.is_open()) {
        std::cerr <<  "Unable to find file: '" << a_Filename << "'!" << std::endl;
        return;
    }

    std::vector<glm::vec3> f_Positions;
    std::vector<glm::vec2> f_TexCoords;
    std::vector<glm::vec3> f_Normals;

    a_Data.clear();

    std::string f_Line;
    while(std::getline(f_File, f_Line)) {
        unsigned int f_SlashCount = 0;
        for(char& c : f_Line) {
            if(c == '/') {
                c = ' ';
                f_SlashCount++;
            }
        }

        unsigned int f_VerticesInFace = f_SlashCount / 2;

        std::strstream f_LineStream;
        f_LineStream << f_Line;

        if(f_Line[0] == 'v' && f_Line[1] == ' ') {
            std::string f_Junk;
            glm::vec3 f_Data;
            f_LineStream >> f_Junk >> f_Data.x >> f_Data.y >> f_Data.z;
            f_Positions.push_back(f_Data);
        }
        else if(f_Line[0] == 'v' && f_Line[1] == 't') {
            std::string f_Junk;
            glm::vec2 f_Data;
            f_LineStream >> f_Junk >> f_Data.x >> f_Data.y;
            f_TexCoords.push_back(f_Data);
        }
        else if(f_Line[0] == 'v' && f_Line[1] == 'n') {
            std::string f_Junk;
            glm::vec3 f_Data;
            f_LineStream >> f_Junk >> f_Data.x >> f_Data.y >> f_Data.z;
            f_Normals.push_back(f_Data);
        }
        else if(f_Line[0] == 'f' && f_Line[1] == ' ') {
            std::string f_Junk;

            if(f_VerticesInFace == 3) {
                unsigned int f_IndexPosition1, f_IndexTexCoord1, f_IndexNormal1,
                            f_IndexPosition2, f_IndexTexCoord2, f_IndexNormal2,
                            f_IndexPosition3, f_IndexTexCoord3, f_IndexNormal3;

                f_LineStream >> f_Junk >> f_IndexPosition1 >> f_IndexTexCoord1 >> f_IndexNormal1
                            >> f_IndexPosition2 >> f_IndexTexCoord2 >> f_IndexNormal2
                            >> f_IndexPosition3 >> f_IndexTexCoord3 >> f_IndexNormal3;

                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition1 - 1], f_TexCoords[f_IndexTexCoord1 - 1], f_Normals[f_IndexNormal1 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition2 - 1], f_TexCoords[f_IndexTexCoord2 - 1], f_Normals[f_IndexNormal2 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition3 - 1], f_TexCoords[f_IndexTexCoord3 - 1], f_Normals[f_IndexNormal3 - 1]));
            }
            else if(f_VerticesInFace == 4) {
                unsigned int f_IndexPosition1, f_IndexTexCoord1, f_IndexNormal1,
                            f_IndexPosition2, f_IndexTexCoord2, f_IndexNormal2,
                            f_IndexPosition3, f_IndexTexCoord3, f_IndexNormal3,
                            f_IndexPosition4, f_IndexTexCoord4, f_IndexNormal4;

                f_LineStream >> f_Junk >> f_IndexPosition1 >> f_IndexTexCoord1 >> f_IndexNormal1
                            >> f_IndexPosition2 >> f_IndexTexCoord2 >> f_IndexNormal2
                            >> f_IndexPosition3 >> f_IndexTexCoord3 >> f_IndexNormal3
                            >> f_IndexPosition4 >> f_IndexTexCoord4 >> f_IndexNormal4;

                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition1 - 1], f_TexCoords[f_IndexTexCoord1 - 1], f_Normals[f_IndexNormal1 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition2 - 1], f_TexCoords[f_IndexTexCoord2 - 1], f_Normals[f_IndexNormal2 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition3 - 1], f_TexCoords[f_IndexTexCoord3 - 1], f_Normals[f_IndexNormal3 - 1]));

                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition3 - 1], f_TexCoords[f_IndexTexCoord3 - 1], f_Normals[f_IndexNormal3 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition4 - 1], f_TexCoords[f_IndexTexCoord4 - 1], f_Normals[f_IndexNormal4 - 1]));
                a_Data.push_back(Vertex3D(f_Positions[f_IndexPosition1 - 1], f_TexCoords[f_IndexTexCoord1 - 1], f_Normals[f_IndexNormal1 - 1]));
            }
            else {
                assert(false);
            }
        }
    }
}

class ShaderElement {
    public:
        static ShaderElement* Create();
        static ShaderElement* Create(unsigned int a_Type, const std::string& a_Source);
        virtual ~ShaderElement() {

        }

        virtual void Compile(unsigned int a_Type, const std::string& a_Source) {

        }

        virtual bool DestroyAfterAttaching() const {

        }

};
class OpenGLShaderElement: public ShaderElement {
    public:
        OpenGLShaderElement() {
            m_Created = false;
        }
        OpenGLShaderElement(unsigned int a_Type, const std::string& a_Source) {
            Compile(a_Type, a_Source);
        }
        ~OpenGLShaderElement() {
            if(m_Created) {
                glDeleteShader(m_RendererID);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL ShaderElement: " << m_RendererID << " Destructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
            }
        }

        void Compile(unsigned int a_Type, const std::string& a_Source) {
            m_Created = true;
            m_RendererID = glCreateShader(a_Type);

            const char* f_Source = a_Source.c_str();
            glShaderSource(m_RendererID, 1, &f_Source, nullptr);

            glCompileShader(m_RendererID);

            int f_Compiled;
            glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &f_Compiled);
            if(f_Compiled == GL_FALSE) {
                int f_Length;
                glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &f_Length);

                char* f_Message = new char[f_Length];
                glGetShaderInfoLog(m_RendererID, f_Length, &f_Length, f_Message);

                std::cerr << "[SHADER COMPILATION ERROR]" << f_Message << std::endl;

                delete[] f_Message;
            }

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL ShaderElement: " << m_RendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        unsigned int GetRendererID() const {
            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL ShaderElement: " << m_RendererID << " Renderer ID Given! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            return m_RendererID;
        }

    private:
        unsigned int m_RendererID;
        bool m_Created;
};
ShaderElement* ShaderElement::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLShaderElement();
    }
}
ShaderElement* ShaderElement::Create(unsigned int a_Type, const std::string& a_Source) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLShaderElement(a_Type, a_Source);
    }
}
class Shader {
    public:
        static Shader* Create();
        static Shader* Create(const ShaderElement* a_VertexShader, const ShaderElement* a_FragmentShader, bool a_Compile = true);
        virtual ~Shader() {

        }

        virtual void AttachShaderElement(const ShaderElement* a_ShaderElement) {

        }

        virtual void Compile() {

        }

        virtual void Bind() const {

        }
};
class OpenGLShader: public Shader {
    public:
        OpenGLShader() {
            m_RendererID = glCreateProgram();

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Shader: " << m_RendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
        OpenGLShader(const ShaderElement* a_VertexShader, const ShaderElement* a_FragmentShader, bool a_Compile) {
            m_RendererID = glCreateProgram();

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Shader: " << m_RendererID << " Constructed! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            AttachShaderElement(a_VertexShader);
            AttachShaderElement(a_FragmentShader);

            if(a_Compile) {
                Compile();
            }
        }
        ~OpenGLShader() {
            glDeleteProgram(m_RendererID);
        }

        void AttachShaderElement(const ShaderElement* a_ShaderElement) {
            glAttachShader(m_RendererID, dynamic_cast<const OpenGLShaderElement*>(a_ShaderElement)->GetRendererID());

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Shader: " << m_RendererID << " ShaderElement Attached: " << dynamic_cast<const OpenGLShaderElement*>(a_ShaderElement)->GetRendererID() << " !" << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        void Compile() {
            glLinkProgram(m_RendererID);
            glValidateProgram(m_RendererID);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Shader: " << m_RendererID << " Compiled! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME

            Bind();
        }

        void Bind() const {
            glUseProgram(m_RendererID);

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Shader: " << m_RendererID << " Bound! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }

        unsigned int GetRendererID() const {
            return m_RendererID;
        }

        void SetUniform(const std::string& Name, glm::mat4 Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Value));
        }
        void SetUniform(const std::string& Name, glm::vec4 Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniform4fv(location, 1, glm::value_ptr(Value));
        }
        void SetUniform(const std::string& Name, glm::vec3 Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniform3fv(location, 1, glm::value_ptr(Value));
        }
        void SetUniform(const std::string& Name, glm::vec2 Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniform2fv(location, 1, glm::value_ptr(Value));
        }
        void SetUniform(const std::string& Name, float Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniform1f(location, Value);
        }
        void SetUniform(const std::string& Name, int Value) {
            Bind();
            int location = glGetUniformLocation(m_RendererID, Name.c_str());
            assert(location != -1);
            glUniform1i(location, Value);
        }

    private:
        unsigned int m_RendererID;
};
Shader* Shader::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLShader();
    }
}
Shader* Shader::Create(const ShaderElement* a_VertexShader, const ShaderElement* a_FragmentShader, bool a_Compile) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLShader(a_VertexShader, a_FragmentShader, a_Compile);
    }
}

class TextureSettings {
    public:
        TextureSettings(unsigned int a_Interpolation = GL_LINEAR, unsigned int a_Wrapping = GL_CLAMP_TO_EDGE) {
            Interpolation = a_Interpolation;
            Wrapping = a_Wrapping;
        }
        ~TextureSettings() {

        }

    public:
        unsigned int Interpolation;
        unsigned int Wrapping;
};
class Texture2D {
    public:
        static Texture2D* Create();
        static Texture2D* Create(const std::string& a_Filename, const TextureSettings& a_Settings = TextureSettings());
        virtual ~Texture2D() {

        }

        virtual void BufferData(const std::string& a_Filename, const TextureSettings& a_Settings = TextureSettings()) {

        }

        virtual void Bind(unsigned int index = 0) const {

        }

};
class OpenGLTexture2D: public Texture2D {
    public:
        OpenGLTexture2D() {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        }
        OpenGLTexture2D(const std::string& a_Filename, const TextureSettings& a_Settings) {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            BufferData(a_Filename, a_Settings);
        }
        ~OpenGLTexture2D() {
            glDeleteTextures(1, &m_RendererID);
        }

        void BufferData(const std::string& a_Filename, const TextureSettings& a_Settings) {
            SDL_Surface* f_Surface = IMG_Load(a_Filename.c_str());

            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, a_Settings.Interpolation);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, a_Settings.Interpolation);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, a_Settings.Wrapping);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, a_Settings.Wrapping);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, f_Surface->w, f_Surface->h, 0, (f_Surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, f_Surface->pixels);

            SDL_FreeSurface(f_Surface);
        }

        void Bind(unsigned int index = 0) const {
            assert(index < 31);
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
        }

        unsigned int GetRendererID() {
            return m_RendererID;
        }

    private:
        unsigned int m_RendererID;
};
Texture2D* Texture2D::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLTexture2D();
    }
}
Texture2D* Texture2D::Create(const std::string& a_Filename, const TextureSettings& a_Settings) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return new OpenGLTexture2D(a_Filename, a_Settings);
    }
}

class Renderer {
    public:
        static void Clear(const glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
            glClearColor(Color.r, Color.g, Color.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        static void Draw(const VertexBuffer* a_VertexBuffer, const Shader* a_Shader) {
            a_Shader->Bind();
            a_VertexBuffer->Bind();
            if(RendererAPI::GetAPI() == API::OpenGL) {
                glDrawArrays(GL_TRIANGLES, 0, a_VertexBuffer->GetCount());
            }

            #ifdef VENUM_DEBUG_EXTREME
            std::cout << "OpenGL Renderer: " << "Drawn! " << std::endl;
            #endif // VENUM_DEBUG_EXTREME
        }
};

class Collider {
    public:
        Collider(unsigned int a_Type = POINT, glm::vec3 a_Position = glm::vec3(0.0f), glm::vec3 a_Size = glm::vec3(1.0f), float a_Radius = 1.0f) {
            Type = a_Type;
            Position = a_Position;
            Size = a_Size;
            Radius = a_Radius;
        }
        Collider(glm::vec3 a_Position) {
            Type = POINT;
            Position = a_Position;
            Size = glm::vec3(0.0f);
            Radius = 0.0f;
        }
        Collider(glm::vec3 a_Position, float a_Radius) {
            Type = CIRCLE;
            Position = a_Position;
            Size = glm::vec3(0.0f);
            Radius = a_Radius;
        }
        Collider(glm::vec3 a_Position, glm::vec3 a_Size) {
            Type = BOX;
            Position = a_Position;
            Size = a_Size;
            Radius = 0.0f;
        }
        ~Collider() {

        }

        bool Intersects(const Collider& a_Other) const {
            if(Type == POINT && a_Other.Type == POINT) {
                return (a_Other.Position == Position);
            }
            else if(Type == POINT && a_Other.Type == CIRCLE) {
                return (glm::distance(Position, a_Other.Position) <= a_Other.Radius);
            }
            else if(Type == POINT && a_Other.Type == BOX) {
                return (Position.x >= a_Other.Position.x && Position.x <= a_Other.Position.x + a_Other.Size.x) &&
                        (Position.y >= a_Other.Position.y && Position.y <= a_Other.Position.y + a_Other.Size.y) &&
                        (Position.z >= a_Other.Position.z && Position.z <= a_Other.Position.z + a_Other.Size.z);
            }
            else if(Type == CIRCLE && a_Other.Type == POINT) {
                return (glm::distance(a_Other.Position, Position) <= Radius);
            }
            else if(Type == CIRCLE && a_Other.Type == CIRCLE) {
                return (glm::distance(a_Other.Position, Position) <= a_Other.Radius + Radius);
            }
            else if(Type == CIRCLE && a_Other.Type == BOX) {
                glm::vec3 circleDistance;
                circleDistance.x = std::abs(Position.x - a_Other.Position.x);
                circleDistance.y = std::abs(Position.y - a_Other.Position.y);
                circleDistance.z = std::abs(Position.z - a_Other.Position.z);

                if (circleDistance.x > (a_Other.Size.x/2.0f + Radius)) {
                    return false;
                }
                if (circleDistance.y > (a_Other.Size.y/2.0f + Radius)) {
                    return false;
                }
                if (circleDistance.z > (a_Other.Size.z/2.0f + Radius)) {
                    return false;
                }

                if (circleDistance.x <= (a_Other.Size.x/2.0f)) {
                    return true;
                }
                if (circleDistance.y <= (a_Other.Size.y/2.0f)) {
                    return true;
                }
                if (circleDistance.z <= (a_Other.Size.z/2.0f)) {
                    return true;
                }

                float cornerDistance_sq = std::pow(circleDistance.x - a_Other.Size.x/2.0f, 2) +
                                     std::pow(circleDistance.y - a_Other.Size.y/2.0f, 2) +
                                     std::pow(circleDistance.z - a_Other.Size.z/2.0f, 2);

                return (cornerDistance_sq <= pow(Radius, 2));
            }
            else if(Type == BOX && a_Other.Type == POINT) {
                return (a_Other.Position.x >= Position.x && a_Other.Position.x <= Position.x + Size.x) &&
                        (a_Other.Position.y >= Position.y && a_Other.Position.y <= Position.y + Size.y) &&
                        (a_Other.Position.z >= Position.z && a_Other.Position.z <= Position.z + Size.z);
            }
            else if(Type == BOX && a_Other.Type == CIRCLE) {
                glm::vec3 circleDistance;
                circleDistance.x = std::abs(a_Other.Position.x - Position.x);
                circleDistance.y = std::abs(a_Other.Position.y - Position.y);
                circleDistance.z = std::abs(a_Other.Position.z - Position.z);

                if (circleDistance.x > (Size.x/2.0f + a_Other.Radius)) {
                    return false;
                }
                if (circleDistance.y > (Size.y/2.0f + a_Other.Radius)) {
                    return false;
                }
                if (circleDistance.z > (Size.z/2.0f + a_Other.Radius)) {
                    return false;
                }

                if (circleDistance.x <= (Size.x/2.0f)) {
                    return true;
                }
                if (circleDistance.y <= (Size.y/2.0f)) {
                    return true;
                }
                if (circleDistance.z <= (Size.z/2.0f)) {
                    return true;
                }

                float cornerDistance_sq = std::pow(circleDistance.x - Size.x/2.0f, 2) +
                                     std::pow(circleDistance.y - Size.y/2.0f, 2) +
                                     std::pow(circleDistance.z - Size.z/2.0f, 2);

                return (cornerDistance_sq <= pow(a_Other.Radius, 2));
            }
            else if(Type == BOX && a_Other.Type == BOX) {
                return (Position.x <= a_Other.Position.x + a_Other.Size.x && Position.x + Size.x >= a_Other.Position.x) &&
                        (Position.y <= a_Other.Position.y + a_Other.Size.y && Position.y + Size.y >= a_Other.Position.y) &&
                        (Position.z <= a_Other.Position.z + a_Other.Size.z && Position.z + Size.z >= a_Other.Position.z);
            }
            else {
                return false;
            }
        }

    public:
        enum {
            POINT = 0,
            CIRCLE = 1,
            BOX = 2,
        };

        unsigned int Type;

    public:
        glm::vec3 Position;
        glm::vec3 Size;
        float Radius;
};
class PhysicsBody {
    public:
        PhysicsBody(const Collider& a_Collider = Collider()) {
            m_Collider = a_Collider;
        }
        ~PhysicsBody() {

        }

        const Collider& GetCollider() const {
            return m_Collider;
        }
        const std::vector<glm::vec3>& GetForces() const {
            return m_Forces;
        }

        Collider& GetColliderRef() {
            return m_Collider;
        }
        std::vector<glm::vec3>& GetForcesRef() {
            return m_Forces;
        }

        void SetCollider(const Collider& a_Collider) {
            m_Collider = a_Collider;
        }

    private:
        Collider m_Collider;
        std::vector<glm::vec3> m_Forces;
};
class SimplePhysics {
    public:
        SimplePhysics() {

        }
        ~SimplePhysics() {

        }

        void Update(float a_DeltaTime = 1.0f) {
            if(m_PhysicsBodys.size() > 1) {
                for(unsigned int i = 0; i < m_PhysicsBodys.size(); i++) {
                    bool f_IsColliding[m_PhysicsBodys[i]->GetForces().size()];
                    for(unsigned int k = 0; k < m_PhysicsBodys[i]->GetForces().size(); k++) {
                        f_IsColliding[k] = false;
                    }

                    for(unsigned int j = 0; j < m_PhysicsBodys.size(); j++) {
                        if(i != j) {
                            for(unsigned int k = 0; k < m_PhysicsBodys[i]->GetForces().size(); k++) {
                                Collider f_Tester = m_PhysicsBodys[i]->GetCollider();
                                f_Tester.Position += (m_PhysicsBodys[i]->GetForces()[k] * glm::vec3(a_DeltaTime)) / glm::vec3(static_cast<float>(m_PhysicsBodys.size()) - 1.0f);

                                if(f_Tester.Intersects(m_PhysicsBodys[j]->GetCollider())) {
                                    f_IsColliding[k] = true;
                                }
                            }
                        }
                    }

                    for(unsigned int j = 0; j < m_PhysicsBodys.size(); j++) {
                        if(i != j) {
                            for(unsigned int k = 0; k < m_PhysicsBodys[i]->GetForces().size(); k++) {
                                if(!f_IsColliding[k]) {
                                    m_PhysicsBodys[i]->GetColliderRef().Position += (m_PhysicsBodys[i]->GetForces()[k] * glm::vec3(a_DeltaTime)) / glm::vec3(static_cast<float>(m_PhysicsBodys.size()) - 1.0f);
                                }
                            }
                        }
                    }
                }
            }
            else if(m_PhysicsBodys.size() == 1) {
                for(unsigned int k = 0; k < m_PhysicsBodys[0]->GetForces().size(); k++) {
                    m_PhysicsBodys[0]->GetColliderRef().Position += m_PhysicsBodys[0]->GetForces()[k] * glm::vec3(a_DeltaTime);
                }
            }
        }

        const std::vector<PhysicsBody*>& GetPhysicsBodys() const {
            return m_PhysicsBodys;
        }

        std::vector<PhysicsBody*>& GetPhysicsBodysRef() {
            return m_PhysicsBodys;
        }

    private:
        std::vector<PhysicsBody*> m_PhysicsBodys;
};

#endif // ENGINE_HPP_INCLUDED
