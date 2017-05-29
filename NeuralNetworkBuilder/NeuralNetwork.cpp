/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NEURAL NETWORKS CLASS                                                                                      */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork()
{
}

NeuralNetwork::NeuralNetwork(int network_topology)
{
	this->topology = network_topology;

	#ifdef DEBUG
	std::cout << "[+] NNE: Network has been defined" << std::endl << std::endl;
	#endif // DEBUG
}

NeuralNetwork::~NeuralNetwork()
{
}

// Define the topology layers & their neurons
void NeuralNetwork::defineFeedForwardNetwork(std::vector<int> layer_topology)
{
	this->network_topology = layer_topology;

	#ifdef DEBUG
	std::cout << "[+] NNE: Feedforward layer toology has been defined: ";
	std::cout << "[=] { ";
	for (int value: this->network_topology)
		std::cout << value << " ";
	std::cout << "}" << std::endl << std::endl;
	#endif // DEBUG
}

// Define an interconnected topology number of neurons
void NeuralNetwork::defineInterconnectedNetwork(int number_of_neurons)
{
	this->num_of_neurons = number_of_neurons;

	#ifdef DEBUG
	std::cout << "[+] NNE: Interconnected toology has been defined: " << this->num_of_neurons << std::endl << std::endl;
	#endif // DEBUG
}

// Set if to use biases to control threshold
void NeuralNetwork::setToUseBiasNeurons(bool useBias)
{
	this->use_biases = useBias;
}

void NeuralNetwork::setNeuronType(int type)
{
	this->neuron_type = type;
}

void NeuralNetwork::setNeuronType(Perceptron& my)
{
	this->user_defined = &my;
}

void NeuralNetwork::setNetworkTopology(int network_topology)
{
	this->topology = network_topology;

	#ifdef DEBUG
	std::cout << "[+] NNE: Network has been defined" << std::endl << std::endl;
	#endif // DEBUG
}

int NeuralNetwork::getNeuronType()
{
	return this->neuron_type;
}

int NeuralNetwork::getNetworkTopology()
{
	return this->topology;
}

bool NeuralNetwork::isBiasUsed()
{
	return this->use_biases;
}

// Initialize network and load data to memory
void NeuralNetwork::initialize()
{
	#ifdef DEBUG
	std::cout << "[=] NNE: Loading network to memory" << std::endl;
	// Get the start time
	int start = clock();
	#endif // DEBUG

	switch (this->topology)
	{
	case FEEDFORWARD_STANDART:
		this->buildFeedforwardNet();
		this->standartFeedforwardConnections();
		break;
	case FEEDFORWARD_SHORTCUT_CONNECTIONS:
		this->buildFeedforwardNet();
		this->shortcutFeedforwardConnections();
		break;
	case FEEDFORWARD_DIRECT_RECURRENCE:
		this->buildFeedforwardNet();
		this->directRecurrenceFeedforwardConnections();
		break;
	case FEEDFORWARD_INDIRECT_RECURRENCE:
		this->buildFeedforwardNet();
		this->indirectRecurrenceFeedforwardConnections();
		break;
	case FEEDFORWARD_LATERAL_RECURRENCE:
		this->buildFeedforwardNet();
		this->lateralFeedforwardConnections();
		break;
	case COMPLETE_INTERCONNECTION:
		this->buildInterconnectedNet();
		this->completeInterconnections();
		break;
	default:
		break;
	}

	#ifdef DEBUG
	std::cout << "[+] NNE: Connections have been loaded." << std::endl;
	// Return time needed for operation to complete
	std::cout << "OP Time (ms): " << clock() - start << std::endl << std::endl;
	#endif // DEBUG
}

void NeuralNetwork::RunNetwork(std::vector<double>& inputs)
{
	#ifdef DEBUG
	std::cout << "[=] NNE: Running network..." << std::endl;
	// Get the start time
	int start = clock();
	#endif // DEBUG

	// Clear last output
	if(!this->lastOutput.empty()) this->lastOutput.clear();

	// Assign inputs into first layer
	#pragma omp parallel for
	for (int input = 0; input < inputs.size(); input++)
		this->feedforward_layers[0][input].RunNeuron(inputs[input]);

	// Loop through first layer and put inputs
	for (int op_layer = 1; op_layer < this->feedforward_layers.size(); op_layer++)
	{
		// Apply each neuron's calculation
		#pragma omp parallel for
		for (int op_neuron = 0; op_neuron < this->feedforward_layers[op_layer].size(); op_neuron++)
		{
			// Run each neuron on preceding layer
			this->feedforward_layers[op_layer][op_neuron].RunNeuron(this->feedforward_layers[op_layer - 1]);
		}
	}

	// Get output from last layer
	for (Perceptron& out : this->feedforward_layers[this->feedforward_layers.size() - 1])
		this->lastOutput.push_back(out.getOutput());

	#ifdef DEBUG
	std::cout << "[+] NNE: Run finished successfully." << std::endl;
	std::cout << "[+] Inputs: { ";
	for (double o : inputs)
		std::cout << o << " ";
	std::cout << "}" << std::endl;
	std::cout << "[+] Outputs: { ";
	for (double o : this->lastOutput)
		std::cout << o << " ";
	std::cout << "}" << std::endl;
	// Return time needed for operation to complete
	std::cout << "OP Time (ms): " << clock() - start << std::endl << std::endl;
	#endif // DEBUG
}

// Load to memory the feedforward network
void NeuralNetwork::buildFeedforwardNet()
{
	// Variable to determine the neuron's ID
	int neuron_id = 0;

	for (int layerNum = 0; layerNum < this->network_topology.size(); layerNum++)
	{
		// Create layer
		this->feedforward_layers.push_back(Layer());

		// Add neurons for layer
		for (int perceptronNum = 0; perceptronNum < this->network_topology[layerNum]; perceptronNum++)
		{
			// Create a neuron
			Perceptron temp;

			// If perceptron is user defined then act accordingly
			if (layerNum == 0)
				temp = Perceptron(INPUT_NEURON);
			else
			{
				if (this->user_defined != NULL)
					temp.setNeuronFunctions(this->user_defined->getPropagationFunction(),
						this->user_defined->getActivationFunction(),
						this->user_defined->getOutputFunction());
				else
					temp = Perceptron(this->neuron_type);
			}

			// Add neuron with ID
			temp.setNeuronId(neuron_id);
			this->feedforward_layers.back().push_back(temp);

			// Next neuron Id
			neuron_id++;
		}

		// If users defined to use a bias neuron - add it
		if (this->use_biases && layerNum != this->network_topology.size() - 1)
		{
			Perceptron temp = Perceptron(BIAS_NEURON);
			temp.setNeuronId(neuron_id);
			this->feedforward_layers.back().push_back(temp);

			// Next neuron Id
			neuron_id++;
		}
	}

	num_of_neurons = neuron_id;
}

// Load to memory all needed neurons
void NeuralNetwork::buildInterconnectedNet()
{
	// Loop through neuron's number
	#pragma omp parallel for
	for (int neuronNum = 0; neuronNum < this->num_of_neurons; neuronNum++)
	{
		// Create a neuron and add to list
		Perceptron temp = Perceptron(this->neuron_type);
		temp.setNeuronId(neuronNum);
		this->neurons.push_back(temp);
	}

	// We want to run the preceding code only one time
	omp_set_num_threads(1);
	// If users defined to use a bias neuron - add it
	#pragma omp parallel
	{
		if (this->use_biases)
		{
			Perceptron temp = Perceptron(BIAS_NEURON);
			temp.setNeuronId(this->num_of_neurons);
			this->neurons.push_back(temp);

			// Increase number of neurons
			this->num_of_neurons++;
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::standartFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers.size(); layerNum++)
	{
		// Get each neuron and assign him connections by previous layer
		for (int neuronNum = 0; neuronNum < this->feedforward_layers[layerNum].size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers[layerNum][neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers[layerNum - 1].size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);
			}
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::shortcutFeedforwardConnections()
{
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::directRecurrenceFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers.size(); layerNum++)
	{
		// Get each neuron and assign him connections by previous layer
		#pragma omp parallel for
		for (int neuronNum = 0; neuronNum < this->feedforward_layers[layerNum].size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers[layerNum][neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers[layerNum - 1].size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);
			}

			// Add connection to itself
			Connection temp_connection;
			temp_connection.weight = NeuralNetwork::getRandomWeight();
			this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum][neuronNum].getNeuronId(), temp_connection);
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::indirectRecurrenceFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers.size(); layerNum++)
	{
	#pragma omp parallel for
		// Get each neuron and assign him connections by previous layer
		for (int neuronNum = 0; neuronNum < this->feedforward_layers[layerNum].size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers[layerNum][neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers[layerNum - 1].size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to neuron's connections
				Connection temp_connection, temp_connection_indirect;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);

				// Add a connection to the opposing side to create the indirect recurrence network
				temp_connection_indirect.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum - 1][prevNeuron].addConnection(this->feedforward_layers[layerNum][neuronNum].getNeuronId(), temp_connection_indirect);
			}

			// Add connection to itself
			Connection temp_connection;
			temp_connection.weight = NeuralNetwork::getRandomWeight();
			this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum][neuronNum].getNeuronId(), temp_connection);
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::lateralFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers.size(); layerNum++)
	{
		#pragma omp parallel for
		// Get each neuron and assign him connections by previous layer
		for (int neuronNum = 0; neuronNum < this->feedforward_layers[layerNum].size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers[layerNum][neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through the current layer to create make the lateral recurrence
			for (int inLayerNum = 0; inLayerNum < this->feedforward_layers[layerNum].size(); inLayerNum++)
			{
				// Jump to next step if the neuron is the same
				if (inLayerNum == neuronNum)
					continue;

				// Create a connection, assign a random weight & add to connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum][inLayerNum].getNeuronId(), temp_connection);
			}

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers[layerNum - 1].size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);
			}
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::completeInterconnections()
{
	// Loop through all neuron
	for (int neuronNum = 0; neuronNum < this->num_of_neurons; neuronNum++)
	{
		#pragma omp parallel for
		// For each neuron loop through all other neurons - to create complete interconnections
		for (int connNum = neuronNum + 1; connNum < this->num_of_neurons; connNum++)
		{
			// A bias neuron can't get inputs
			if (this->neurons[connNum].getNeuronType() != BIAS_NEURON)
			{
				// Create a connection, assign a random weight & add to connections
				Connection temp_connection_reverse;
				temp_connection_reverse.weight = NeuralNetwork::getRandomWeight();
				this->neurons[connNum].addConnection(this->neurons[neuronNum].getNeuronId(), temp_connection_reverse);
			}

			// A bias neuron can't get inputs
			if (this->neurons[neuronNum].getNeuronType() != BIAS_NEURON)
			{
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->neurons[neuronNum].addConnection(this->neurons[connNum].getNeuronId(), temp_connection);
			}
		}
	}
}
