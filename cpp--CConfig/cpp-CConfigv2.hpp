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
#include <vector>
#include <algorithm>

class CConfig {
private:
	std::ofstream WriteConfig;
	std::fstream ReadConfig;
	bool FirstLine = true, FirstRead = true;
	std::string FileName;
  std::vector< std::string >Lines;
  void RemoveSubStr(std::string substr, std::string& str){
    size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = str.find(substr)) != std::string::npos)
		{
			// If found then erase it from string
			str.erase(pos, substr.length());
		}
  }
public:
	CConfig() {}
	CConfig(std::string fileName){
		OpenFile(fileName);
	}
	~CConfig() {
		if (ReadConfig.is_open())
			ReadConfig.close();
		if (WriteConfig.is_open())
			WriteConfig.close();
	}
  ///////////////////
  int FindElement(std::string Section, std::string offsetText){
    Section = "NEW_SECTION: [" + Section + "]";
    bool SectionFound;
    for (int i = 0; i < Lines.size(); i++){
      if (Lines[i] == Section)
        SectionFound = true;
      else if (SectionFound && Lines[i].find("NEW_SECTION: [") != std::string::npos)
        break;
      if (SectionFound && Lines[i].find(offsetText) != std::string::npos)
        return i;
    }
    return -1;
  }
	///////////////////
	bool OpenFile(std::string fileName) {
		FileName = fileName;

		ReadConfig.open(fileName);
		return (ReadConfig.is_open());
	} // Will open file with specified filename and will return whether the file is open
	//////////////////
	void WriteLine(std::string offsetText, std::string valToWrite) {
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine) {
			WriteConfig.open(FileName);
			FirstLine = false;
		}

		WriteConfig << offsetText << valToWrite << std::endl;

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
  void Section(std::string SectionText) {
    SectionText = "[" + SectionText + "]";
    WriteLine("NEW_SECTION: ", SectionText);
  }
	//////////////////
	int iRead(std::string Section, std::string offsetText) {
    if (FirstRead){
      std::string str;
      while (std::getline(ReadConfig, str)) {
        Lines.push_back(str);
      }
    }
    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return 0;
    else{
      try{
        RemoveSubStr(offsetText, Lines[LineFound]);
        return stoi(Lines[LineFound]);
      }
      catch (...){
        return 0;
      }
    }
	}
	float fRead(std::string Section, std::string offsetText) {
    if (FirstRead){
      std::string str;
      while (std::getline(ReadConfig, str)) {
        Lines.push_back(str);
      }
    }
    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return 0.0f;
    else{
      try{
        RemoveSubStr(offsetText, Lines[LineFound]);
        return stof(Lines[LineFound]);
      }
      catch (...){
        return 0.0f;
      }
    }
	}
	std::string sRead(std::string Section, std::string offsetText) {
    if (FirstRead){
      std::string str;
      while (std::getline(ReadConfig, str)) {
        Lines.push_back(str);
      }
    }
    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return "";
    else{
      try{
        RemoveSubStr(offsetText, Lines[LineFound]);
        return Lines[LineFound];
      }
      catch (...){
        return "";
      }
    }
	}
	bool bRead(std::string Section, std::string offsetText) {
    if (FirstRead){
      std::string str;
      while (std::getline(ReadConfig, str)) {
        Lines.push_back(str);
      }
    }
    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return false;
    else{
      try{
        RemoveSubStr(offsetText, Lines[LineFound]);
        return Lines[LineFound] == "1";
      }
      catch (...){
        return false;
      }
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
