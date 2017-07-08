// JSONParser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JSONParser.h"

JSONParser::JSONParser(std::string d) {

	Utilities u;
	u.Erase(d, "\t\n");
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

std::vector<std::string> JSONParser::ParseArray(std::string Array, int keys=0) { //if keys = 1 then parse keys, or else parse array from JSON data
	int first = Array.find("[");
	Utilities u;
	int last = 0;
	std::vector<std::string> values;
	if (keys == 1) {
		int control_char = 0;
		char to_find = '[';
		while (Array.rfind("]") != std::string::npos && Array.find("[") != std::string::npos) {
			last = Array.rfind("]");			
			values.push_back(Array.substr(0, first));
			Array = Array.substr(first+1, last - first-1);
			first = Array.find("[");
		}
		values.push_back(Array);
	}else{
		last = Array.find("]");
		Array = Array.substr(first + 1, last - first - 1);
		std::string buffer;
		int pos[3];

		for (int i = 0; i < Array.length(); i++) {
			
			if (Array[i] == '{') {
				u.GetSyntacticalPos(Array.substr(i), '{', pos);
				buffer = Array.substr(i+pos[1], pos[2]-1);
				i += pos[2] + 1;
			}
			else if (Array[i] != ','){
				buffer += Array[i];
			}

			if (Array[i] == ',') {
				values.push_back(buffer);
				buffer.clear();
			}
		}
		values.push_back(buffer);
		buffer.clear();
	}
	return values;
}


std::string JSONParser::GetValue(std::string pair) {
	return pair.substr(pair.find(":") + 1, pair.length() - pair.find(":") - 1);
}

////////////////////////////////////////START OF JSON PROCESSING////////////////////////////////////////

/*
	below code is for parsing keys that are passed by other function.
	One by One keys are found from the string and result is returned.
	at the end if no key is passed, then residual is the information
	that is sent to screen.
	there are 3 types of logics 1. handle array, 2. handle object, 3.handle value

	coding status:
	handle values complete
	handle object wip
	handle array wip
*/

std::string JSONParser::GetValueFromKey(std::vector<std::string> keySeq, std::string information , int def) { // if def = 1 then use data else use information as it is
	if (def == 1) 	information = data;

	if (keySeq.size() == 0) {
		return information;
	}

	Utilities u;
	std::string answer;
	int pos[3];
	int keyPos = information.find(keySeq.front());
	keySeq.erase(keySeq.begin());
	int colonPos = information.find(":", keyPos);
	int commaPos = information.find(",", keyPos);
	if (commaPos == -1) commaPos = information.length();

////////////////////////OBJECT PARSER/////////////////////////////////

	if (u.Contains(information.substr(colonPos, commaPos - colonPos), "{") && !u.Contains(information.substr(colonPos, commaPos - colonPos), "[")) {
		u.GetSyntacticalPos(information.substr(colonPos), '{', pos);
		answer = GetValueFromKey(keySeq, information.substr(colonPos + pos[1], pos[2]-1),0);
	}
////////////////////////OBJECT PARSER END/////////////////////////////////

////////////////////////ARRAY PARSER /////////////////////////////////
	else if (u.Contains(information.substr(colonPos, commaPos - colonPos), "[")) {
		int pos[3];
		std::string val;
		u.GetSyntacticalPos( information.substr(colonPos) , '[' , pos);
		std::vector<std::string> vec = ParseArray(information.substr(colonPos + pos[1], pos[2]));
		if (keySeq.size() > 0) {
			val = vec[atoi(keySeq.front().c_str())];
			keySeq.erase(keySeq.begin());
		}
		else val = information.substr(colonPos + pos[1]-1, pos[2]+1);
		answer = GetValueFromKey(keySeq, val ,0);
	}
////////////////////////ARRAY PARSER END/////////////////////////////////
////////////////////////NON ARRAY NON OBJECT PARSER/////////////////////////////////
	else {
		answer = information.substr(colonPos+1, commaPos - colonPos-1);
	}

	return answer;		//main result
}

JSONParser::~JSONParser() {

}