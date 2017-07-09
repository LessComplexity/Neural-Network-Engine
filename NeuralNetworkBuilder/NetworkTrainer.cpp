/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NETWORK TRAINING CLASS                                                                                     */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#include "NetworkTrainer.h"


NetworkTrainer::NetworkTrainer()
{
}


NetworkTrainer::~NetworkTrainer()
{
}

void NetworkTrainer::setTrainingType(int type)
{
	this->training_type = type;
}

void NetworkTrainer::setErrorMesurementFunction(int function_type)
{
	this->err_function_type = function_type;
	switch (this->err_function_type)
	{
	case SIMPLE_BACKPROPAGATION_ERROR_MES:
		// Simple backpropagation error mesurement function
		this->error_mesurement = [](Vector training_input, Vector network_output) -> long double {
			long double to_ret = 0;
			for (int elm = 0; elm < training_input.size(); elm++)
			{
				to_ret += pow(training_input[elm] - network_output[elm], 2);
			}

			return to_ret / 2;
		};
		break;

	case EUCLIDEAN_DISTANCE_ERROR_MES:
		// Euclidean distance error mesurement function
		this->error_mesurement = [](Vector training_input, Vector network_output) -> long double {
			long double to_ret = 0;
			for (int elm = 0; elm < training_input.size(); elm++)
			{
				to_ret += pow(training_input[elm] - network_output[elm], 2);
			}

			return sqrt(to_ret);
		};

		break;
	case ROOT_MEAN_SQUARE_ERROR_MES:
		// Root mean square error mesurement function
		this->error_mesurement = [](Vector training_input, Vector network_output) -> long double {
			long double to_ret = 0;
			for (int elm = 0; elm < training_input.size(); elm++)
			{
				to_ret += pow(training_input[elm] - network_output[elm], 2);
			}

			return sqrt(to_ret / training_input.size());
		};

		break;
	default:
		break;
	}
}

void NetworkTrainer::setTrainingSet(VectorSet training_inputs, VectorSet training_patterns)
{
	this->training_inputs = training_inputs;
	this->training_patterns = training_patterns;
}

void NetworkTrainer::setErrorMesurementFunction(std::function<long double(Vector training_input, Vector network_output)> error_mesurement)
{
	this->err_function_type = COSTUM_ERROR_MES;
	this->error_mesurement = error_mesurement;
}

void NetworkTrainer::calculateTotalError() {
	// Reset error variable
	this->total_error = 0;

	// Calculate total error
	for (long double element : this->error_mes_vector)
	{
		this->total_error += element;
	}
}

long double NetworkTrainer::getTotalError() {
	return this->total_error;
}