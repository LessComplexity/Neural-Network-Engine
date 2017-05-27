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
	std::cout << "[+] NNE: Feedforward layer toology has been defined: " << std::endl;
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

// Build the neurons and network topology - load all elements needed for network to run
void NeuralNetwork::buildNeurons()
{
	#ifdef DEBUG
	std::cout << "[=] NNE: Loading neurons to memory..." << std::endl;
	#endif // DEBUG

	if(this->topology == FEEDFORWARD_STANDART ||
		this->topology == FEEDFORWARD_SHORTCUT_CONNECTIONS ||
		this->topology == FEEDFORWARD_DIRECT_RECURRENCE ||
		this->topology == FEEDFORWARD_INDIRECT_RECURRENCE ||
		this->topology == FEEDFORWARD_LATERAL_RECURRENCE)
		this->buildFeedforwardNet();

	if(this->topology == COMPLETE_INTERCONNECTION)
		this->buildInterconnectedNet();

	#ifdef DEBUG
	std::cout << "[+] NNE: Loaded to memory." << std::endl << std::endl;
	#endif // DEBUG
}

// Create connections and load them to memory
void NeuralNetwork::buildConnections()
{
	#ifdef DEBUG
	std::cout << "[=] NNE: Loading connections to memory" << std::endl;
	#endif // DEBUG

	switch (this->topology)
	{
	case FEEDFORWARD_STANDART:
		this->standartFeedforwardConnections();
		break;
	case FEEDFORWARD_SHORTCUT_CONNECTIONS:
		this->shortcutFeedforwardConnections();
		break;
	case FEEDFORWARD_DIRECT_RECURRENCE:
		this->directRecurrenceFeedforwardConnections();
		break;
	case FEEDFORWARD_INDIRECT_RECURRENCE:
		this->indirectRecurrenceFeedforwardConnections();
		break;
	case FEEDFORWARD_LATERAL_RECURRENCE:
		this->lateralFeedforwardConnections();
		break;
	case COMPLETE_INTERCONNECTION:
		this->completeInterconnections();
		break;
	default:
		break;
	}

	#ifdef DEBUG
	std::cout << "[+] NNE: Connections have been loaded." << std::endl << std::endl;
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
			// Create a neuron, add ID and push it to neurons array
			Perceptron temp = layerNum == 0 ? Perceptron(INPUT_NEURON) : Perceptron();
			temp.setNeuronId(neuron_id);
			this->feedforward_layers.back().push_back(temp);

			// Used for debugging
			#ifdef DEBUG
			if (layerNum == 0) std::cout << "[+] NNE: Input neuron Has been created - " << neuron_id << std::endl;
			else std::cout << "[+] NNE: Empty neuron Has been created - " << neuron_id << std::endl;
			#endif // DEBUG

			// Next neuron Id
			neuron_id++;
		}

		// If users defined to use a bias neuron - add it
		if (this->use_biases && layerNum != this->network_topology.size() - 1)
		{
			Perceptron temp = Perceptron(BIAS_NEURON);
			temp.setNeuronId(neuron_id);
			this->feedforward_layers.back().push_back(temp);

			// Used for debugging
			#ifdef DEBUG
			std::cout << "[+] NNE: Bias neuron Has been created - " << neuron_id << std::endl;
			#endif // DEBUG

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
	for (int neuronNum = 0; neuronNum < this->num_of_neurons; neuronNum++)
	{
		// Create a neuron and add to list
		Perceptron temp = Perceptron();
		temp.setNeuronId(neuronNum);
		this->neurons.push_back(temp);

		#ifdef DEBUG
		std::cout << "[+] NNE: Empty neuron Has been created - " << neuronNum << std::endl;
		#endif // DEBUG
	}

	// If users defined to use a bias neuron - add it
	if (this->use_biases)
	{
		Perceptron temp = Perceptron(BIAS_NEURON);
		temp.setNeuronId(this->num_of_neurons);
		this->neurons.push_back(temp);

		// Used for debugging
		#ifdef DEBUG
		std::cout << "[+] NNE: Bias neuron Has been created - " << this->num_of_neurons << std::endl;
		#endif // DEBUG

		// Increase number of neurons
		this->num_of_neurons++;
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

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG
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

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG
			}

			// Add connection to itself
			Connection temp_connection;
			temp_connection.weight = NeuralNetwork::getRandomWeight();
			this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum][neuronNum].getNeuronId(), temp_connection);

			#ifdef DEBUG
			std::cout << "[=] Connection: "
				<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
				<< " |=> "
				<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
				<< " - w: "
				<< temp_connection.weight
				<< std::endl;
			#endif // DEBUG
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::indirectRecurrenceFeedforwardConnections()
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
				// Create a connection, assign a random weight & add to neuron's connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG

				// Add a connection to the opposing side to create the indirect recurrence network
				Connection temp_connection_indirect;
				temp_connection_indirect.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum - 1][prevNeuron].addConnection(this->feedforward_layers[layerNum][neuronNum].getNeuronId(), temp_connection_indirect);

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId()
					<< " - w: "
					<< temp_connection_indirect.weight
					<< std::endl;
				#endif // DEBUG
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

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum][inLayerNum].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG
			}

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers[layerNum - 1].size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->feedforward_layers[layerNum][neuronNum].addConnection(this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId(), temp_connection);

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->feedforward_layers[layerNum - 1][prevNeuron].getNeuronId()
					<< " |=> "
					<< this->feedforward_layers[layerNum][neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG
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

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->neurons[neuronNum].getNeuronId()
					<< " |=> "
					<< this->neurons[connNum].getNeuronId()
					<< " - w: "
					<< temp_connection_reverse.weight
					<< std::endl;
				#endif // DEBUG
			}

			// A bias neuron can't get inputs
			if (this->neurons[neuronNum].getNeuronType() != BIAS_NEURON)
			{
				Connection temp_connection;
				temp_connection.weight = NeuralNetwork::getRandomWeight();
				this->neurons[neuronNum].addConnection(this->neurons[connNum].getNeuronId(), temp_connection);

				#ifdef DEBUG
				std::cout << "[=] Connection: "
					<< this->neurons[connNum].getNeuronId()
					<< " |=> "
					<< this->neurons[neuronNum].getNeuronId()
					<< " - w: "
					<< temp_connection.weight
					<< std::endl;
				#endif // DEBUG
			}
		}
	}
}
