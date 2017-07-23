// JSONParser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JSONParser.h"

JSONParser::JSONParser(std::string filename) {

	Utilities u;
	std::string JSON = u.OpenAndReadFileAndClose(filename);
	JSON = '"' + filename + '"' + ":{" + JSON + "}";
	ROOT = new Node();
	CreateTree(JSON, ROOT);
}
/*
	This function creates a tree structure to store data.
	Current program can handle Objects but it can't handle Array.
	Also Error handling is pending.

	Stat:
		Object Done
		Array Done
		Error Handle WIP
*/
// Working Fine
void JSONParser::CreateTree(std::string JSON, Node *root) {

	Utilities u;
	char a = u.GetFirstNonWhiteSpaceChar(JSON);
	if (a == '{' || a == '[') {
		int pos1[3];
		u.GetSyntacticalPos(JSON, a,pos1);
		JSON = JSON.substr(pos1[1]+1,pos1[2]-pos1[1]-2);
	}
	while ( JSON.length() > 0 && u.GetFirstNonWhiteSpaceChar(JSON) != -1) {
		int StartPosition = JSON.find('"'), i;
		int LastPosition;
		if (StartPosition != -1) {
			LastPosition = JSON.find('"', StartPosition + 1);
			if (LastPosition != -1 && JSON[LastPosition - 1] == '\\') LastPosition = JSON.find('"', LastPosition);
			std::string container = JSON.substr(StartPosition, LastPosition + 1 - StartPosition);
			root->name = container;
			JSON = JSON.substr(LastPosition + 1);
		}
		else if (u.is_number(JSON)) {
			root->DTV.number = 1;
			root->name = JSON;
			return;
		}
		else if (JSON == "true" || JSON == "false") {
			root->DTV.boolean = 1;
			root->name = JSON;
			return;
		}
		else if (JSON == "null") {
			root->DTV.nullable = 1;
			root->name = JSON ;
			return;
		}
		char firstNonSpace = u.GetFirstNonWhiteSpaceChar(JSON);
		if (firstNonSpace == ':') {
			int posColon = JSON.find(':');
			JSON = JSON.substr(posColon+1);
			char secondNonSpace = u.GetFirstNonWhiteSpaceChar(JSON);
			if (secondNonSpace == '{') {
				int pos[3];
				std::vector<std::string> v1;
				u.GetSyntacticalPos(JSON,'{',pos);
				pos[1]++;
				pos[2]--;
				std::string buff = JSON.substr(pos[1], pos[2] - pos[1]);
				Vectorize(buff,&v1);
				for (int i = 0; i < v1.size(); i++, root->count++) {
					root->ptr.push_back(new Node());
					CreateTree(v1[i], root->ptr[root->count]);
				}
				v1.clear();
				JSON = JSON.substr(pos[2]+1);
			}
			else if (secondNonSpace == '[') {
				int pos[3];
				std::vector<std::string> v2;
				u.GetSyntacticalPos(JSON, '[', pos);
				pos[1]++;
				pos[2]--;
				std::string buff = JSON.substr(pos[1], pos[2]);
				Vectorize(buff, &v2);
				for (int i = 0; i < v2.size(); i++, root->count++) {
					root->ptr.push_back(new Node());
					CreateTree( "\"____"+std::to_string(i)+"____\":"+v2[i], root->ptr[root->count]);
				}
				v2.clear();
				JSON = JSON.substr(pos[2]+1);
			}
			else {
				int CommaPos = JSON.find(',');


				if (CommaPos == -1) {
					root->ptr.push_back(new Node());
					CreateTree(JSON, root->ptr[root->count]);
				}
				else {
					root->ptr.push_back(new Node());
					CreateTree(JSON.substr(0, CommaPos), root->ptr[root->count]);
					JSON = JSON.substr(CommaPos);
				}
				root->count++;
				return;
			}
		}
		else if (firstNonSpace == ',') {
			JSON = JSON.substr(JSON.find(','));
		}
		else {
			u.Erase(JSON, "\t\n ");
		}
	}
}


std::string JSONParser::FetchValue( std::string query ) {

	Node *pointer = ROOT;
	Utilities u;
	bool decision;
	std::string buffer;
	std::vector<std::string> vec;
	std::istringstream iss(query);

	while (getline(iss, buffer, '.')) {
		int p1, p2;
		std::string temp;
		if((p1 = buffer.find('[') )> -1 && (p2 = buffer.find(']') )> -1) {
			while ((p1 = buffer.find('[')) > -1 && (p2 = buffer.find(']')) > -1) {
				temp = buffer.substr(0, p1);
				if (temp != "") vec.push_back(temp);
				temp = buffer.substr(p1 + 1, p2 - p1 -1);
				vec.push_back("____"+temp+"____");
				buffer = buffer.substr(p2 + 1);
			}
		}else vec.push_back(buffer);
	}

	for (int i = 0; i < pointer->count; i++) {

		std::string buff = pointer->ptr[i]->name;
		u.Erase(buff, "\"");
		if (vec.front() == buff) {
			pointer = pointer->ptr[i];
			vec.erase(vec.begin());
			decision = true;
			if (vec.size() == 0) break;
			i = -1;
		}
		else decision = false;
	}

	if ( decision )	return pointer->ptr[0]->name;
	else {
		ErrCode = 1;
		errBuffer = vec.front();
		return std::string();
	}
}

int JSONParser::getObjectCountOf(std::string query)
{
	Node *pointer = ROOT;
	Utilities u;
	bool decision = false;
	std::vector<std::string> vec;

	for (int pos1 = query.find('['), pos2 = query.rfind(']'); pos1 != -1 && pos2 != -1;) {
		vec.push_back(query.substr(0, pos1));
		query = query.substr(pos1 + 1, pos2 - pos1 - 1);
		pos1 = query.find('[');
		pos2 = query.rfind(']');
	}
	vec.push_back(query);

	for (int i = 0; i < pointer->count; i++) {

		std::string buff = pointer->ptr[i]->name;
		u.Erase(buff, "\"");
		if (vec.front() == buff) {
			pointer = pointer->ptr[i];
			vec.erase(vec.begin());
			decision = true;
			if (vec.size() == 0) break;
			i = -1;
		}
		else decision = false;
	}

	if (decision) {
		return pointer->ptr[0]->count;
	}
	else {
		ErrCode = 1;
		errBuffer = vec.front();
		return -1;
	}
}

std::string JSONParser::getErrorReason()
{
	if (ErrCode == 0) {
		return "No Error";
	}
	else if (ErrCode == 1) {
		return "Invalid Key : " + errBuffer;
	}
	else if (ErrCode == 2) {
		return "Key Syntax Error : " + errBuffer;
	}
	return "Unknown";
}

bool JSONParser::getErrorStatus()
{
	return (ErrCode > 0);
}


 JSONParser::~JSONParser() {
	 DestroyTree(ROOT);
}

 void JSONParser::DestroyTree(Node *root) {
	 for (int i = 0; i < root->count; i++) {
		 if (root->ptr[i] != NULL) {
			 DestroyTree(root->ptr[i]);
		 }
		 else {
			 delete root;
		 }
	 }
 }
 void JSONParser::Vectorize(std::string buff, std::vector<std::string> *vec) {
	 Utilities u;
	 std::string buffer;
	 int i = 0, pos[3];

	 while (buff.length() > i && u.GetFirstNonWhiteSpaceChar(buff) != -1) {
		 if (i > 0) {
			 if (buff[i] == '"' && buff[i - 1] != '\\') {
				 int strt = buff.find('"', i);
				 int end = buff.find('"', strt + 1);
				 buffer += buff.substr(strt, end - strt + 1);
				 i += end - strt + 1;
			 }
			 else if (buff[i] == '{' && buff[i - 1] != '\\') {
				 u.GetSyntacticalPos(buff.substr(i), '{', pos);
				 buffer += buff.substr(pos[1] + i, pos[2]);
				 i += pos[2];
			 }
			 else if (buff[i] == '[' && buff[i - 1] != '\\') {
				 u.GetSyntacticalPos(buff.substr(i), '[', pos);
				 buffer += buff.substr(pos[1] + i, pos[2]);
				 i += pos[2];
			 }
			 else if (buff[i] >= '0' && buff[i] <= '9') {
				 int number = u.getNumber(buff.substr(i));
				 if (u.ERRNOUTILITIES != 0) continue;
				 std::string num_alfa = std::to_string(number);
				 buffer += num_alfa;
				 i += num_alfa.length();
			 }
			 else if (u.is_bool(buff.substr(i))) {
				 int p = buff.find(',', i);
				 std::string t = buff.substr(i, p - i);
				 if (p == -1) {
					 p = t.length();
					 i += p;
				 }
				 else {
					 i = p;
				 }
				 u.Erase(t, "\t\n ");
				 buffer += t;
			 }
			 else if (u.is_null(buff.substr(i))) {
				 int p = buff.find(',', i);
				 std::string t = buff.substr(i, p - i);
				 if (p == -1) {
					 p = t.length();
					 i += p;
				 }
				 else {
					 i = p;
				 }
				 u.Erase(t, "\t\n ");
				 buffer += t;
			 }

			 if (buff[i] == ',' && buff[i - 1] != '\\') {
				 vec->push_back(buffer);
				 buffer.clear();
			 }
			 else if (buff[i] == ':' && buff[i - 1] != '\\') {
				 buffer += ':';
			 }
			 i++;
		 }
		 else {
			 if (buff[i] == '"' && buff[i - 1] != '\\') {
				 int strt = buff.find('"', i);
				 int end = buff.find('"', strt + 1);
				 buffer += buff.substr(strt, end - strt + 1);
				 i += end - strt + 1;
			 }
			 else if (buff[i] == '{') {
				 u.GetSyntacticalPos(buff.substr(i), '{', pos);
				 buffer += buff.substr(pos[1] + i, pos[2]);
				 i += pos[2];
			 }
			 else if (buff[i] == '[') {
				 u.GetSyntacticalPos(buff.substr(i), '[', pos);
				 buffer += buff.substr(pos[1] + i, pos[2]);
				 i += pos[2];
			 }
			 else if (buff[i] >= '0') {
				 int number = u.getNumber(buff.substr(i));
				 if (u.ERRNOUTILITIES != 0) continue;
				 std::string num_alfa = std::to_string(number);
				 buffer += num_alfa;
				 i += num_alfa.length();
			 }
			 else if (u.is_bool(buff.substr(i))) {
				 int p = buff.find(',', i);
				 std::string t = buff.substr(i, p - i);
				 if (p == -1) {
					 p = t.length();
					 i += p;
				 }
				 else {
					 i = p;
				 }
				 u.Erase(t, "\t\n ");
				 buffer += t;
			 }
			 else if (u.is_null(buff.substr(i))) {
				 int p = buff.find(',', i);
				 std::string t = buff.substr(i, p - i);
				 if (p == -1) {
					 p = t.length();
					 i += p;
				 }
				 else {
					 i = p;
				 }
				 u.Erase(t, "\t\n ");
				 buffer += t;
			 }

			 if (buff[i] == ',') {
				 vec->push_back(buffer);
				 buffer.clear();
			 }
			 else if (buff[i] == ':') {
				 buffer += ':';
			 }
			 i++;
		 }
	 }
	 vec->push_back(buffer);
	 buffer.clear();
 }
