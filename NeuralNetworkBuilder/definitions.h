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
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#endif

// ************************************************************ //
// ********************** Define Helpers ********************** //
// ************************************************************ //

#define		DEBUG
#define		RAND_MAXIMUM		0x00000FFF

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

// ************************************************************ //
// ********************** Define Simulator ******************** //
// ************************************************************ //

class Drawable;
class NeuralSimulator;

// ************************************************************ //
// ********************** Define Classes ********************** //
// ************************************************************ //

class Neuron;
class NeuralNetwork;
class NetworkTrainer;

// ************************************************************ //
// ********************** Define Networking ******************* //
// ************************************************************ //

class Server;
class Client;
class Machine;

#define		MACHINE_CODE				"IAmMachine"
#define		CLIENT_CODE					"IAmMachine"

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

// A set of double values
typedef std::vector<long double> Vector;

// A set of vectors
typedef std::vector<Vector> VectorSet;

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

#define		BIAS_NEURON							0x100001
#define		INPUT_NEURON						0x100002
#define		STANDART_NEURON						0x100003
#define		STANDART_NEURON_HEAVISIDE			0x100004
#define		STANDART_NEURON_FERMI				0x100005
#define		STANDART_NEURON_HYPERBOLIC			0x100006


// ************************************************************ //
// *********************** Define Training ******************** //
// ************************************************************ //

#define		COSTUM_ERROR_MES					0x200000
#define		SIMPLE_BACKPROPAGATION_ERROR_MES	0x200001
#define		EUCLIDEAN_DISTANCE_ERROR_MES		0x200002
#define		ROOT_MEAN_SQUARE_ERROR_MES			0x200003