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
#include "Neuron.h"
#include "NeuralSimulator.h"
#include "NeuralClient.h"

class NeuralNetwork
{
public:
	NeuralNetwork();
	NeuralNetwork(int network_topology);
	// Set as virtual to despose of data quickly
	virtual ~NeuralNetwork();

	/* Class functions */
	// Define the network topology in case of feedforward
	void defineFeedForwardNetwork(std::vector<int> layer_topology);
	// Define number of neurons in case of an interconnected network
	void defineInterconnectedNetwork(int number_of_neurons);
	// Initialize and build the network after eveything is set up
	void initialize();
	// Run the network with certain outputs
	void RunNetwork(std::vector<double>& inputs);
	// Start simulating
	void startSimulator(const char* window_title, int width, int height, bool full_screen);
	// Stop simulating
	void stopSimulator();
	// Reset the network values randomly
	void resetNetwork();
	// Use a server to increase proccessing power
	void useServer(const char * server_ip, const char * port, const char * token);
	// Start network as server
	void runAsServer();

	// Function to get random weight
	static double getRandomWeight() { return RandomNumber() / double(RAND_MAXIMUM); }

	/* Setters */
	// Notify network to create a bias neuron to change the threshold of each neuron
	void setToUseBiasNeurons(bool useBias);	
	// Set neuron type the user wants the network to work with
	void setNeuronType(int type);
	// Set the topology in which a feedforward network is to be created
	void setNetworkTopology(int network_topology);
	// Set neuron type by a user defined neuron
	void setNeuronType(Neuron& my);

	/* Getters */
	// Get the neuron type the network uses
	int getNeuronType();
	// Get the network topology that has been defined earlier
	int getNetworkTopology();
	// Returns true is a bias neuron is used
	bool isBiasUsed();
private:
	// Simulator thread
	std::thread simulator_thread;
	// Connection thread
	std::thread connection_thread;

	// A viable that stores the topology type of the network
	unsigned short topology;
	// Stores the network topology for a feedforward network
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
	Neuron * user_defined = NULL;

	// Stores all the neurons in the net
	int num_of_neurons = 0;

	/* Private builder functions */
	// Build network as feedforward
	void buildFeedforwardNet();
	// Build network as interconnected
	void buildInterconnectedNet();
	// Create standard feedforward network connections
	void standartFeedforwardConnections();
	// Create shotcut feedforward network connections
	void shortcutFeedforwardConnections();
	// Create direct recurrence feedforward network connections
	void directRecurrenceFeedforwardConnections();
	// Create indirect recurrence feedforward network connections
	void indirectRecurrenceFeedforwardConnections();
	// Create lateral feedforward network connections
	void lateralFeedforwardConnections();
	// Create complete interconnections
	void completeInterconnections();

	/* Simulatoe functions */
	void simulate(const char* window_title, int width, int height, bool full_screen);
};
