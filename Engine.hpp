#ifndef ENGINE_HPP_INCLUDED
#define ENGINE_HPP_INCLUDED

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/transform.hpp>

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

        return f_Result;
    }
    else {
        std::cerr << "Unable to load file: '" << a_Filename << "'!" << std::endl;
        return "";
    }
}
template <typename T> T Map(T a_Value, T a_Start1, T a_Stop1, T a_Start2, T a_Stop2) {
	return ((a_Value - a_Start1) / (a_Stop1 - a_Start1)) * (a_Stop2 - a_Start2) + a_Start2;
}
template <typename T> T Constrain(T n, T start, T stop) {
    if(n < start) {
        return start;
    }
    else if(n > stop) {
        return stop;
    }
    else {
        return n;
    }
}

enum class API {
    OpenGL = 0,
};
class RendererAPI {
    public:
        static API GetAPI() {
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
        }
        void Update(glm::vec2 a_Position, glm::vec2 a_Size) {
            Position = a_Position;
            Size = a_Size;
            if(RendererAPI::GetAPI() == API::OpenGL) {
                glViewport(Position.x, Position.y, Size.x, Size.y);
            }
        }

        float GetAspect() {
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
            SDL_GL_DeleteContext(m_Context);
            SDL_DestroyWindow(m_Window);
            Mix_Quit();
            IMG_Quit();
            SDL_Quit();
        }

        bool Create(const std::string& a_Title, unsigned int a_Width, unsigned int a_Height, unsigned int a_Flags) {
            SDL_Init(SDL_INIT_EVERYTHING);
            IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);
            Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID | MIX_INIT_MOD | MIX_INIT_OPUS);
            if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                return false;
            }

            m_Window = SDL_CreateWindow(a_Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, a_Width, a_Height, (RendererAPI::GetAPI() == API::OpenGL ? SDL_WINDOW_OPENGL : SDL_WINDOW_VULKAN) | a_Flags);

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

            Viewport.Update(glm::vec2i(0, 0), glm::vec2ui(GetWidth(), GetHeight()));

            return true;
        }

        bool Running() {
            return m_Running;
        }
        void Close() {
            m_Running = false;
        }

        bool PollEvent(SDL_Event* a_Event) {
            if(SDL_PollEvent(a_Event)) {
                if(a_Event->type == SDL_QUIT) {
                    m_Running = false;
                }
                return true;
            }
            else {
                return false;
            }
        }

        void SwapBuffers() {
            SDL_GL_SwapWindow(m_Window);
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
            m_Position = glm::vec2i(0, 0);
            m_LastPosition = glm::vec2i(0, 0);
            m_Visible = true;
        }
        ~MouseClass() {

        }

        glm::vec2i GetPosition() {
            m_LastPosition = m_Position;
            SDL_GetMouseState(&m_Position.x, &m_Position.y);

            return m_Position;
        }
        glm::vec2i GetPositionRelative() {
            m_LastPosition = m_Position;
            SDL_GetMouseState(&m_Position.x, &m_Position.y);

            return m_Position - m_LastPosition;
        }
        bool GetVisibility() {
            return m_Visible;
        }

        void SetPosition(glm::vec2i a_Position) {
            SDL_WarpMouseInWindow(Window.GetWindow(), a_Position.x, a_Position.y);
        }
        void SetVisibility(bool a_Visible) {
            SDL_ShowCursor(a_Visible);
            m_Visible = a_Visible;
        }

    private:
        glm::vec2i m_Position;
        glm::vec2i m_LastPosition;
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

class Music {
    public:
        Music() : m_Sample(NULL) {

        }
        Music(const std::string& a_Filename) : m_Sample(NULL) {
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

        bool IsPlayed() {
            return Mix_PlayingMusic();
        }
        bool IsPaused() {
            return Mix_PausedMusic();
        }
        bool IsStopped() {
            return !IsPlayed();
        }
        int GetVolume() {
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
        Sound(int a_Channel = 0) : m_Sample(NULL) {
            m_Channel = a_Channel;
        }
        Sound(const std::string& a_Filename, int a_Channel = 0) : m_Sample(NULL) {
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

        bool IsPlayed() {
            return Mix_Playing(m_Channel);
        }
        bool IsPaused() {
            return Mix_Paused(m_Channel);
        }
        bool IsStopped() {
            return !IsPlayed();
        }
        int GetVolume() {
            return Mix_Volume(m_Channel, -1);
        }
        int GetChannel() {
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

class Transform3D {
    public:
        Transform3D(glm::vec3 a_Position = glm::vec3(0.0f), glm::vec3 a_Rotation = glm::vec3(0.0f), glm::vec3 a_Scale = glm::vec3(1.0f)) {
            Position = a_Position;
            Rotation = a_Rotation;
            Scale = a_Scale;
        }

        glm::mat4 GetMatrix() const {
            glm::mat4 f_RotationX = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 f_RotationY = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 f_RotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::mat4 f_Rotation = f_RotationZ * f_RotationY * f_RotationX;
            glm::mat4 f_Position = glm::translate(glm::mat4(1.0f), Position);
            glm::mat4 f_Scale = glm::scale(glm::mat4(1.0f), Scale);

            return f_Position * f_Rotation * f_Scale;
        }

    public:
        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
};
class Camera {
    public:
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
            glm::mat4 f_Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::vec3 f_Direction = f_Rotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
            glm::vec3 f_Upside = f_Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

            return glm::perspective(glm::radians(FOV), Viewport.GetAspect(), Near, Far) * glm::lookAt(Transform.Position, Transform.Position + f_Direction, f_Upside);
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
            glm::mat4 f_Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(Transform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::vec3 f_Direction = f_Rotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
            glm::vec3 f_Upside = f_Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

            return glm::ortho(LeftDown.x, RightTop.x, LeftDown.y, RightTop.y, Near, Far) * glm::lookAt(Transform.Position, Transform.Position + f_Direction, f_Upside);
        }

    public:
        glm::vec2 LeftDown;
        glm::vec2 RightTop;
        float Near, Far;
};

class Vertex {
    public:
        glm::vec3 Position;
        glm::vec2 TexCoord;
        glm::vec3 Normal;
};
class VertexBuffer {
    public:
        static VertexBuffer Create();
        static VertexBuffer Create(const std::vector<Vertex>& a_Data);
        static VertexBuffer Create(Vertex* a_Data, unsigned int a_Count);
        virtual ~VertexBuffer() {

        }

        virtual void BufferData(const std::vector<Vertex>& a_Data) {

        }
        virtual void BufferData(Vertex* a_Data, unsigned int a_Count) {

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

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
            glEnableVertexAttribArray(2);

            m_Count = 0;
        }
        OpenGLVertexBuffer(const std::vector<Vertex>& a_Data) {
            glCreateVertexArrays(1, &m_VARendererID);
            glBindVertexArray(m_VARendererID);

            glCreateBuffers(1, &m_VBRendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
            glEnableVertexAttribArray(2);

            BufferData(a_Data);
        }
        OpenGLVertexBuffer(Vertex* a_Data, unsigned int a_Count) {
            glCreateVertexArrays(1, &m_VARendererID);
            glBindVertexArray(m_VARendererID);

            glCreateBuffers(1, &m_VBRendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, TexCoord)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
            glEnableVertexAttribArray(2);

            BufferData(a_Data, a_Count);
        }
        ~OpenGLVertexBuffer() {
            glDeleteBuffers(1, &m_VBRendererID);
            glDeleteVertexArrays(1, &m_VARendererID);
        }

        void BufferData(const std::vector<Vertex>& a_Data) {
            Bind();
            m_Count = a_Data.size();
            glBufferData(GL_ARRAY_BUFFER, a_Data.size() * sizeof(Vertex), &a_Data[0], GL_STATIC_DRAW);
        }
        void BufferData(Vertex* a_Data, unsigned int a_Count) {
            Bind();
            m_Count = a_Count;
            glBufferData(GL_ARRAY_BUFFER, a_Count * sizeof(Vertex), a_Data, GL_STATIC_DRAW);
        }

        void Bind() const {
            glBindVertexArray(m_VARendererID);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBRendererID);
        }

        unsigned int GetVertexArrayRendererID() const {
            return m_VARendererID;
        }
        unsigned int GetVertexBufferRendererID() const {
            return m_VBRendererID;
        }
        unsigned int GetCount() const {
            return m_Count;
        }

    private:
        unsigned int m_VBRendererID;
        unsigned int m_VARendererID;
        unsigned int m_Count;
};
VertexBuffer VertexBuffer::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return OpenGLVertexBuffer();
    }
}
VertexBuffer VertexBuffer::Create(const std::vector<Vertex>& a_Data) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return OpenGLVertexBuffer(a_Data);
    }
}
VertexBuffer VertexBuffer::Create(Vertex* a_Data, unsigned int a_Count) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return OpenGLVertexBuffer(a_Data, a_Count);
    }
}

class Shader {
    public:
        static Shader Create();
        static Shader Create(const std::string& a_Code);
        virtual ~Shader() {

        }

        virtual void Compile(const std::string& a_Code) {

        }

        virtual void Bind() const {

        }
};
class OpenGLShader: public Shader {
    public:
        OpenGLShader() {
            m_RendererID = glCreateProgram();
            Bind();
        }
        OpenGLShader(const std::string& a_Code) {
            m_RendererID = glCreateProgram();
            Bind();

            Compile(a_Code);
        }
        ~OpenGLShader() {
            glDeleteProgram(m_RendererID);
        }

        void Compile(const std::string& a_Code) {
            std::strstream f_Code;
            f_Code << a_Code;

            std::string f_ShaderCode[2];

            std::string f_Line;
            unsigned int f_Parsed = 0;
            while(std::getline(f_Code, f_Line)) {
                if(f_Line == "#Shader Vertex") {
                    f_Parsed = 1;
                }
                else if(f_Line == "#Shader Fragment") {
                    f_Parsed = 2;
                }
                else {
                    if(f_Parsed == 1) {
                        f_ShaderCode[0] += f_Line;
                        f_ShaderCode[0] += '\n';
                    }
                    else if(f_Parsed == 2) {
                        f_ShaderCode[1] += f_Line;
                        f_ShaderCode[1] += '\n';
                    }
                }
            }

            unsigned int f_ShaderID[2];

            f_ShaderID[0] = CompileShader(GL_VERTEX_SHADER, f_ShaderCode[0]);
            f_ShaderID[1] = CompileShader(GL_VERTEX_SHADER, f_ShaderCode[1]);

            glAttachShader(m_RendererID, f_ShaderID[0]);
            glAttachShader(m_RendererID, f_ShaderID[1]);
            glLinkProgram(m_RendererID);
            glValidateProgram(m_RendererID);

            glDeleteShader(f_ShaderID[0]);
            glDeleteShader(f_ShaderID[1]);
        }

        void Bind() const {
            glUseProgram(m_RendererID);
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
        unsigned int CompileShader(unsigned int a_Type, const std::string& a_Code) {
            unsigned int f_RendererID = glCreateShader(a_Type);

            const char* f_Source = a_Code.c_str();
            glShaderSource(f_RendererID, 1, &f_Source, nullptr);

            glCompileShader(f_RendererID);

            int f_Compiled;
            glGetShaderiv(f_RendererID, GL_COMPILE_STATUS, &f_Compiled);
            if(f_Compiled == GL_FALSE) {
                int f_Length;
                glGetShaderiv(f_RendererID, GL_INFO_LOG_LENGTH, &f_Length);

                char* f_Message = new char[f_Length];
                glGetShaderInfoLog(f_RendererID, f_Length, &f_Length, f_Message);

                std::cerr << "[SHADER COMPILATION ERROR]" << f_Message << std::endl;

                delete[] f_Message;

                return 0;
            }

            return f_RendererID;
        }

    private:
        unsigned int m_RendererID;
};
Shader Shader::Create() {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return OpenGLShader();
    }
}
Shader Shader::Create(const std::string& a_Code) {
    if(RendererAPI::GetAPI() == API::OpenGL) {
        return OpenGLShader(a_Code);
    }
}

enum TextureSettings {
    InterpolationLinear = 1,
    InterpolationNearest = 2,
    WrapRepeat = 4,
    WrapClamp = 8,
};
class Texture2D {
    public:
        Texture2D() {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
        }
        Texture2D(const std::string& a_Filename, unsigned char a_Settings = TextureSettings::InterpolationLinear | TextureSettings::WrapRepeat) {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);
            BufferData(Settings, Filename);
        }
        ~Texture2D() {
            glDeleteTextures(1, &m_RendererID);
        }

        void BufferData(const std::string& a_Filename, unsigned char a_Settings = TextureSettings::InterpolationLinear | TextureSettings::WrapRepeat) {
            SDL_Surface*  m_Surface = IMG_Load(Filename.c_str());

            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Settings.MinMagFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Settings.MinMagFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Settings.WrapSTR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Settings.WrapSTR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Surface->w, m_Surface->h, 0, (m_Surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, m_Surface->pixels);

            if(m_Surface != NULL) {
                SDL_FreeSurface(m_Surface);
            }

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
        SDL_Surface* m_Surface;
};
class TextureCubeMap {
    public:
        OpenGLTextureCubeMap() {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        }
        OpenGLTextureCubeMap(const OpenGLTextureSettings& Settings, std::string Filename[6]) {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
            BufferData(Settings, Filename);
        }
        ~OpenGLTextureCubeMap() {
            glDeleteTextures(1, &m_RendererID);
        }

        void BufferData(const OpenGLTextureSettings& Settings, std::string Filename[6]) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, Settings.MinMagFilter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, Settings.MinMagFilter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, Settings.WrapSTR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, Settings.WrapSTR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, Settings.WrapSTR);

            for(unsigned int i = 0; i < 6; i++) {
                SDL_Surface* m_Surface = IMG_Load(Filename[i].c_str());
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, m_Surface->w, m_Surface->h, 0, (m_Surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, m_Surface->pixels);
                if(m_Surface != NULL) {
                    SDL_FreeSurface(m_Surface);
                }
            }


        }

        void Bind(unsigned int index = 0) const {
            assert(index < 31);
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        }

        unsigned int GetRendererID() const {
            return m_RendererID;
        }

    private:
        unsigned int m_RendererID;
};

class Mesh {
    public:
        Mesh() {
            m_VBO.reset(VertexBuffer::Create());
        }
        Mesh(const std::string a_Filename, bool a_DoBufferData = true) {
            m_VBO.reset(VertexBuffer::Create());
            LoadMeshFromFileOBJ(a_Filename, a_DoBufferData);
        }
        ~Mesh() {

        }

        void BufferData() {
            m_VBO->Bind();
            m_VBO->BufferData(m_VBOData);
            m_VBOData.clear();
        }

        bool LoadMeshFromFileOBJ(const std::string a_Filename, bool a_DoBufferData = true) {
            std::fstream filestream;

            filestream.open(a_Filename);

            std::vector<glm::vec3> arrayPosition;
            std::vector<glm::vec2> arrayTexCoord;
            std::vector<glm::vec3> arrayNormal;

            if( filestream.good(  ) ) {

                m_VBOData.clear(  );

                while( !filestream.eof(  ) ) {
                    std::strstream linestream;

                    std::string line;
                    std::getline( filestream, line );

                    for( char& i : line ) {
                        if( i == '/' ) {
                            i = ' ';
                        }
                    }

                    linestream << line;

                    if( line.c_str()[0] == 'v' && line.c_str()[1] == ' ' ) {
                        glm::vec3 data;
                        std::string junk;
                        linestream >> junk >> data.x >> data.y >> data.z;
                        arrayPosition.push_back( data );
                    }
                    else if( line.c_str()[0] == 'v' && line.c_str()[1] == 't' ) {
                        glm::vec2 data;
                        std::string junk;
                        linestream >> junk >> data.x >> data.y;
                        arrayTexCoord.push_back( data );
                    }
                    else if( line.c_str()[0] == 'v' && line.c_str()[1] == 'n' ) {
                        glm::vec3 data;
                        std::string junk;
                        linestream >> junk >> data.x >> data.y >> data.z;
                        arrayNormal.push_back( data );
                    }
                    else if(line.c_str()[0] == 'f' && line.c_str()[1] == ' ') {
                        std::string junk;
                        unsigned int indexPosition1, indexTexCoord1, indexNormal1,
                                    indexPosition2, indexTexCoord2, indexNormal2,
                                    indexPosition3, indexTexCoord3, indexNormal3;
                        linestream >> junk >> indexPosition1 >> indexTexCoord1 >> indexNormal1
                                >> indexPosition2 >> indexTexCoord2 >> indexNormal2
                                >> indexPosition3 >> indexTexCoord3 >> indexNormal3;

                        m_VBOData.push_back({arrayPosition[indexPosition1 - 1], arrayTexCoord[indexTexCoord1 - 1], arrayNormal[indexNormal1 - 1]});
                        m_VBOData.push_back({arrayPosition[indexPosition2 - 1], arrayTexCoord[indexTexCoord2 - 1], arrayNormal[indexNormal2 - 1]});
                        m_VBOData.push_back({arrayPosition[indexPosition3 - 1], arrayTexCoord[indexTexCoord3 - 1], arrayNormal[indexNormal3 - 1]});
                    }
                }
            }
            else
            {
                std::cerr << "Cannot load an obj file : " << a_Filename << '\n';
                return false;
            }

            if(a_DoBufferData) {
                BufferData();
            }

            return true;
        }

        const VertexBuffer& GetVertexBuffer() const {
            return *m_VBO;
        }
        const std::vector<Vertex>& GetVertexBufferData() const {
            return m_VBOData;
        }

        VertexBuffer& GetVertexBufferRef() {
            return *m_VBO;
        }
        std::vector<Vertex>& GetVertexBufferDataRef() {
            return m_VBOData;
        }

    private:
        std::unique_ptr<VertexBuffer> m_VBO;
        std::vector<Vertex> m_VBOData;
};
class OpenGLSimpleMaterial {
    public:
        OpenGLSimpleMaterial(Camera* CameraPtr = nullptr, Transform3D* TransformPtr = nullptr, Light2DManager* LightManagerPtr = nullptr, bool IsItTransparent = false, bool IsItDepthTested = true, bool IsItKeyColored = false, bool IsItLighted = false) {
            m_Shader.AttachShader(OpenGLShader(GL_VERTEX_SHADER, LoadStringFromFile("./res/sh/SimpleMaterial.vs")));
            m_Shader.AttachShader(OpenGLShader(GL_FRAGMENT_SHADER, LoadStringFromFile("./res/sh/SimpleMaterial.fs")));
            m_Shader.Compile();
            m_Transparent = IsItTransparent;
            m_DepthTested = IsItDepthTested;
            m_KeyColored = IsItKeyColored;
            m_Camera = CameraPtr;
            m_Transform = TransformPtr;

            SpriteSize = glm::vec2(1.0f);
            ChosenSprite = glm::vec2(0.0f);
            KeyColor = glm::vec4(1.0f);
            AmbientColor = glm::vec4(1.0f);
        }
        ~OpenGLSimpleMaterial() {

        }

        void Update() const {
            if(m_Camera != nullptr) {
                m_Shader.SetUniform("u_ProjectionView", m_Camera->GetMatrix());
            }
            else {
                m_Shader.SetUniform("u_ProjectionView", glm::mat4(1.0f));
            }
            if(m_Transform != nullptr) {
                m_Shader.SetUniform("u_Model", m_Transform->GetMatrix());
            }
            else  {
                m_Shader.SetUniform("u_Model", glm::mat4(1.0f));
            }
            m_Shader.SetUniform("u_IsAlpha", m_Transparent);
            m_Shader.SetUniform("u_KeyColored", m_KeyColored);

            m_Shader.SetUniform("u_AlbedoTextureID", Textures::ALBEDO);

            m_Shader.SetUniform("u_SpriteSize", SpriteSize);
            m_Shader.SetUniform("u_SpriteToShow", ChosenSprite);

            m_Shader.SetUniform("u_KeyColor", KeyColor);
            m_Shader.SetUniform("u_AmbientColor", AmbientColor);

        }

        const OpenGLShaderProgram& GetShader() const {
            return m_Shader;
        }
        bool IsTransparent() const {
            return m_Transparent;
        }
        bool IsDepthTested() const {
            return m_DepthTested;
        }
        bool IsKeyColored() const {
            return m_KeyColored;
        }

        OpenGLShaderProgram& GetShaderRef() {
            return m_Shader;
        }

        void SetTransparent(bool Transparent) {
            m_Transparent = Transparent;
        }
        void SetDepthTested(bool DepthTested) {
            m_DepthTested = DepthTested;
        }
        void SetKeyColored(bool KeyColored) {
            m_KeyColored = KeyColored;
        }
        void SetCameraPtr(Camera& CameraPtr) {
            m_Camera = &CameraPtr;
        }
        void SetTransformPtr(Transform3D& TransformPtr) {
            m_Transform = &TransformPtr;
        }

    private:
        Shader* m_Shader;
        bool m_Transparent;
        bool m_DepthTested;
        bool m_KeyColored;

        Camera* m_Camera;
        Transform3D* m_Transform;


    public:
        enum Textures {
            ALBEDO = 0,
        };

        glm::vec2 SpriteSize;
        glm::vec2 ChosenSprite;

        glm::vec4 KeyColor;
        glm::vec4 AmbientColor;
};
class OpenGLModel {
    public:
        OpenGLModel() {

        }
        OpenGLModel(const std::string& Filename, bool DoBufferData = false) {
            LoadModelFromFileOBJ(Filename, DoBufferData);
        }
        ~OpenGLModel() {

        }

        void LoadModelFromFileOBJ(const std::string& Filename, bool DoBufferData = false) {
            m_Mesh.LoadMeshFromFileOBJ(Filename);
            if(DoBufferData) {
                BufferData();
            }
        }

        void BufferData() {
            m_Mesh.BufferData();
        }
        void Update() const {
            m_Material.Update();
        }

        const OpenGLMesh& GetMesh() const {
            return m_Mesh;
        }
        const OpenGLSimpleMaterial& GetMaterial() const {
            return m_Material;
        }

        OpenGLMesh& GetMeshRef() {
            return m_Mesh;
        }
        OpenGLSimpleMaterial& GetMaterialRef() {
            return m_Material;
        }

    private:
        OpenGLMesh m_Mesh;
        OpenGLSimpleMaterial m_Material;
};

class OpenGLSkybox {
    public:
        OpenGLSkybox(Camera* CameraPtr = nullptr) {
            Vertex VBOData[] = {
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}
            };

            m_VAO.BufferData(m_VBO, {0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Position)});
            m_VAO.BufferData(m_VBO, {1, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, TexCoord)});
            m_VAO.BufferData(m_VBO, {2, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Normal)});

            m_VBO.BufferData(VBOData, 36);

            m_Shader.AttachShader(OpenGLShader(GL_VERTEX_SHADER, LoadStringFromFile("./res/sh/Skybox.vs")));
            m_Shader.AttachShader(OpenGLShader(GL_FRAGMENT_SHADER, LoadStringFromFile("./res/sh/Skybox.fs")));
            m_Shader.Compile();

            CustomModelMatrix = glm::mat4(1.0f);

            m_Camera = CameraPtr;
        }
        OpenGLSkybox(const OpenGLTextureSettings& Settings, std::string Filename[6], Camera* CameraPtr = nullptr) {
            Vertex VBOData[] = {
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},

                {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)},
                {glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)}
            };

            m_VAO.BufferData(m_VBO, {0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Position)});
            m_VAO.BufferData(m_VBO, {1, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, TexCoord)});
            m_VAO.BufferData(m_VBO, {2, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, Normal)});

            m_VBO.BufferData(VBOData, 36);

            m_Shader.AttachShader(OpenGLShader(GL_VERTEX_SHADER, LoadStringFromFile("./res/sh/Skybox.vs")));
            m_Shader.AttachShader(OpenGLShader(GL_FRAGMENT_SHADER, LoadStringFromFile("./res/sh/Skybox.fs")));
            m_Shader.Compile();

            m_Camera = CameraPtr;

            CustomModelMatrix = glm::mat4(1.0f);

            LoadTextures(Settings, Filename);
        }
        ~OpenGLSkybox() {

        }

        void LoadTextures(const OpenGLTextureSettings& Settings, std::string Filename[6]) {
            m_Texture.BufferData(Settings, Filename);
        }

        void Update()const {
            if(m_Camera != nullptr) {
                m_Shader.SetUniform("u_ProjectionView", m_Camera->GetMatrix());
                m_Shader.SetUniform("u_Model", glm::translate(glm::mat4(1.0f), m_Camera->Transform.Translation) * CustomModelMatrix);
            }
            else {
                m_Shader.SetUniform("u_ProjectionView", glm::mat4(1.0f));
                m_Shader.SetUniform("u_Model", glm::mat4(1.0f));
            }

            m_Shader.SetUniform("u_AlbedoTextureID", 0);
        }

        const OpenGLVertexBuffer& GetVertexBuffer() const {
            return m_VBO;
        }
        const OpenGLTextureCubeMap& GetTexture() const {
            return m_Texture;
        }
        const OpenGLShaderProgram& GetShader() const {
            return m_Shader;
        }

        OpenGLVertexBuffer& GetVertexBufferRef() {
            return m_VBO;
        }
        OpenGLTextureCubeMap& GetTextureRef() {
            return m_Texture;
        }
        OpenGLShaderProgram& GetShaderRef() {
            return m_Shader;
        }

        void SetCameraPtr(Camera* a_Camera) {
            m_Camera = a_Camera;
        }

    public:
        glm::mat4 CustomModelMatrix;

    private:
        OpenGLVertexBuffer m_VBO;
        OpenGLShaderProgram m_Shader;
        OpenGLTextureCubeMap m_Texture;
        Camera* m_Camera;
};

class OpenGLRenderer {
    public:
        static void Clear(const glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) {
            glClearColor( Color.x, Color.y, Color.z, 1.0f );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        }
        static void Draw(const OpenGLModel& Model) {
            if(Model.GetMaterial().IsDepthTested()) {
                glEnable(GL_DEPTH_TEST);
            }
            else {
                glDisable(GL_DEPTH_TEST);
            }
            Model.Update();
            Model.GetMaterial().GetShader().Bind();
            Model.GetMesh().GetVertexArray().Bind();
            Model.GetMesh().GetVertexBuffer().Bind();
            glDrawArrays(GL_TRIANGLES, 0, Model.GetMesh().GetVertexBuffer().GetCount());
        }
        static void Draw(const OpenGLSkybox& Skybox) {
            glDisable(GL_DEPTH_TEST);
            Skybox.Update();
            Skybox.GetShader().Bind();
            Skybox.GetVertexArray().Bind();
            Skybox.GetVertexBuffer().Bind();
            Skybox.GetTexture().Bind(0);
            glDrawArrays(GL_TRIANGLES, 0, Skybox.GetVertexBuffer().GetCount());
        }

};

class Collider {
    public:
        Collider(unsigned int TypeU = Types::POINT, glm::vec3 PositionU = glm::vec3(0.0f), glm::vec3 SizeU = glm::vec3(0.0f), float RadiusU = 0.0f) {
            Type = TypeU;
            Position = PositionU;
            Size = SizeU;
            Radius = RadiusU;
        }
        Collider(glm::vec3 PositionU) {
            Type = Types::POINT;
            Position = PositionU;
            Size = glm::vec3(0.0f);
            Radius = 0.0f;
        }
        Collider(glm::vec3 PositionU, float RadiusU) {
            Type = Types::CIRCLE;
            Position = PositionU;
            Size = glm::vec3(0.0f);
            Radius = RadiusU;
        }
        Collider(glm::vec3 PositionU, glm::vec3 SizeU) {
            Type = Types::BOX;
            Position = PositionU;
            Size = SizeU;
            Radius = 0.0f;
        }
        ~Collider() {

        }

        bool Intersects(const Collider& other) const {
            if(Type == Types::POINT && other.Type == Types::POINT) {
                return (other.Position == Position);
            }
            else if(Type == Types::POINT && other.Type == Types::CIRCLE) {
                return (glm::distance(Position, other.Position) <= other.Radius);
            }
            else if(Type == Types::POINT && other.Type == Types::BOX) {
                return (Position.x >= other.Position.x && Position.x <= other.Position.x + other.Size.x) &&
                        (Position.y >= other.Position.y && Position.y <= other.Position.y + other.Size.y) &&
                        (Position.z >= other.Position.z && Position.z <= other.Position.z + other.Size.z);
            }
            else if(Type == Types::CIRCLE && other.Type == Types::POINT) {
                return (glm::distance(other.Position, Position) <= Radius);
            }
            else if(Type == Types::CIRCLE && other.Type == Types::CIRCLE) {
                return (glm::distance(other.Position, Position) <= other.Radius + Radius);
            }
            else if(Type == Types::CIRCLE && other.Type == Types::BOX) {
                glm::vec3 circleDistance;
                circleDistance.x = std::abs(Position.x - other.Position.x);
                circleDistance.y = std::abs(Position.y - other.Position.y);
                circleDistance.z = std::abs(Position.z - other.Position.z);

                if (circleDistance.x > (other.Size.x/2.0f + Radius)) {
                    return false;
                }
                if (circleDistance.y > (other.Size.y/2.0f + Radius)) {
                    return false;
                }
                if (circleDistance.z > (other.Size.z/2.0f + Radius)) {
                    return false;
                }

                if (circleDistance.x <= (other.Size.x/2.0f)) {
                    return true;
                }
                if (circleDistance.y <= (other.Size.y/2.0f)) {
                    return true;
                }
                if (circleDistance.z <= (other.Size.z/2.0f)) {
                    return true;
                }

                float cornerDistance_sq = std::pow(circleDistance.x - other.Size.x/2.0f, 2) +
                                     std::pow(circleDistance.y - other.Size.y/2.0f, 2) +
                                     std::pow(circleDistance.z - other.Size.z/2.0f, 2);

                return (cornerDistance_sq <= pow(Radius, 2));
            }
            else if(Type == Types::BOX && other.Type == Types::POINT) {
                return (other.Position.x >= Position.x && other.Position.x <= Position.x + Size.x) &&
                        (other.Position.y >= Position.y && other.Position.y <= Position.y + Size.y) &&
                        (other.Position.z >= Position.z && other.Position.z <= Position.z + Size.z);
            }
            else if(Type == Types::BOX && other.Type == Types::CIRCLE) {
                glm::vec3 circleDistance;
                circleDistance.x = std::abs(other.Position.x - Position.x);
                circleDistance.y = std::abs(other.Position.y - Position.y);
                circleDistance.z = std::abs(other.Position.z - Position.z);

                if (circleDistance.x > (Size.x/2.0f + other.Radius)) {
                    return false;
                }
                if (circleDistance.y > (Size.y/2.0f + other.Radius)) {
                    return false;
                }
                if (circleDistance.z > (Size.z/2.0f + other.Radius)) {
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

                return (cornerDistance_sq <= pow(other.Radius, 2));
            }
            else if(Type == Types::BOX && other.Type == Types::BOX) {
                return (Position.x <= other.Position.x + other.Size.x && Position.x + Size.x >= other.Position.x) &&
                        (Position.y <= other.Position.y + other.Size.y && Position.y + Size.y >= other.Position.y) &&
                        (Position.z <= other.Position.z + other.Size.z && Position.z + Size.z >= other.Position.z);
            }
            else {
                return false;
            }
        }

    public:
        enum Types {
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
        PhysicsBody(const Collider& ColliderU = Collider()) {
            m_Collider = ColliderU;
        }
        ~PhysicsBody() {

        }

        void ApplyForce(const glm::vec3& Movement) {
            m_ForceApplied += Movement;
        }
        void SetForce(const glm::vec3& Force) {
            m_ForceApplied = Force;
        }

        const Collider& GetCollider() const {
            return m_Collider;
        }
        const glm::vec3& GetForceApplied() const {
            return m_ForceApplied;
        }

        Collider& GetColliderRef() {
            return m_Collider;
        }
        glm::vec3& GetForceAppliedRef() {
            return m_ForceApplied;
        }

    protected:
        friend class SimplePhysics;
        void Move(float dt) {
            m_Collider.Position += m_ForceApplied * dt;
        }


    private:
        Collider m_Collider;
        glm::vec3 m_ForceApplied;
};
class SimplePhysics {
    public:
        SimplePhysics() {

        }
        ~SimplePhysics() {

        }

        void ClearPhysicsBodyArray() {
            m_PhysicsBodyPtrs.clear();
        }
        void PushPhysicsBody(PhysicsBody& PB) {
            m_PhysicsBodyPtrs.push_back(&PB);
        }

        void Update(float dt) {
            for(unsigned int i = 0; i < m_PhysicsBodyPtrs.size(); i++) {
                m_PhysicsBodyPtrs[i]->Move(dt);
            }

            for(unsigned int i = 0; i < m_PhysicsBodyPtrs.size(); i++) {
                for(unsigned int j = i + 1; j < m_PhysicsBodyPtrs.size(); j++) {
                    if(m_PhysicsBodyPtrs[i]->GetCollider().Intersects(m_PhysicsBodyPtrs[j]->GetCollider())) {
                        m_PhysicsBodyPtrs[i]->Move(dt * -1.0f);
                        m_PhysicsBodyPtrs[j]->Move(dt * -1.0f);
                    }
                }
            }
        }


    private:
        std::vector<PhysicsBody*> m_PhysicsBodyPtrs;
};


#endif // ENGINE_HPP_INCLUDED
