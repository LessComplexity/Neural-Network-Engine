#include <iostream>
#include "NeuralNetworkBuilder.h"

int main()
{
	// Only for testing right now...
	NeuralNetwork nn(FEEDFORWARD_DIRECT_RECURRENCE);
	nn.setToUseBiasNeurons(true);
	nn.defineFeedForwardNetwork({3, 3, 3, 2});

	nn.buildNeurons();
	nn.buildConnections();

	return 0;
}