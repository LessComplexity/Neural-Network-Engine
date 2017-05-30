#include <iostream>
#include "NeuralNetworkBuilder.h"

int main()
{
	// Only for testing right now...
	
	// Create a network holder object with the network's architechture
	NeuralNetwork * nn = new NeuralNetwork(FEEDFORWARD_STANDART);

	// Set a neuron type to use for the network
	nn->setNeuronType(STANDART_NEURON_FREMI);

	// Set to use bias neurons
	nn->setToUseBiasNeurons(true);

	// Add network topology
	nn->defineFeedForwardNetwork({ 50000, 5000, 500, 500, 500, 500, 500, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 
		50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,50, 50, 50, 50, 50, 50, 50, 50, 50, 50 });

	// Initialize network
	nn->initialize();

	// Generate random inputs
	std::vector<double> inputs;
	for (int i = 0; i < 50000; i++)
		inputs.push_back(NeuralNetwork::getRandomWeight());

	// Insert input and run network
	nn->RunNetwork(inputs);

	delete nn;
	return 0;
}
