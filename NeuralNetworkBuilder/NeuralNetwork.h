/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NEURAL NETWORKS HEADER                                                                                     */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "Perceptron.h"

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int network_topology);
	~NeuralNetwork();

	// Class functions
	void defineFeedForwardNetwork(std::vector<int> layer_topology);
	void defineInterconnectedNetwork(int number_of_neurons);
	void setToUseBiasNeurons(bool useBias);
	void buildNeurons();
	void buildConnections();

	// Function to get random weight
	static double getRandomWeight() { return rand() / double(RAND_MAX); }
private:
	// A viable that stores the topology type of the network
	unsigned short topology;
	std::vector<int> network_topology;

	// A layer which is defined for a feedforward network topology
	std::vector<Layer> feedforward_layers;

	// Container of all neurons for a complete interconnection case
	Layer neurons;

	// Indicates to use a bias neuron
	bool use_biases;

	// Stores all the neurons in the net
	int num_of_neurons = 0;

	// Private builder functions
	void buildFeedforwardNet();
	void buildInterconnectedNet();
	void standartFeedforwardConnections();
	void shortcutFeedforwardConnections();
	void directRecurrenceFeedforwardConnections();
	void indirectRecurrenceFeedforwardConnections();
	void lateralFeedforwardConnections();
	void completeInterconnections();
};
