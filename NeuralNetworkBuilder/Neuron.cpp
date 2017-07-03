/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   Neuron CLASS                                                                                               */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "Neuron.h"

Neuron::Neuron()
{
}

Neuron::Neuron(unsigned type)
{
	this->neuron_type = type;
	switch (this->neuron_type)
	{
	case BIAS_NEURON:
		Neuron::asBias(this);

		break;
	case INPUT_NEURON:
		Neuron::asInput(this);

		break;
	case STANDART_NEURON:
		Neuron::asStandart(this);

		break;
	case STANDART_NEURON_HEAVISIDE:
		Neuron::asStandartHeaviside(this);

		break;
	case STANDART_NEURON_FERMI:
		Neuron::asStandartFermi(this);

		break;
	case STANDART_NEURON_HYPERBOLIC:
		Neuron::asStandartHyperbolic(this);

		break;
	default:
		break;
	}
}

Neuron::~Neuron()
{
}

// Run neuron to get output after calculation
double Neuron::RunNeuron(Layer& input_neurons)
{
	// Store network input, actvation value & output
	this->neuron_input = this->propagationFunction(input_neurons, this->connections);
	this->activated_input = this->activationFunction(this->neuron_input);
	this->output = this->outputFunction(activated_input);
	return this->output;
}

// Run a neuron as an input neuron
double Neuron::RunNeuron(double input)
{
	this->output = this->outputFunction(input);
	this->neuron_input = this->output;
	this->activated_input = this->output;
	return this->output;
}

// Define neuron as a bias neuron
void Neuron::asBias(Neuron* neuron)
{
	// A bias neuron doesn't allow any input
	neuron->setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		return 0;
	});

	// There's no need for an activation function
	neuron->setActivationFunction([](double input) -> double {
		return 0;
	});

	// Output will alway be 1
	neuron->setOutputFunction([](double input) -> double {
		return 1;
	});
}

// Define neuron as an input neuron
void Neuron::asInput(Neuron* neuron)
{
	// A propagation function is not needed
	neuron->setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		return 0;
	});

	// There's no need for an activation function
	neuron->setActivationFunction([](double input) -> double {
		return 0;
	});

	// Output will alway be the input
	neuron->setOutputFunction([](double input) -> double {
		return input;
	});
}

// Define neuron as standart
void Neuron::asStandart(Neuron* neuron)
{
	// Standart propagation function
	neuron->setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// We want to return the propagation function output
	neuron->setActivationFunction([](double input) -> double {
		return input;
	});

	// We want the output function to return the activation output
	neuron->setOutputFunction([](double input) -> double {
		return input;
	});
}

// Define a strandart neuron with the heaviside activation function
void Neuron::asStandartHeaviside(Neuron* neuron)
{
	// Standart propagation function
	neuron->setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Calculate the network input using all the values
		for (int i = 0; i < connections.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// If the input in the network is above 0, then return 1
	neuron->setActivationFunction([](double input) -> double {
		if (input > 0)
			return 1;

		return 0;
	});

	// We want the output function to return the activation output
	neuron->setOutputFunction([](double input) -> double {
		return input;
	});
}

// Define a strandart neuron with the hyperbolic activation function
void Neuron::asStandartHyperbolic(Neuron* neuron)
{
	// Standart propagation function
	neuron->setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;

		return network_input;
	});

	// Hyperbolic function for neuron activation
	neuron->setActivationFunction([](double input) -> double {
		return std::tanh(input);
	});

	// We want the output function to return the activation output
	neuron->setOutputFunction([](double input) -> double {
		return input;
	});
}

// Define a strandart neuron with the fermi function as an activation function
void Neuron::asStandartFermi(Neuron* neuron)
{
	// Standart set functions
	neuron->setNeuronFunctions(
	// Propagation function
	[](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	},
	// Activation function
	[](double input) -> double {
		return (1 / (1 + std::exp(input)));
	},
	// Output function
	[](double input) -> double {
		return input;
	}
	);
}

// Add a connection to the neuron
void Neuron::addConnection(int neuron_id, Connection toadd)
{
	this->connections.insert(std::pair<int, Connection>(neuron_id, toadd));
}

// Add a connection to the neuron
void Neuron::addConnection(int neuron_id, double weight)
{
	this->connections.insert(std::pair<int, Connection>(neuron_id, Connection()));
	this->connections[neuron_id].weight = weight;
}

// Returns the propagation function of the neuron
std::function<double(Layer&inputs_neurons, Connections&connections)> Neuron::getPropagationFunction()
{
	return this->propagationFunction;
}

// Returns the activation function of the neuron
std::function<double(double input)> Neuron::getActivationFunction()
{
	return this->activationFunction;
}

// Returns the output function of the neuron
std::function<double(double input)> Neuron::getOutputFunction()
{
	return this->outputFunction;
}

// Sets user defined propagation function
void Neuron::setPropagationFunction(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation)
{
	this->propagationFunction = propagation;
}

// Sets user defined activation function
void Neuron::setActivationFunction(std::function<double(double input)> activation)
{
	this->activationFunction = activation;
}

// Sets user defined output function
void Neuron::setOutputFunction(std::function<double(double input)> output)
{
	this->outputFunction = output;
}

// Sets all neuron's run functions
void Neuron::setNeuronFunctions(std::function<double(Layer&inputs_neurons, Connections&connections)> propagation,
	std::function<double(double input)> activation,
	std::function<double(double input)> output)
{
	this->propagationFunction = propagation;
	this->activationFunction = activation;
	this->outputFunction = output;
}

void Neuron::setNeuronId(int id)
{
	this->nID = id;
}

void Neuron::setConnections(Connections connections)
{
	this->connections = connections;
}

// Get the nework input
double Neuron::getNeuronInput()
{
	return this->neuron_input;
}

// Get the activation input
double Neuron::getActivationInput()
{
	return this->activated_input;
}

// Get the neuron's output
double Neuron::getOutput()
{
	return this->output;
}

// Get the neuron's ID in the net
int Neuron::getNeuronId()
{
	return this->nID;
}

int Neuron::getNeuronType()
{
	return this->neuron_type;
}
