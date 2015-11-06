/*--------------------------------------
Project:	mp-follow-build
File: 		bash_command.h
Author:		Jay Hudson

(c) 2015 Jay Hudson
This application is licensed under the 
MIT License.  		
--------------------------------------*/

#ifndef BASH_COMMAND_H
#define BASH_COMMAND_H

#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>

typedef std::multimap<unsigned short, std::string> DependantMap;

std::string& getCommand(std::string& command, const std::string& macPort) {
	command = "port deps ";
	command += macPort;
	command += 
		" | grep -v \'no dependencies\'"
		" | grep -v \'^Full Name\'"
		" | sed \'s|^.*Dependencies: ||g\'"
		" | tr \',\' \'\\n\'"
		" | awk \'{$1=$1};1\'";

	return command;
}

const unsigned short SIZE=0xFF;

std::vector<std::string>& getFileResults(std::vector<std::string>& in, const std::string& macPort) {
	char grabber[SIZE+1];

	std::string command;

	FILE* results = popen(getCommand(command, macPort).c_str(), "r");

	while (fgets(grabber, SIZE, results) != NULL) {
		std::string tmp(grabber);
		tmp.pop_back();
		in.push_back(tmp);
	}
	
	pclose(results);

	return in;
}

bool checkForDupes (DependantMap& in, const std::string& macPort) {
	for ( auto& g : in) {
		if( g.second == macPort ) {
			return true;
		}
	}

	return false;
}
	
DependantMap& getDependencies(DependantMap& in, const std::string& macPort) {
	std::vector<std::string> fileResults;

	getFileResults(fileResults, macPort);
	in.insert(std::make_pair(fileResults.size(), macPort));

	// cerr is used so that stdout can be redirected to a file.
	std::cerr << "checking " << macPort << "...\n"; 

	for(auto& i : fileResults) {
		if(!checkForDupes(in, i)) {
			getDependencies(in, i);
		}
	}
	
	return in;
}



#endif