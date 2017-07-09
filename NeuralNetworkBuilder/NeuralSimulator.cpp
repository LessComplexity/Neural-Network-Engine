/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NeuralSimulator CLASS                                                                                      */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "NeuralSimulator.h"

// Init the display
NeuralSimulator::NeuralSimulator()
{
}

NeuralSimulator::~NeuralSimulator()
{
}

// Initialize window
void NeuralSimulator::createWindow(const char * title, int width, int height, bool full_screen)
{
	// Store variables
	this->title = title;
	this->width = width;
	this->height = height;
	this->full_screen = full_screen;

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "[-] Couldn't initialize GLFW" << std::endl;
	}

	// Set default values
	//std::cout << *glGetString(GL_VERSION);

	glfwWindowHint(GLFW_SAMPLES, this->anti_aliasing); // Set Antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->ogl_version.first);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->ogl_version.second);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	// Open a window and create its OpenGL context
	this->window = glfwCreateWindow(width, height, title, NULL, NULL);
	// If window can't be created then terminate
	if (this->window == NULL)
	{
		std::cout << "[-] Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "[-] Failed to initialize GLEW" << std::endl;
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

// Render scene
void NeuralSimulator::startRendering()
{
	// User defined initialization
	//this->before_render();

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// User-defined render loop
		glUseProgram(this->programs[this->used_program_id]);
		//this->render_loop();

		// Draw user defined objects
		for (Drawable d : this->drawables)
			d.draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

// Set antialiasing number (xaa)
void NeuralSimulator::setAntialiasing(int aa)
{
	this->anti_aliasing = aa;
}

// Set openGL version
void NeuralSimulator::setOGLVersion(int major, int minor)
{
	this->ogl_version.first = major;
	this->ogl_version.second = minor;
}

// Set render loop
void NeuralSimulator::setRenderLoop(std::function<void(void)> render_loop)
{
	this->render_loop = render_loop;
}

void NeuralSimulator::setBeforeRenderer(std::function<void(void)> before_render)
{
	this->before_render = before_render;
}

int NeuralSimulator::addShadingProgram(const char * vertexShader, const char * fragmentShader)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexShader, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		std::cout << "[-] Impossible to open " << vertexShader << ". Are you in the right directory ? Don't forget to read the FAQ !" << std::endl;
		getchar();
		return -1;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentShader, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;
	
	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}

	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// Insert program id
	this->programs.push_back(ProgramID);

	// Return ID
	return (int) this->programs.size() - 1;
}

void NeuralSimulator::setProgram(int program_id)
{
	if (program_id > this->programs.size() - 1)
	{
		std::cout << "[-] Program ID not found";
		return;
	}

	this->used_program_id = program_id;
}

void NeuralSimulator::createDrawingContext()
{
	// Create a new vertex array
	this->vertex_array_objects.push_back(GLuint());
	glGenVertexArrays(1, &this->vertex_array_objects.back());
	glBindVertexArray(this->vertex_array_objects.back());
}

void NeuralSimulator::addDrawable(Vertices vertices)
{
	// Create a vertex buffer
	this->drawables.push_back(Drawable(vertices));
}

void NeuralSimulator::addDrawable(Drawable drawable)
{
	this->drawables.push_back(drawable);
}

std::function<void(void)> NeuralSimulator::getRenderLoop()
{
	return this->render_loop;
}

std::function<void(void)> NeuralSimulator::getBeforeRenderer()
{
	return this->before_render;
}

std::pair<int, int> NeuralSimulator::getOGLVersion()
{
	return this->ogl_version;
}
