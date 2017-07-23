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
	class Node {
		public:
		std::string name;
		int count;
		std::vector<Node *> ptr;
		struct datatypeVar {
			unsigned int boolean : 1;
			unsigned int number : 1;
			unsigned int nullable : 1;
		}DTV;
		JSONPARSER Node() {
			count = 0;
			DTV.boolean = 0;
			DTV.number = 0;
			DTV.nullable = 0;
		}
	};

	Node *ROOT;
	int ErrCode = 0;
	std::string errBuffer;
	JSONPARSER void CreateTree(std::string JSON, Node *node);
	JSONPARSER void DestroyTree( Node *root);
	void Vectorize(std::string buff, std::vector<std::string> *vec);
public:
	JSONPARSER JSONParser( std::string filename );
	JSONPARSER ~JSONParser();
	JSONPARSER std::string FetchValue(std::string query);
	JSONPARSER int getObjectCountOf(std::string query);
	JSONPARSER std::string getErrorReason();
	JSONPARSER bool getErrorStatus();
};


