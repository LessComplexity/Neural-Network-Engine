/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   PERCEPTRON CLASS                                                                                           */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "Perceptron.h"

Perceptron::Perceptron()
{
	Perceptron::asStandart(*this);
}

Perceptron::Perceptron(unsigned type)
{
	this->neuron_type = type;
	switch (this->neuron_type)
	{
	case BIAS_NEURON:
		Perceptron::asBias(*this);

		break;
	case INPUT_NEURON:
		Perceptron::asInput(*this);

		break;
	case STANDART_NEURON:
		Perceptron::asStandart(*this);

		break;
	case STANDART_NEURON_HEAVISIDE:
		Perceptron::asStandartHeaviside(*this);

		break;
	case STANDART_NEURON_FREMI:
		Perceptron::asStandartFermi(*this);

		break;
	case STANDART_NEURON_HYPERBOLIC:
		Perceptron::asStandartHyperbolic(*this);

		break;
	default:
		break;
	}
}

Perceptron::~Perceptron()
{
}

// Run neuron to get output after calculation
void Perceptron::RunNeuron(Layer& inputs_neurons)
{
	// Store network input, actvation value & output
	this->network_input = this->propagationFunction(inputs_neurons, this->connections);
	this->activated_input = this->activationFunction(this->network_input);
	this->output = this->outputFunction(activated_input);
}

// Run a neuron as an input neuron
void Perceptron::RunNeuron(double input)
{
	this->network_input = this->activated_input = this->output = this->outputFunction(activated_input);
}

// Define neuron as a bias neuron
void Perceptron::asBias(Perceptron& neuron)
{
	// A bias neuron doesn't allow any input
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		return 0;
	});

	// There's no need for an activation function
	neuron.setActivationFunction([](double& input) -> double {
		return 0;
	});

	// Output will alway be 1
	neuron.setOutputFunction([](double& input) -> double {
		return 1;
	});
}

// Define neuron as an input neuron
void Perceptron::asInput(Perceptron& neuron)
{
	// A propagation function is not needed
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		return 0;
	});

	// There's no need for an activation function
	neuron.setActivationFunction([](double& input) -> double {
		return 0;
	});

	// Output will alway be the input
	neuron.setOutputFunction([](double& input) -> double {
		return input;
	});
}

// Define neuron as standart
void Perceptron::asStandart(Perceptron& neuron)
{
	// Standart propagation function
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Check if the size of inputs and connections is corrent
		if (connections.size() != inputs_neurons.size())
		{
			throw std::logic_error("NNE Error: Input number doesn't match connections");
		}

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// We want to return the propagation function output
	neuron.setActivationFunction([](double& input) -> double {
		return input;
	});

	// We want the output function to return the activation output
	neuron.setOutputFunction([](double& input) -> double {
		return input;
	});
}

// Define a strandart neuron with the heaviside activation function
void Perceptron::asStandartHeaviside(Perceptron& neuron)
{
	// Standart propagation function
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Check if the size of inputs and connections is corrent
		if (connections.size() != inputs_neurons.size())
		{
			throw std::logic_error("NNE Error: Input number doesn't match connections");
		}

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// If the input in the network is above 0, then return 1
	neuron.setActivationFunction([](double& input) -> double {
		if (input > 0)
			return 1;

		return 0;
	});

	// We want the output function to return the activation output
	neuron.setOutputFunction([](double& input) -> double {
		return input;
	});
}

// Define a strandart neuron with the hyperbolic activation function
void Perceptron::asStandartHyperbolic(Perceptron& neuron)
{
	// Standart propagation function
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Check if the size of inputs and connections is corrent
		if (connections.size() != inputs_neurons.size())
		{
			throw std::logic_error("NNE Error: Input number doesn't match connections");
		}

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// Hyperbolic function for neuron activation
	neuron.setActivationFunction([](double& input) -> double {
		return std::tanh(input);
	});

	// We want the output function to return the activation output
	neuron.setOutputFunction([](double& input) -> double {
		return input;
	});
}

// Define a strandart neuron with the fermi function as an activation function
void Perceptron::asStandartFermi(Perceptron& neuron)
{
	// Standart propagation function
	neuron.setPropagationFunction([](Layer& inputs_neurons, Connections& connections) -> double {
		double network_input = 0;

		// Check if the size of inputs and connections is corrent
		if (connections.size() != inputs_neurons.size())
		{
			throw std::logic_error("NNE Error: Input number doesn't match connections");
		}

		// Calculate the network input using all the values
		for (int i = 0; i < inputs_neurons.size(); i++)
		{
			network_input += inputs_neurons[i].getOutput() * connections[inputs_neurons[i].getNeuronId()].weight;
		}

		return network_input;
	});

	// Fermi's function for neuron activation
	neuron.setActivationFunction([](double& input) -> double {
		return (1 / exp(1 + std::exp(input)));
	});

	// We want the output function to return the activation output
	neuron.setOutputFunction([](double& input) -> double {
		return input;
	});
}

void Perceptron::addConnection(int neuron_id, Connection toadd)
{
	this->connections.insert_or_assign(neuron_id, toadd);
}

// Sets user defined propagation function
void Perceptron::setPropagationFunction(std::function<double(Layer& inputs_neurons, Connections& connections)> propagation)
{
	this->propagationFunction = propagation;
}

// Sets user defined activation function
void Perceptron::setActivationFunction(std::function<double(double& input)> activation)
{
	this->activationFunction = activation;
}

// Sets user defined output function
void Perceptron::setOutputFunction(std::function<double(double& input)> output)
{
	this->activationFunction = output;
}

void Perceptron::setNeuronId(int id)
{
	this->nID = id;
}

void Perceptron::setConnections(Connections connections)
{
	this->connections = connections;
}


// Get the nework input
double Perceptron::getNetworkInput()
{
	return this->network_input;
}

// Get the activation input
double Perceptron::getActivationInput()
{
	return this->activated_input;
}

// Get the neuron's output
double Perceptron::getOutput()
{
	return this->output;
}

// Get the neuron's ID in the net
int Perceptron::getNeuronId()
{
	return this->nID;
}

int Perceptron::getNeuronType()
{
	return this->neuron_type;
}
