/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   GENERAL DEFINITIONS HEADER                                                                                 */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
// General nesseccery includes
#include <iostream>
#include <vector>
#include <functional>
#include <exception>
#include <map>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <thread>
#include <fstream>
#include <random>

// Include OpenCL & OpenGL
#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL\cl.h>
#include <CL\cl.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#endif

// ************************************************************ //
// ********************** Define Helpers ********************** //
// ************************************************************ //

#define		DEBUG
#define		RAND_MAXIMUM		0x0000FFFF

static void pause()
{
#ifdef MAC
	system("read");
#else
	system("pause");
#endif
}

// Seed with a real random value, if available
static std::random_device r;
//Function to get random number
static std::default_random_engine engine(r());
static std::uniform_int_distribution<int> uniform_dist(-RAND_MAXIMUM, RAND_MAXIMUM);

static int RandomNumber()
{
	return uniform_dist(engine);
}

// ************************************************************** //
// ********************** Define Simulator ********************** //
// ************************************************************** //

class Drawable;
class NeuralSimulator;

// ************************************************************ //
// ********************** Define Classes ********************** //
// ************************************************************ //

class Neuron;
class NeuralNetwork;

// ************************************************************ //
// ***************** Define Connections/Layers **************** //
// ************************************************************ //

// Define a connection between two neurons
struct Connection {
	double weight;
	double deltaWeight;
};

// Connections[i][j] = The connection between neuron i & j
typedef std::map<int, Connection> Connections;

// Layer defines a line of neuron/Neurons - used for a feedforward network topology
typedef std::vector<Neuron> Layer;

// A point of 4 vertices (x, y, z, w)
struct Point {
	GLfloat x, y, z, w;
	GLfloat operator[] (int n) {
		switch (n)
		{
		case 0:
			return this->x;
		case 1:
			return this->y;
		case 2:
			return this->z;
		case 3:
			return this->w;
		default:
			return NULL;
		}
	}
};

typedef std::vector<Point> Vertices;

// ************************************************************ //
// ********************* Define Topologies ******************** //
// ************************************************************ //

#define		FEEDFORWARD_STANDART				0x000001
#define		FEEDFORWARD_SHORTCUT_CONNECTIONS	0x000002
#define		FEEDFORWARD_DIRECT_RECURRENCE		0x000003
#define		FEEDFORWARD_INDIRECT_RECURRENCE		0x000004
#define		FEEDFORWARD_LATERAL_RECURRENCE		0x000005
#define		COMPLETE_INTERCONNECTION			0x000006


// ************************************************************ //
// *********************** Define Neurons ********************* //
// ************************************************************ //

#define		BIAS_NEURON							0x000007
#define		INPUT_NEURON						0x000008
#define		STANDART_NEURON						0x000009
#define		STANDART_NEURON_HEAVISIDE			0x00000A
#define		STANDART_NEURON_FERMI				0x00000B
#define		STANDART_NEURON_HYPERBOLIC			0x00000C