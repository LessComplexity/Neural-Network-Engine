#include "Drawable.h"


Drawable::Drawable()
{

}

Drawable::Drawable(Vertices vertices)
{
	init(vertices);
}


Drawable::~Drawable()
{
}

void Drawable::init(Vertices& vertices)
{
	if (!this->has_init)
	{
		this->vertices = vertices;
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &this->vbo);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Point), &(this->vertices[0]), GL_STATIC_DRAW);

		// Set initialized as true
		this->has_init = true;
	}
}

void Drawable::draw()
{
	if (this->has_init)
	{
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,			// normalized?
			0,                  // stride
			0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, this->vertices.size()); // Starting from vertex 0;
		glDisableVertexAttribArray(0);
	}
}
