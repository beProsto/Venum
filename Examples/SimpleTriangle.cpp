#include "Engine.hpp"

int main(int argc, char** argv) {
    Venum::Renderer* Renderer = Venum::Renderer::Create(Venum::API::OpenGL);
    Venum::Window Window("Venum Simple Triangle Example");
    SDL_Event Event;

    std::string VertexShaderSource = "\n\
    #version 330 \n\
    \n\
    layout(location = 0) in vec3 a_Position; \n\
    layout(location = 1) in vec2 a_TexCoord; \n\
    layout(location = 2) in vec3 a_Normal; \n\
    \n\
    void main() { \n\
        gl_Position = vec4(a_Position, 1.0f); \n\
    } ";

    std::string FragmentShaderSource = "\n\
    #version 330 \n\
    \n\
    out vec4 o_Color; \n\
    \n\
    void main() { \n\
        o_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f); \n\
    } ";

    Venum::Shader* TestShader = Renderer->ShaderCreate(Renderer->ShaderElementCreate(GL_VERTEX_SHADER, VertexShaderSource), 
                                                        Renderer->ShaderElementCreate(GL_FRAGMENT_SHADER, FragmentShaderSource));

    Venum::Vertex3D TestVertexBufferData[] = {
        Venum::Vertex3D(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
        Venum::Vertex3D(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        Venum::Vertex3D(1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
    };

    Venum::VertexBuffer* TestVertexBuffer = Renderer->VertexBufferCreate(TestVertexBufferData, 3);
    
    Renderer->Init();
    
    while(Window.Running()) {
        Window.PollEvent(Event);

        Renderer->Clear();

        Renderer->Draw(TestVertexBuffer, TestShader);

        Window.SwapBuffers();
    }
    
    delete TestShader;
    delete TestVertexBuffer;
    delete Renderer;

    return 0;
}
