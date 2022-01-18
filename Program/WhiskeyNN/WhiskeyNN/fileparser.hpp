#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <utility>

#include "yannl.hpp"

namespace WhiskeyNN
{

	std::vector<std::string> explodeString(std::string const & input, char delim);
	std::vector<std::vector<std::string>> readCSV(std::string const & input, char delimLine, char delimValue);
	bool SkipBOM(std::istream& file);
	std::string ReadFile(std::string filename);

	void LoadMatrixFromCSV(std::vector<std::vector<std::string>> parsedCSV, yannl::Matrix& output);
	void LoadMatrixFromString(std::string csv, yannl::Matrix& output);
	void LoadMatrixFromFile(std::string filename, yannl::Matrix& output);
	yannl::Matrix LoadMatrixFromFile(std::string filename);

	void AssistantLoad(std::string fileStub, yannl::Matrix& data, yannl::Matrix& target);
}
