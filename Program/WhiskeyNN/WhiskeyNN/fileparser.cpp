#include "fileparser.hpp"

std::vector<std::string> WhiskeyNN::explodeString
(std::string const & input, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(input);

	for (std::string token; std::getline(iss, token, delim);)
	{
		result.push_back(std::move(token));
	}

	return result;
}

std::vector<std::vector<std::string>> WhiskeyNN::readCSV(std::string const & input, char delimLine, char delimValue)
{

	std::vector<std::vector<std::string>> result;

	// For every delimited line
	auto lines = WhiskeyNN::explodeString(input, delimLine);
	for (auto line : lines)
	{
		// Explode by value delimiter and store result
		result.push_back(WhiskeyNN::explodeString(line, delimValue));
	}

	return result;
}

bool WhiskeyNN::SkipBOM(std::istream& file)
{
	char test[4] = { 0 };
	file.read(test, 3);
	if ((unsigned char)test[0] == 0xEF &&
		(unsigned char)test[1] == 0xBB &&
		(unsigned char)test[2] == 0xBF)
	{
		return true;
	}
	file.seekg(0);
	return false;
}

std::string WhiskeyNN::ReadFile(std::string filename)
{
	std::ifstream file(filename);
	if (!file)
	{
		std::cout << "ERROR LOADING FILE " << filename << std::endl;
	}
	std::string output;

	// Skip BOM where required
	bool bom = WhiskeyNN::SkipBOM(file);

	// Reserve string size
	file.seekg(0, std::ios::end);
	output.reserve(file.tellg());
	file.seekg(bom ? 3 : 0, std::ios::beg);

	// Assign string
	output.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	file.close();
	return output;
}

void WhiskeyNN::LoadMatrixFromCSV(std::vector<std::vector<std::string>> parsedCSV, yannl::Matrix& output)
{
	unsigned int nRow = 0;
	unsigned int nCol = 0;

	for (auto row : parsedCSV)
	{

		for (auto cell : row)
		{
			// Convert cell value to double type and set
			try
			{
				output.set(nRow, nCol, std::stod(cell));
			}
			catch (...)
			{
				output.set(nRow, nCol, 0); // ERROR
			}


			// Increment column counter
			nCol++;
		}

		// Increment row counter and reset column counter
		nRow++;
		nCol = 0;
	}
}

void WhiskeyNN::LoadMatrixFromString(std::string csv, yannl::Matrix& output)
{
	auto parsedCSV = WhiskeyNN::readCSV(csv, '\n', ',');
	WhiskeyNN::LoadMatrixFromCSV(parsedCSV, output);
}

void WhiskeyNN::LoadMatrixFromFile(std::string filename, yannl::Matrix& output)
{
	// Read entire contents of file
	std::string fileContents = WhiskeyNN::ReadFile(filename);
	WhiskeyNN::LoadMatrixFromString(fileContents, output);
}

yannl::Matrix WhiskeyNN::LoadMatrixFromFile(std::string filename)
{
	std::string fileContents = WhiskeyNN::ReadFile(filename);
	auto parsedCSV = WhiskeyNN::readCSV(fileContents, '\n', ',');

	std::size_t nRow = parsedCSV.size();
	std::size_t nCol = parsedCSV[0].size();

	yannl::Matrix output(nRow, nCol, 0);
	WhiskeyNN::LoadMatrixFromCSV(parsedCSV, output);

	return output;
}

void WhiskeyNN::AssistantLoad(std::string fileStub, yannl::Matrix& data, yannl::Matrix& target)
{
	WhiskeyNN::LoadMatrixFromFile(fileStub + "_data.csv", data);
	WhiskeyNN::LoadMatrixFromFile(fileStub + "_target.csv", target);
}