// BullFrog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "../include/BullFrog.h"

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cout << "USAGE: " << argv[0] << " <PATH_TO_DUMPED_CODE>" << std::endl;
		return -1;
	}

	std::string path = argv[1];
	if (!Tadpole::Disassemble(path)) {
		return -1;
	}
	
	return 0;
}
