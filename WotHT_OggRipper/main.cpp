
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <regex>

int main(int argc, char* argv[])
{
	if (argc < 2) { std::cout << "Needs a file as an argument.\n"; return 0; }

	std::ifstream input(argv[1], std::ios::binary);

	//std::vector<char> buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
	std::string buffer((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>())); //so we can use regex more easily

	std::vector<std::string> filenames;

	std::cout << "Read " << argv[1] << "\n";
	std::cout << buffer.size() << " bytes read.\n";
	
	//find all occurences of the regex (in order, hopefully) and store them into the vector of filenames
	std::regex r(R"(WAV.....\.ogg)");
	std::sregex_iterator rit(buffer.begin(), buffer.end(), r);
	std::sregex_iterator rend;
	while (rit != rend) 
	{
		std::cout << rit->str() << std::endl;
		filenames.push_back(rit->str());
		++rit;
	}

	//Go through each filename and copy the data from the Ogg header until the next file name.
	//Some extraneous information is included at the ends of each file, but as far as I can tell, that does not impact audio quality.
	size_t pos = 0;
	for (unsigned i = 0; i < filenames.size(); i++)
	{
		std::string outputName = "WHT MUSIC " + filenames[i].substr(3);
		std::cout << "Writing " << filenames[i] << " as " << outputName << '\n';
		std::ofstream output(outputName, std::ios::binary);

		pos = buffer.find(filenames[i], pos); //Find the file name
		pos = buffer.find(std::string({ 0x4F, 0x67, 0x67, 0x53, 0x00, 0x02 }), pos + 1); //Find the ogg header

		//If this is the last file
		if (i + 1 >= filenames.size())
		{
			std::string sub = buffer.substr(pos); //the end of this Ogg is until the end of the buffer.
			std::copy(sub.begin(), sub.end(), std::ostreambuf_iterator<char>(output)); //write to output
			std::cout << std::hex << "Wrote from 0x" << pos << " to 0x" << buffer.size() << std::dec << "\n";
		}
		else
		{
			size_t endpos = buffer.find(filenames[i+1], pos+1); //the end of this Ogg is until the beginning of the next filename.
			std::string sub = buffer.substr(pos, endpos - pos);
			std::copy(sub.begin(), sub.end(), std::ostreambuf_iterator<char>(output)); //write to output
			std::cout << std::hex << "Wrote from 0x" << pos << " to 0x" << endpos << std::dec << "\n";
		}
		
		output.close();
	}

	return 0;
}
