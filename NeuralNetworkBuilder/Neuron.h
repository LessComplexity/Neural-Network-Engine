/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Neuron HEADER                                                                                          */
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
	void RunNeuron(Layer& inputs_neurons);
	void RunNeuron(double input);

	// Helper functions
	static void asBias(Neuron* neuron);
	static void asInput(Neuron* neuron);
	static void asStandart(Neuron* neuron);
	static void asStandartHeaviside(Neuron* neuron);
	static void asStandartHyperbolic(Neuron* neuron);
	static void asStandartFermi(Neuron* neuron);
	void addConnection(int neuron_id, Connection toadd);

	// Getters
	std::function<double(Layer& inputs_neurons, Connections& connections)> getPropagationFunction();
	std::function<double(double& input)> getActivationFunction();
	std::function<double(double& input)> getOutputFunction();
	double getNetworkInput();
	double getActivationInput();
	double getOutput();
	int getNeuronId();
	int getNeuronType();

	// Setters
	void setPropagationFunction(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation);
	void setActivationFunction(std::function<double(double& input)> activation);
	void setOutputFunction(std::function<double(double& input)> output);
	void setNeuronFunctions(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation,
		std::function<double(double& input)> activation,
		std::function<double(double& input)> output);
	void setNeuronId(int id);
	void setConnections(Connections connections);

private:
	// Propagation function
	std::function<double(Layer& inputs_neurons, Connections& connections)> propagationFunction;

	// Activation function
	std::function<double(double& input)> activationFunction;

	// Output function
	std::function<double(double& input)> outputFunction;

	// Variables used to store the output of certain functions in the neuron
	double network_input = 0, activated_input = 0, output = 0;

	// The neuron's identification number on the net
	int nID = 0;

	// Neuron connections
	Connections connections;

	// The type on neuron - standart by default
	int neuron_type = STANDART_NEURON;
};