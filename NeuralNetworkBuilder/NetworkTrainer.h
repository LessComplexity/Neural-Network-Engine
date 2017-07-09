/****************************************************************************************************************/
/*                                                                                                              */
/*   NNE - Neural Networks Engine                                                                               */
/*   www.sapirshemer.com                                                                                        */
/*                                                                                                              */
/*   NETWORK TRAINING HEADER                                                                                    */
/*                                                                                                              */
/*   Sapir Shemer                                                                                               */
/*   DevShift - Shifting development forward                                                                    */
/*   me@sapirshemer.com                                                                                         */
/*                                                                                                              */
/****************************************************************************************************************/

#pragma once
#include "definitions.h"

class NetworkTrainer
{
public:
	NetworkTrainer();
	virtual ~NetworkTrainer();

	// Set the type of training you want to work with
	void setTrainingType(int type);
	// Set the type of error mesuring function
	void setErrorMesurementFunction(int function_type);
	// Set the training inputs and patterns for the trainer to work with
	void setTrainingSet(VectorSet training_input, VectorSet training_patterns);
	// Set the function to use for error mesurement
	void setErrorMesurementFunction(std::function<long double(Vector training_input, Vector network_output)> error_mesurement);

	// Calculate total error
	void calculateTotalError();
	// Returns the total error varialbe
	long double getTotalError();
private:
	// A variable that indicated the type of training we want to perfoqrm
	int training_type = 0;
	int err_function_type = COSTUM_ERROR_MES;

	/* Vectors used for training */
	// Used for comparing the output of the network and getting an error vector in return
	VectorSet training_inputs = VectorSet();
	// Inputs that we want the network to train on
	VectorSet training_patterns = VectorSet();

	/* Vectors used for error checking */
	// A vector list to check the difference of the output from the desired result
	VectorSet error_vectors = VectorSet();
	// A vector list of the outputs the network gave us when training
	VectorSet output_vectors = VectorSet();
	// A vector containing all the error mesurement values that have returned by the net
	Vector error_mes_vector = Vector();
	// Variable for storing the total error of a batch of training samples
	long double total_error = 0;

	// An object holder of the error measuring function
	std::function<long double(Vector training_input, Vector network_output)> error_mesurement;
};