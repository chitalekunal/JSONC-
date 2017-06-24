// JSONParser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JSONParser.h"

JSONParser::JSONParser(std::string d) {

	Utilities u;
	u.Erase(d, "\t\n\"");
	data = d;
}

std::vector<std::string> JSONParser::SeprateObjectsByComma(std::string Objects) {
	std::istringstream iss(Objects);
	std::string buffer;
	std::vector<std::string> JSONvec;

	return JSONvec;
}

std::string JSONParser::GetObjects(std::string Objects) {
	int first = Objects.find("{"),i;
	std::string obj = Objects.substr(first);
	int count = 0;
	int pos_from_first;
	for ( i = 0; i < Objects.length(); i++) {
		if (obj[i] == '{') count++;
		else if (obj[i] == '}') count--;
	}

	if (count == 0) {
		return Objects.substr(first, i - first);
	}
	else {
		perror("Malformed JSON file");
		return nullptr;
	}
}

std::vector<std::string> ParseArray(std::string Array, int keys=0) { //if keys = 1 then parse keys, or else parse array from JSON data
	int first = Array.find("[");
	int last = Array.find("]");
	std::vector<std::string> values;
	if (keys == 1) {
		values.push_back(Array.substr(0, first));
	}
	Array = Array.substr(first + 1, last - first - 1);
	std::istringstream iss(Array);
	std::string buffer;

	while (std::getline(iss, buffer, ',')) {
		values.push_back(buffer);
	}

	return values;
}


std::string JSONParser::GetValue(std::string pair) {
	return pair.substr(pair.find(":") + 1, pair.length() - pair.find(":") - 1);
}

////////////////////////////////////////START OF JSON PROCESSING////////////////////////////////////////

std::string JSONParser::GetValueFromKey(std::string keySeq) {
	std::vector<std::string> keys = ParseArray(keySeq,1), arr;
	Utilities u;
	std::string container;
	std::string value;
	
	int first= data.find(keys.front());
	keys.erase(keys.begin());
	int colon_pos = data.find(":", first);
	int comma_pos = data.find(",", first);
	if ( u.Contains( data.substr( colon_pos+1, comma_pos - colon_pos -1 ), "[{" ) == 0 ) {
		return data.substr(colon_pos+1, data.find(",", first) - colon_pos-1);
	}
	else if (u.Contains(data.substr(colon_pos + 1, comma_pos - colon_pos - 1), "[") == 1 && u.Contains(data.substr(colon_pos + 1, comma_pos - colon_pos - 1), "{") == 0)
	{
		int pos = data.substr(colon_pos + 1).find(']');
		arr = ParseArray(data.substr(colon_pos + 1, colon_pos - pos));
		if (keys.size() != 0) {
			return arr[std::stoi(keys.front().c_str())];
		}
		else {
			while (arr.size() > 1) {
				container += arr.front() + ",";
				arr.erase(arr.begin());
			}
			container += arr.front();
			arr.erase(arr.begin());
			return container;
		}
	}
}

JSONParser::~JSONParser() {

}