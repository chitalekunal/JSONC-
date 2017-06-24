#pragma once
#pragma once
#ifdef JSONPARSER_COND
#define JSONPARSER __declspec(dllexport)
#else
#define JSONPARSER __declspec(dllimport)
#endif // JSONPARSER

#include <iostream>
#include <iterator>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include "..\Utilities\Utilities.h"

class JSONParser
{
	std::vector<std::string> JSONvec;
	std::string data;
	JSONPARSER std::vector<std::string> SeprateObjectsByComma(std::string Objects);
	JSONPARSER std::string GetObjects(std::string Objects);

public:
	JSONPARSER JSONParser(std::string data);
	JSONPARSER ~JSONParser();
	JSONPARSER std::string GetValue(std::string pair);
	JSONPARSER std::string GetValueFromKey(std::string keySeq);
	JSONPARSER bool isArray(std::string str);
};


