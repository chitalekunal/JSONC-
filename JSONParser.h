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
		Node **ptr;
		int capacity;
		JSONPARSER Node() {
			count = 0;
			ptr = 0;
			capacity = 0;
		}
	};

	Node *root;
	JSONPARSER void CreateTree(std::string JSON, Node *node);
	JSONPARSER void InitializePointer(Node **ptr, int start, int end);
public:
	JSONPARSER JSONParser( std::string filename );
	JSONPARSER ~JSONParser();
	JSONPARSER std::string FetchValue(std::string query);
};


