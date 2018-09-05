////////////////////////////////////////////////////////////
//
// cpp-CConfig
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

#ifndef cppCConfig
#define cppCConfig

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

class CConfig {
private:
	std::ofstream WriteConfig;
	std::fstream ReadConfig;
	bool FirstLine = true;
	std::string FileName;
public:
	CConfig() {}
	~CConfig() {
		if (ReadConfig.is_open())
			ReadConfig.close();
		if (WriteConfig.is_open())
			WriteConfig.close();
	}
	///////////////////
	bool OpenFile(std::string fileName) {
		FileName = fileName;

		ReadConfig.open(fileName);
		return (ReadConfig.is_open());
	} // Will open file with specified filename and will return whether the file is open
	//////////////////
	void WriteLine(std::string Text) {
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine) {
			WriteConfig.open(FileName);
			FirstLine = false;
		}

		WriteConfig << Text << std::endl;

		ReadConfig.open(FileName);
	}
	template <class c>
	void WriteLine(std::string offsetText, c valToWrite) {
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine) {
			WriteConfig.open(FileName);
			FirstLine = false;
		}

		WriteConfig << offsetText << std::to_string(valToWrite) << std::endl;

		ReadConfig.open(FileName);
	}
	//////////////////
	int iRead(std::string offsetText) {
		std::string Line;
		std::getline(ReadConfig, Line);

		size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = Line.find(offsetText)) != std::string::npos)
		{
			// If found then erase it from string
			Line.erase(pos, offsetText.length());
		}
		try {
			return stoi(Line);
		}
		catch (...) {
			return 0;
		}

	}
	float fRead(std::string offsetText) {
		std::string Line;
		std::getline(ReadConfig, Line);

		size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = Line.find(offsetText)) != std::string::npos)
		{
			// If found then erase it from string
			Line.erase(pos, offsetText.length());
		}

		try {
			return stof(Line);
		}
		catch (...) {
			return 0.0f;
		}
	}
	std::string sRead(std::string offsetText) {
		std::string Line;
		std::getline(ReadConfig, Line);

		size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = Line.find(offsetText)) != std::string::npos)
		{
			// If found then erase it from string
			Line.erase(pos, offsetText.length());
		}

		try {
			return (Line);
		}
		catch (...) {
			return "";
		}
	}
	bool bRead(std::string offsetText) {
		std::string Line;
		std::getline(ReadConfig, Line);

		size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = Line.find(offsetText)) != std::string::npos)
		{
			// If found then erase it from string
			Line.erase(pos, offsetText.length());
		}

		try {
			return (Line == "1");
		}
		catch (...) {
			return false;
		}
	}
	//////////////////
	bool CheckLine(std::string cLine) {
		std::string Line;
		std::getline(ReadConfig, Line);

		return (Line == cLine);
	} // Will return whether the current line == std::string cLine
	//////////////////
	void ClearFile() {
		std::ofstream Clear;
		Clear.open(FileName);
		Clear.close();
	} // Will remove all lines of text from current opened file
};

#endif
