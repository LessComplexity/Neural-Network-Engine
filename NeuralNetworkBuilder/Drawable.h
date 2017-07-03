#pragma once
#include "definitions.h"

class Drawable
{
public:
	// Create an empty drawable
	Drawable();
	// Create and initialize on the spot
	Drawable(Vertices vertices);
	virtual ~Drawable();

	// Initialize drawable
	void init(Vertices& vertices);
	// Draw drawable on screen
	void draw();

private:
	Vertices vertices;
	GLuint vbo;
	bool has_init = false;
};

