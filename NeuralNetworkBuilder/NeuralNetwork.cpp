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
	this->stopSimulator();
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

void NeuralNetwork::setNeuronType(Neuron& my)
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
		this->feedforward_layers->at(0)[input].RunNeuron(inputs[input]);

	// Loop through layers and run the neurons
	for (int op_layer = 1; op_layer < this->feedforward_layers->size(); op_layer++)
	{
		// Apply each neuron's calculation
		#pragma omp parallel for num_threads(8)
		for (int op_neuron = 0; op_neuron < this->feedforward_layers->at(op_layer).size(); op_neuron++)
		{
			// Run each neuron on preceding layer 
			this->feedforward_layers->at(op_layer)[op_neuron].RunNeuron(this->feedforward_layers->at(op_layer - 1));
		}
	}

	// Get output from last layer
	for (Neuron& out : this->feedforward_layers->at(this->feedforward_layers->size() - 1))
		this->lastOutput.push_back(out.getOutput());

	#ifdef DEBUG
	std::cout << "[+] NNE: Run finished successfully." << std::endl;
	std::cout << "[+] Outputs: { ";
	for (double o : this->lastOutput)
		std::cout << o << " ";
	std::cout << "}" << std::endl;
	// Return time needed for operation to complete
	std::cout << "OP Time (ms): " << clock() - start << std::endl << std::endl;
	#endif // DEBUG
}

void NeuralNetwork::startSimulator(const char * window_title, int width, int height, bool full_screen)
{
	simulator_thread = std::thread([window_title, width, height, full_screen]() {
		NeuralSimulator * simulator = new NeuralSimulator();

		// Set OGL version
		simulator->setOGLVersion(4, 3);

		simulator->createWindow(window_title, width, height, full_screen);

		simulator->createDrawingContext();
		simulator->addDrawable({
			{ -0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.5f, -0.5f, 0.0f, 1.0f },
			{ 0.0f,  0.5f, 0.0f, 1.0f }
		});

		// Set program
		simulator->setProgram(simulator->addShadingProgram("VertexShader.glsl", "FragmentShader.glsl"));

		simulator->startRendering();
	});
}

void NeuralNetwork::stopSimulator()
{
	simulator_thread.detach();
}

void NeuralNetwork::resetNetwork()
{
	#ifdef DEBUG
		std::cout << "[+] NNE: Resetting network." << std::endl;
	#endif // DEBUG
	// In case of an interconnected network
	if (this->topology == COMPLETE_INTERCONNECTION)
	{
		// Reset each connection for every neuron
		for (Neuron neuron : this->neurons)
			neuron.resetConnections();
	}
	else
	{
		// Loop through each layer and reset connections as random
		for (int layerNum = 0; layerNum < this->feedforward_layers->size(); layerNum++)
		{
			for (int neuronNum = 0; neuronNum < this->feedforward_layers->at(layerNum).size(); neuronNum++)
			{
				this->feedforward_layers->at(layerNum)[neuronNum].resetConnections();
			}
		}
	}
	#ifdef DEBUG
		std::cout << "[+] NNE: Reset done." << std::endl;
		std::cout << std::endl;
	#endif // DEBUG
}

void NeuralNetwork::useServer(const char * server_ip, const char * port, const char * token)
{
}

void NeuralNetwork::runAsServer()
{
}

// Load to memory the feedforward network
void NeuralNetwork::buildFeedforwardNet()
{
	// Variable to determine the neuron's ID
	int neuron_id = 0;

	for (int layerNum = 0; layerNum < this->network_topology.size(); layerNum++)
	{
		// Create layer
		this->feedforward_layers->push_back(Layer());

		// Add neurons for layer
		for (int NeuronNum = 0; NeuronNum < this->network_topology[layerNum]; NeuronNum++)
		{
			// If Neuron is user defined then act accordingly
			if (layerNum == 0)
				this->feedforward_layers->back().push_back(Neuron(INPUT_NEURON));
			else
			{
				if (this->user_defined != NULL)
				{
					this->feedforward_layers->back().push_back(Neuron());
					this->feedforward_layers->back().back().setNeuronFunctions(this->user_defined->getPropagationFunction(),
						this->user_defined->getActivationFunction(),
						this->user_defined->getOutputFunction());
				}
				else
					this->feedforward_layers->back().push_back(Neuron(this->neuron_type));
			}

			// Add ID
			this->feedforward_layers->back().back().setNeuronId(neuron_id);

			// Next neuron Id
			neuron_id++;
		}

		// If users defined to use a bias neuron - add it
		if (this->use_biases && layerNum != this->network_topology.size() - 1)
		{
			this->feedforward_layers->back().push_back(Neuron(BIAS_NEURON));
			this->feedforward_layers->back().back().setNeuronId(neuron_id);
			
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
		this->neurons.push_back(Neuron(this->neuron_type));
		this->neurons.back().setNeuronId(neuronNum);
	}

	// We want to run the preceding code only one time
	omp_set_num_threads(1);
	// If users defined to use a bias neuron - add it
	#pragma omp parallel
	{
		if (this->use_biases)
		{
			this->neurons.push_back(Neuron(BIAS_NEURON));
			this->neurons.back().setNeuronId(this->num_of_neurons);

			// Increase number of neurons
			this->num_of_neurons++;
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::standartFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers->size(); layerNum++)
	{
		// Get each neuron and assign him connections by previous layer
		#pragma omp parallel for
		for (int neuronNum = 0; neuronNum < this->feedforward_layers->at(layerNum).size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers->at(layerNum)[neuronNum].getNeuronType() == BIAS_NEURON)
				continue;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers->at(layerNum - 1).size(); prevNeuron++)
			{
				// Create a connection, assign a random weight
				this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum - 1)[prevNeuron].getNeuronId(), NeuralNetwork::getRandomWeight());
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
	for (int layerNum = 1; layerNum < this->feedforward_layers->size(); layerNum++)
	{
		// Get each neuron and assign him connections by previous layer
		#pragma omp parallel for
		for (int neuronNum = 0; neuronNum < this->feedforward_layers->at(layerNum).size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers->at(layerNum)[neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers->at(layerNum - 1).size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum - 1)[prevNeuron].getNeuronId(), NeuralNetwork::getRandomWeight());
			}

			// Add connection to itself
			this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum)[neuronNum].getNeuronId(), NeuralNetwork::getRandomWeight());
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::indirectRecurrenceFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers->size(); layerNum++)
	{
		#pragma omp parallel for
		// Get each neuron and assign him connections by previous layer
		for (int neuronNum = 0; neuronNum < this->feedforward_layers->at(layerNum).size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers->at(layerNum)[neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers->at(layerNum - 1).size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to neuron's connections
				this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum - 1)[prevNeuron].getNeuronId(), NeuralNetwork::getRandomWeight());

				// Add a connection to the opposing side to create the indirect recurrence network
				this->feedforward_layers->at(layerNum - 1)[prevNeuron].addConnection(this->feedforward_layers->at(layerNum)[neuronNum].getNeuronId(), NeuralNetwork::getRandomWeight());
			}

			// Add connection to itself
			this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum)[neuronNum].getNeuronId(), NeuralNetwork::getRandomWeight());
		}
	}
}

// Load to memory all the connections and assign them a random weight
void NeuralNetwork::lateralFeedforwardConnections()
{
	// Loop through the layers with active neurons - skip input layer
	for (int layerNum = 1; layerNum < this->feedforward_layers->size(); layerNum++)
	{
		#pragma omp parallel for
		// Get each neuron and assign him connections by previous layer
		for (int neuronNum = 0; neuronNum < this->feedforward_layers->at(layerNum).size(); neuronNum++)
		{
			// If a bias neuron then break
			if (this->feedforward_layers->at(layerNum)[neuronNum].getNeuronType() == BIAS_NEURON)
				break;

			// Loop through the current layer to create make the lateral recurrence
			for (int inLayerNum = 0; inLayerNum < this->feedforward_layers->at(layerNum).size(); inLayerNum++)
			{
				// Jump to next step if the neuron is the same
				if (inLayerNum == neuronNum)
					continue;

				// Create a connection, assign a random weight & add to connections
				this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum)[inLayerNum].getNeuronId(), NeuralNetwork::getRandomWeight());
			}

			// Loop through previous layer
			for (int prevNeuron = 0; prevNeuron < this->feedforward_layers->at(layerNum - 1).size(); prevNeuron++)
			{
				// Create a connection, assign a random weight & add to connections
				this->feedforward_layers->at(layerNum)[neuronNum].addConnection(this->feedforward_layers->at(layerNum - 1)[prevNeuron].getNeuronId(), NeuralNetwork::getRandomWeight());
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
				this->neurons[connNum].addConnection(this->neurons[neuronNum].getNeuronId(), NeuralNetwork::getRandomWeight());
			}

			// A bias neuron can't get inputs
			if (this->neurons[neuronNum].getNeuronType() != BIAS_NEURON)
			{
				this->neurons[neuronNum].addConnection(this->neurons[connNum].getNeuronId(), NeuralNetwork::getRandomWeight());
			}
		}
	}
}