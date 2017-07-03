#include <iostream>
#include "NeuralNetworkBuilder.h"

int main()
{
	// Create a network holder object with the network's architechture
	NeuralNetwork * nn = new NeuralNetwork(FEEDFORWARD_STANDART);

	// Start simulator
	nn->startSimulator("Simulator", 700, 500, false);

	// Set a neuron type to use for the network
	nn->setNeuronType(STANDART_NEURON_HYPERBOLIC);

	// Set to use bias neurons
	nn->setToUseBiasNeurons(false);

	// Add network topology
	nn->defineFeedForwardNetwork({ 50, 20, 50, 50, 10 });

	// Initialize network
	nn->initialize();

	// Generate random inputs
	std::vector<double> inputs;
	for (int i = 0; i < 50; i++)
		inputs.push_back(double(RandomNumber()) / double(RAND_MAX));

	// Insert input and run network
	nn->RunNetwork(inputs);

	pause();

 	return 0;
}