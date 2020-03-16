#include "Vendor.h"
#include "Window.h"
#include "Shader.h"
#include "Utility.h"
#include "VertexArray.h"
#include "Buffer.h"

typedef  struct {
    GLuint  count;
    GLuint  primCount;
    GLuint  firstIndex;
    GLuint  baseVertex;
    GLuint  baseInstance;
} glDrawElementsIndirectCommand;

void APIENTRY opengl_error_callback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* msg,
	const void* data
) {
	char message[BUFSIZ];

	const char* _source;
	const char* _type;
	const char* _severity;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW_SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER_COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD_PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		_source = "OTHER";
		break;

	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED_BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED_BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		_severity = "HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;

	default:
		_severity = "UNKNOWN";
		break;
	}

	std::snprintf(message, BUFSIZ, "%s (%d) of %s severity, raised from %s: %s\n", _type, id, _severity, _source, msg);

	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		throw std::runtime_error(message);
}

int main()
{
    WindowManager::init();
    Window::CreationHints hints;
    hints.context_version_major = 4;
    hints.context_version_minor = 6;
    hints.opengl_profile = Window::OpenGLProfile::CORE;
#ifndef NDEBUG
	hints.opengl_debug_context = true;
#endif // !NDEBUG

	Window wnd{ {}, hints };
    
    wnd.get_context().make_current();

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

#ifndef NDEBUG
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengl_error_callback, NULL);
#endif // DEBUG

    ShaderProgram program{ Shader{read_file("res/sample.vert").c_str(), GL_VERTEX_SHADER}, Shader{read_file("res/sample.frag").c_str(), GL_FRAGMENT_SHADER} };

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    
    VertexArray VAO;
    Buffer VBO, EBO;

    VBO.data(sizeof(vertices), vertices, GL_STATIC_DRAW);
    EBO.data(sizeof(indices), indices, GL_STATIC_DRAW);
    
    const unsigned int VBO_IDX = 0;
    const VertexAttribute POS_IDX = program.get_attrib_location("a_pos");
    VAO.vertex_buffer(VBO_IDX, VBO, 0, 3 * sizeof(float));
    VAO.enable_attrib(POS_IDX);
    VAO.attrib_binding(POS_IDX, VBO_IDX);
    VAO.attrib_format(POS_IDX, 3, GL_FLOAT, GL_FALSE, 0);

    VAO.element_buffer(EBO);

	glDrawElementsIndirectCommand commands[] =
	{
		{
			sizeof(indices) / sizeof(unsigned int),
			1,
			0,
			0,
			0
		}
	};

	Buffer indirect;
	indirect.data(sizeof(commands), &commands, GL_STATIC_DRAW);
	indirect.bind(GL_DRAW_INDIRECT_BUFFER);

    while (!wnd.should_close())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        program.use();
        VAO.bind();

		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, sizeof(commands) / sizeof(glDrawElementsIndirectCommand), 0);

        wnd.swap_buffers();
        WindowManager::poll_events();
    }
}