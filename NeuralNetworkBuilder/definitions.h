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
#include <iostream>
#include <vector>
#include <functional>
#include <exception>
#include <map>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <CL\cl.hpp>

// ************************************************************ //
// ********************** Define Helpers ********************** //
// ************************************************************ //

#define DEBUG

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
#define		STANDART_NEURON_FREMI				0x00000B
#define		STANDART_NEURON_HYPERBOLIC			0x00000C