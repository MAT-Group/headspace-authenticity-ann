/*
 * Classification example.
 */

#include <cstdlib>
#include <cstdio>
#include <ctime>

#include <string>
#include <iostream>
#include <vector>

#include "yannl.hpp" // Neural network library
#include "print_matrix.hpp" // Neural network debug tool
#include "fileparser.hpp" // CSV interface
#include "TCLAP/CmdLine.h" // Command line interface

using namespace std;
using namespace yannl;

template<typename... Args> void verbosePrint(bool v, Args... args)
{
	if (v)
		printf(args...);
}

int main(int argc, char* argv[])
{

	// Setup random number generator
	srand((unsigned int)time(NULL));

	// Capture parsed arguments
	try
	{
		TCLAP::CmdLine cmd("WhiskeyNN.exe by James Swift - Training and prediction tool for Advion CMS Headspace vAPCI data of whiskey", ' ', "1.0A");
		TCLAP::SwitchArg cmd_verbose("v", "verbose", "Detailed output from tool", cmd, true);
		TCLAP::SwitchArg cmd_train("t", "train", "Whether to train a new neural network", cmd, true);

		// Training mode
		TCLAP::ValueArg<unsigned int> cmd_inHidden("k", "nHidden", "Number of hidden neurons", false, 100, "unsigned integer", cmd);
		TCLAP::ValueArg<double> cmd_inLearnRate("l", "nLearn", "Learning rate", false, 0.16, "double", cmd);
		TCLAP::ValueArg<double> cmd_inAnneal("a", "nAnneal", "Annealing rate", false, 1000, "double", cmd);
		TCLAP::ValueArg<double> cmd_inMomentum("m", "nMomentum", "Learning Momentum", false, 0.9, "double", cmd);

		TCLAP::ValueArg<unsigned int> cmd_inRepeats("r", "nRepeats", "Number of repeats", false, 1, "unsigned integer", cmd);

		// Input
		TCLAP::ValueArg<std::string> cmd_inData("d", "data", "Learning data", false, "input_data.csv", "string", cmd);
		TCLAP::ValueArg<std::string> cmd_inTarget("c", "target", "Learning target", false, "input_target.csv", "string", cmd);
		TCLAP::ValueArg<std::string> cmd_inNetwork("n", "network", "Network location", false, "network.data", "string", cmd);

		cmd.parse(argc, argv);

		bool training = cmd_train.getValue();
		bool verbose = cmd_verbose.getValue();

		if (training)
		{
			verbosePrint(verbose, "Entering training mode . . .\n");

			verbosePrint(verbose, "Loading training data and targets\n");
			yannl::Matrix data = WhiskeyNN::LoadMatrixFromFile(cmd_inData.getValue());
			yannl::Matrix target = WhiskeyNN::LoadMatrixFromFile(cmd_inTarget.getValue());

			// Train network. Request it to print out loss every 100 epochs.
			verbosePrint(verbose, "Network is training . . .\n");

			std::vector<std::vector<double>> LossTable;
			unsigned int nRepeats = cmd_inRepeats.getValue();
			for (unsigned int it = 0; it < nRepeats; it++)
			{

				// Create network
				yannl::Network network(yannl::CLASSIFICATION, data.cols(), { cmd_inHidden.getValue() }, { yannl::sigmoid }, target.cols(), yannl::softmax);

				// Create training parameters
				yannl::TrainingParams params;
				params.data = data;
				params.target = target;
				params.batch_size = data.rows();
				params.max_iterations = 5000;
				params.epoch_analysis_interval = 10;
				params.min_accuracy = 0.95;
				params.learning_rate = cmd_inLearnRate.getValue();
				params.annealing_factor = cmd_inAnneal.getValue();
				params.regularization_factor = 0.002;
				params.momentum_factor = cmd_inMomentum.getValue();
				params.shuffle = true;

			
				std::vector<double> Losssingle;
				network.train(params, true, std::cout, Losssingle);

				// Collect training analysis
				network.feed_forward(data);
				print_matrix_train(network.predict(true), target);
				printf("Accuracy: %.3f% \n", network.accuracy(data, target) * 100);

				verbosePrint(verbose, "Saving network . . .\n");
				network.save(cmd_inNetwork.getValue());

				LossTable.push_back(Losssingle);
			}

			// Output Loss Table
			for (auto &LossSingle : LossTable)
			{
				for (double Loss : LossSingle)
				{
					std::cout << std::fixed << Loss << '\t';
				}
				std::cout << '\n';
			}
			
		}
		else // Prediction mode
		{
			verbosePrint(verbose, "Entering prediction mode . . .\n");
			yannl::Network network = yannl::Network(cmd_inNetwork.getValue());

			yannl::Matrix data = WhiskeyNN::LoadMatrixFromFile(cmd_inData.getValue());
			network.feed_forward(data);
			print_matrix(network.predict(true));
		}

	}
	catch (TCLAP::ArgException& e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

	return 0;

}
