////////////////////////////////////////////////////////////
//
// cpp-CowConfig
// Copyright (C) 2023 CowCorp
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

#ifndef COWCONFIG
#define COWCONFIG

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>

namespace CowConfig {
	class Section {
	private:
		template <class D>
		static D dummyStrFunc(std::string s) { 
			D ret;
			std::istringstream(s) >> ret;
			return ret; 
		}
		template <class D>
		static std::string dummyToStrFunc(D obj) {
			std::ostringstream s;
			s << obj;
			return s.str();
		}
	public:
		std::vector<std::string> raw_lines = {};
		std::map<std::string, std::string> lines = {};
		std::string Name;

		Section(std::string name) { this->Name = name; }
		Section() : Section("N/A") { }
		Section(std::string name, std::vector<std::string> raw_lines) : Section(name) {
			this->raw_lines = raw_lines;
		}

		void fillLinesMap() {
			for (auto raw_line : raw_lines) {
				int seperator = raw_line.find(": ");
				if (seperator != -1) {
					auto offsetText = raw_line.substr(0, seperator);
					auto valueText = raw_line.substr(seperator + 2);
					if (lines.find(offsetText) == lines.end())
						lines.insert(std::pair<std::string,std::string>(offsetText, valueText));
				}
			}
		}

		template <class T>
		T Read(std::string valueName, T(*conversionFunc)(std::string)) {
			fillLinesMap();

			auto readResult = lines.find(valueName);
			if (readResult == lines.end())
				return T();
			else {
				try {
					T ret;

					ret = conversionFunc(readResult->second);

					return ret;
				}
				catch (...) {
					return T();
				}
			}
		}
		template <class T>
		T Read(std::string valueName) { return Read<T>(valueName, dummyStrFunc<T>); }
		std::string Read(std::string valueName) { return Read<std::string>(valueName, dummyStrFunc); }

		template <class C>
		void Write(std::string valueName, C value, std::string(*toStringFunc)(C)) {
			raw_lines.push_back(valueName + ": " + toStringFunc(value));
		}
		template <class C>
		void Write(std::string valueName, C value) { Write<C>(valueName, value, dummyToStrFunc); }
		void Write(std::string valueName, std::string valueStr) { Write<std::string>(valueName, valueStr, dummyToStrFunc); }

		template <class C>
		std::vector<C> ReadVector(std::string valueName, C(*conversionFunc)(std::string)) {
			std::vector<C> result = {};
			std::string s = Read(valueName);
			while (s.find(",") != -1) {
				int seperatorPos = s.find(",");
				auto item_str = s.substr(0, seperatorPos);

				result.push_back(conversionFunc(item_str));

				s = s.substr(seperatorPos + 1);
			}
			return result;
		}
		template <class C>
		void WriteVector(std::string offsetText, std::vector<C> vec, std::string(*toStringFunc)(C)) {
			std::string built = "";
			for (auto item : vec) {
				built += toStringFunc(item) + ",";
			}
			Write(offsetText, built);
		}
		std::vector<std::string> ReadVector(std::string valueName){ return ReadVector<std::string>(valueName, dummyStrFunc); }
		void WriteVector(std::string offsetText, std::vector<std::string> vec) { WriteVector(offsetText, vec, dummyStrFunc); }
	};

	class Config {
	private:
		std::ofstream* writeConfig = nullptr;
		std::fstream* readConfig = nullptr;
		std::string currentFile = "";

		std::vector<Section*> sections = {};

		Section* _getSection(std::string name) {
			for (int s = 0; s < sections.size(); s++) {
				if (sections[s]->Name == name)
					return sections[s];
			}

			return nullptr;
		}
	public:
		Config(std::string file) {
			openFile(file);
		}

		std::string getCurrentFile() { return currentFile; }

		bool isOpen() {
			return (readConfig != nullptr && readConfig->is_open()) || (writeConfig != nullptr && writeConfig->is_open());
		}

		bool openFile(std::string fileName) {
			currentFile = fileName;

			if (readConfig == nullptr)
				readConfig = new std::fstream();
			if (writeConfig == nullptr)
				writeConfig = new std::ofstream();

			readConfig->open(fileName);
			bool open = readConfig->is_open();

			if (open) {
				std::string str;
				std::vector<std::string> lines;
				while (std::getline(*readConfig, str))
					lines.push_back(str);

				Section* currentSection = nullptr;
				for (auto line : lines) {
					if (line.find('[') == 0 && line.rfind(']') == line.length() - 1) {
						if (currentSection != nullptr) {
							sections.push_back(currentSection);
							currentSection = nullptr;
						}

						currentSection = new Section(line.substr(1, line.length() - 2), {});
					}
					else if (currentSection != nullptr) {
						currentSection->raw_lines.push_back(line);
					}
				}

				if (currentSection != nullptr) {
					sections.push_back(currentSection);
				}
			}

			return open;
		}
		
		Section* getSection(std::string name) {
			if (!readConfig->is_open()) {
				openFile(currentFile); // switching from write to read mode
			}

			return _getSection(name);
		}
		void writeSection(Section* section) {
			if (readConfig->is_open()) {
				readConfig->close(); // switch from read to write mode
				sections.clear();
			}

			assert(_getSection(section->Name) == nullptr);
			sections.push_back(section);
		}

		bool writeAllSections() {
			writeConfig->open(currentFile);

			if (!writeConfig->is_open())
				return false;

			for (auto section : sections) {
				*writeConfig << "[" << section->Name << "]\n";
				for (auto _line : section->raw_lines)
					*writeConfig << _line << "\n";
			}

			writeConfig->close();

			sections.clear();
			return true;
		}
	};
}

#endif
