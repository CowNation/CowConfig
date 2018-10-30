# cpp-CowConfig

CowConfig is a small, portable and simple file saver and parser that is mainly aimed at saving & parsing variables to/from a file.

## Functions
CowConfig(std::string fileName) - Calls OpenFile with fileName as parameter on construction of object. (See bool OpenFile(std::string fileName))

bool OpenFile(std::string fileName) - Opens a file using fstring object and stored the specified fileName.

void WriteLine(std::string offsetText, std::string valToWrite) - Will write offsetText and valToWrite to the currently opened file.

void WriteLine(std::string offsetText, c valToWrite) - Will write offsetText and valToWrite to the currently opened file.

void Section(std::string SectionText) - Writes a line to the currently opened file in this format: '[SectionText]'. Sections are used for searching within only that section of the file when attempting to read a line.

int iRead(std::string Section, std::string offsetText) - Returns value of the specified line within the specified section, will return 0 if line is not found.

float fRead(std::string Section, std::string offsetText) - Returns value of the specified line within the specified section, will return 0 if line is not found.

std::string sRead(std::string Section, std::string offsetText) - Returns value of the specified line within the specified section, will return "" if line is not found.

void ClearFile() - Using an ofstream object, this will clear all lines of text from the currently opened file.
