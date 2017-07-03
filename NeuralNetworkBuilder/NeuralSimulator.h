/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NeuralSimulator HEADER                                                                                     */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "definitions.h"
#include "Drawable.h"

class NeuralSimulator
{
public:
	// Init the display
	NeuralSimulator();
	virtual ~NeuralSimulator();

	/* Helpers */
	// Initialize window
	void createWindow(const char * title, int width, int height, bool full_screen);
	// Render scene
	void startRendering();

	/* Setters */
	// Set antialiasing number (xaa)
	void setAntialiasing(int aa);
	// Set openGL version
	void setOGLVersion(int major, int minor);
	// Set render loop
	void setRenderLoop(std::function<void(void)> render_loop);
	// Set before rendering function
	void setBeforeRenderer(std::function<void(void)> before_render);
	// Set shaders
	int addShadingProgram(const char * vertexShader, const char * fragmentShader);
	// Set program to use
	void setProgram(int program_id);
	// Create a new drawing context (VAO)
	void createDrawingContext();
	// Add object to draw
	void addDrawable(Vertices points);
	// Add object to draw
	void addDrawable(Drawable drawable);

	/* Getters */
	std::function<void(void)> getRenderLoop();
	std::function<void(void)> getBeforeRenderer();
	std::pair<int, int> getOGLVersion();
private:
	// Other data
	GLFWwindow * window;
	int anti_aliasing = 4; // x4 by default
	std::pair<int, int> ogl_version = std::pair<int, int>(3,3);
	int width, height;
	bool full_screen = false;
	const char * title;

	// User set loop function
	std::function<void(void)> render_loop;
	// User set before loop function
	std::function<void(void)> before_render;

	// Program holder
	std::vector<GLuint> programs = std::vector<GLuint>();
	int used_program_id = 0	;

	// VAO's holder
	std::vector<GLuint> vertex_array_objects = std::vector<GLuint>();
	// Drawables holder
	std::vector<Drawable> drawables = std::vector<Drawable>();

};

