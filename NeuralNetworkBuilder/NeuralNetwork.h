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
	virtual ~NeuralNetwork();

	// Class functions
	void defineFeedForwardNetwork(std::vector<int> layer_topology);
	void defineInterconnectedNetwork(int number_of_neurons);
	void initialize();
	void RunNetwork(std::vector<double>& inputs);

	// Function to get random weight
	static double getRandomWeight() { return rand() / double(RAND_MAX); }

	// Setters
	void setToUseBiasNeurons(bool useBias);	
	void setNeuronType(int type);
	void setNetworkTopology(int network_topology);
	void setNeuronType(Perceptron& my);

	// Getters
	int getNeuronType();
	int getNetworkTopology();
	bool isBiasUsed();
private:
	// A viable that stores the topology type of the network
	unsigned short topology;
	std::vector<int> network_topology;

	// A layer which is defined for a feedforward network topology
	std::vector<Layer> * feedforward_layers = new std::vector<Layer>();

	// Vector that stores the last output
	std::vector<double> lastOutput;

	// User defined neuron type
	int neuron_type = FEEDFORWARD_STANDART;

	// Container of all neurons for a complete interconnection case
	Layer neurons;

	// Indicates to use a bias neuron
	bool use_biases;

	// Set user defined neuron
	Perceptron * user_defined = NULL;

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
