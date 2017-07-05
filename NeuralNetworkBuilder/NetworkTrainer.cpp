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
	default:
		break;
	}
}

void NetworkTrainer::setTrainingSet(VectorSet training_inputs, VectorSet training_patterns)
{
	this->training_inputs = training_inputs;
	this->training_patterns = training_patterns;
}

void NetworkTrainer::setErrorMesurementFunction(std::function<Vector(Vector training_input, Vector network_output)> error_mesurement)
{
	this->err_function_type = COSTUM_ERROR_MES;
	this->error_mesurement = error_mesurement;
}
