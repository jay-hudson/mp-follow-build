/*--------------------------------------
Project:	mp-follow-build
File: 		main.cpp
Author:		Jay Hudson

(c) 2015 Jay Hudson
This application is licensed under the 
MIT License.  		
--------------------------------------*/

#include <iostream>
#include <iomanip>
#include "bash_command.h"


int main(int argc, char** argv) {
	DependantMap dependencies;

	if(argc > 1) {
		char** beg = &argv[1];
		char** end = &argv[argc];
		while(beg != end) {
			const std::string argument = *beg;
			getDependencies(dependencies, *beg);
			++beg;
		}
	}
	else {
		std::cerr << "You need to enter a port! Usage mp-follow-build [name of Macport]\n";
		return 1;
	}

	// cerr is used so that stdout can be redirected to a file.
	std::cerr << '\n';

	std::cout << std::setw(15) << std::left << "No. deps" << "Macport" << '\n';
	for(auto& i : dependencies) {
		std::cout << std::setw(15) << std::left << i.first << i.second << '\n';
	}	

	return 0;
}


