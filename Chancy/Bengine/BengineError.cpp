#include "BengineError.h"

#include <cstdlib>
#include <iostream>

namespace Bengine {

	// Prints out error message and exits the game
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit...";
		int tmp;
		std::cin >> tmp;
		exit(10);
	}

}