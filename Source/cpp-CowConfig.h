////////////////////////////////////////////////////////////
//
// cpp-CowConfig
// Copyright (C) 2018 CowCorp
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the author be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef cppCowConfig
#define cppCowConfig

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class CowConfig {
private:
	std::ofstream writeConfig;
	std::fstream readConfig;
	bool firstLine = true, firstRead = true;
	std::string fileName;
	std::vector< std::string > lines;
	///////////////////
	void RemoveSubStr(std::string substr, std::string& str) {
		size_t pos = std::string::npos;

		while ((pos = str.find(substr)) != std::string::npos)
			str.erase(pos, substr.length());
	}
	///////////////////
	void ReadAllLines() {
		std::string str;
		while (std::getline(readConfig, str))
			lines.push_back(str);
	}
	///////////////////
	int FindElement(std::string section, std::string offsetText) {
		if (firstRead)
			ReadAllLines();

		section = "[" + section + "]";
		bool sectionFound = false;
		for (int i = 0; i < lines.size(); i++) {
			if (lines[i] == section || section == "[]")
				sectionFound = true;
			else if (sectionFound && lines[i].find("[") == 0 && lines[i].find("]") > 0)
				break;
			if (sectionFound && lines[i].find(offsetText) == 0)
				return i;
		}
		return -1;
	}
public:
	CowConfig() {}
	CowConfig(std::string fileName) {
		OpenFile(fileName);
	}
	~CowConfig() {
		if (readConfig.is_open())
			readConfig.close();
		if (writeConfig.is_open())
			writeConfig.close();
	}
	///////////////////
	bool OpenFile(std::string fileName) {
		fileName = fileName;

		readConfig.open(fileName);
		return (readConfig.is_open());
	} // Will open file with specified filename and will return whether the file is open
	//////////////////
	std::vector< std::string > GetLines() {
		std::string str;
		std::vector< std::string > ret;
		while (std::getline(readConfig, str))
			ret.push_back(str);
		return ret;
	}
	//////////////////
	void WriteLine(std::string offsetText, std::string valToWrite) {
		if (readConfig.is_open())
			readConfig.close(); //Close fstream file before writing to the file

		if (firstLine) {
			writeConfig.open(fileName);
			firstLine = false;
		}

		writeConfig << offsetText << valToWrite << std::endl;

		readConfig.open(fileName);
	}
	void WriteLine(std::string offsetText, const char* valToWrite) {
		if (readConfig.is_open())
			readConfig.close(); // Close fstream file before writing to the file

		if (firstLine) {
			writeConfig.open(fileName);
			firstLine = false;
		}

		writeConfig << offsetText << valToWrite << std::endl;

		readConfig.open(fileName);
	}
	template <class c>
	void WriteLine(std::string offsetText, c valToWrite) {
		if (readConfig.is_open())
			readConfig.close(); // Close fstream file before writing to the file

		if (firstLine) {
			writeConfig.open(fileName);
			firstLine = false;
		}

		writeConfig << offsetText << std::to_string(valToWrite) << std::endl;

		readConfig.open(fileName);
	}
	void Section(std::string sectionText) {
		sectionText = "[" + sectionText + "]";
		WriteLine("", sectionText);
	}
	//////////////////
	template <class T>
	T Read(std::string section, std::string offsetText) {
		int elementIndex = FindElement(section, offsetText);

		firstRead = false;

		if (elementIndex == -1)
			return 0;
		else {
			try {
				std::string temp = lines[elementIndex];
				RemoveSubStr(offsetText, temp);
				T ret;
				std::istringstream(temp) >> ret;
				return ret;
			}
			catch (...) {
				return T();
			}
		}
	}
	//////////////////
	void ClearFile() {
		std::ofstream clear;
		clear.open(fileName);
		clear.close();
	}
};
#endif
