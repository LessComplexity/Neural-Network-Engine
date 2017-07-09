/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Neuron HEADER                                                                                              */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "definitions.h"

class Neuron
{
public:
	Neuron();
	Neuron(unsigned type);
	~Neuron();

	// Run the neuron to get output
	double RunNeuron(Layer& input_neurons);
	// Run neuron to get output after calculation
	double RunNeuron(double input);

	/* Helper functions */
	// Define neuron as a bias neuron
	static void asBias(Neuron* neuron);
	// Define neuron as a input neuron
	static void asInput(Neuron* neuron);
	// Define neuron as a standard neuron
	static void asStandart(Neuron* neuron);
	// Define neuron as a heaviside neuron
	static void asStandartHeaviside(Neuron* neuron);
	// Define neuron as a hyperbolic neuron
	static void asStandartHyperbolic(Neuron* neuron);
	// Define neuron as a fermi neuron
	static void asStandartFermi(Neuron* neuron);
	// Add a connection to the neuron
	void addConnection(int neuron_id, Connection toadd);
	// Add a connection to the neuron
	void addConnection(int neuron_id, double weight);

	/* Getters */
	// Returns the propagation function of the neuron
	std::function<double(Layer& inputs_neurons, Connections& connections)> getPropagationFunction();
	// Returns the propagation activation of the neuron
	std::function<double(double input)> getActivationFunction();
	// Returns the propagation output of the neuron
	std::function<double(double input)> getOutputFunction();
	// Get the neuron input after propagation function
	double getNeuronInput();
	// Get the input acter the activation function
	double getActivationInput();
	// Get the neuron's output
	double getOutput();
	// Returns neuron ID
	int getNeuronId();
	// Returns neuron type
	int getNeuronType();

	/* Setters */
	// Sets user defined propagation function
	void setPropagationFunction(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation);
	// Sets user defined activation function
	void setActivationFunction(std::function<double(double input)> activation);
	// Sets user defined output function
	void setOutputFunction(std::function<double(double input)> output);
	// Set neuron functions
	void setNeuronFunctions(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation,
		std::function<double(double input)> activation,
		std::function<double(double input)> output);
	// The the ID of the neuron in the network
	void setNeuronId(int id);
	// Set the connections of the neuron to the previous neurons
	void setConnections(Connections connections);
	// Reset connections and their values
	void resetConnections();

private:
	// Propagation function
	std::function<double(Layer& inputs_neurons, Connections& connections)> propagationFunction;

	// Activation function
	std::function<double(double input)> activationFunction;

	// Output function
	std::function<double(double input)> outputFunction;

	// Variables used to store the output of certain functions in the neuron
	double neuron_input = 0, activated_input = 0, output = 0;

	// The neuron's identification number on the net
	int nID = 0;

	// Neuron connections
	Connections connections;

	// The type on neuron - standart by default
	int neuron_type = STANDART_NEURON;
};