// JSONParser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "JSONParser.h"

JSONParser::JSONParser(std::string filename) {

	Utilities u;
	std::string JSON = u.OpenAndReadFileAndClose(filename);
	JSON = '"' + filename + '"' + ':' + JSON;
	root = new Node();
	CreateTree(JSON, root);
}
/*
	This function creates a tree structure to store data.
	Current program can handle Objects but it can't handle Array.
	Also Error handling is pending.

	Stat:
		Object Done
		Array WIP
		Error Handle WIP
*/
// Working Fine
void JSONParser::CreateTree(std::string JSON, Node *root) {

	Utilities u;
	while ( JSON.length() > 0 && u.GetFirstNonWhiteSpaceChar(JSON) != -1) {

		int StartPosition = JSON.find('"'), i;
		int LastPosition = JSON.find('"', StartPosition + 1);
		if (LastPosition != -1 && JSON[LastPosition-1] == '\\' ) LastPosition = JSON.find('"', LastPosition);
		std::string container = JSON.substr(StartPosition, LastPosition+1 - StartPosition);
		root->name = container;
		JSON = JSON.substr(LastPosition + 1);

		if (u.GetFirstNonWhiteSpaceChar(JSON) == ':') {
			int posColon = JSON.find(':');
			JSON = JSON.substr(posColon+1);
			if (u.GetFirstNonWhiteSpaceChar(JSON) == '{') {
				int pos[3];
				if (root->capacity == 0) {
					root->ptr = (Node **)malloc(300 * sizeof(Node *));
					root->capacity += 300;
					InitializePointer(root->ptr, root->count, root->capacity);
				}
				else {
					root->ptr = (Node **)realloc(root->ptr, root->capacity + 300);
					root->capacity += 300;
				}
				
				u.GetSyntacticalPos(JSON,'{',pos);
				pos[1]++;
				pos[2]--;
				std::string buff = JSON.substr(pos[1], pos[2] - pos[1]);
				std::vector<std::string> v = u.Vectorize(buff,",");
				for (int i = 0; i < v.size(); i++, root->count++) {
					CreateTree(v[i], root->ptr[root->count]);
				}
				v.clear();
				JSON = JSON.substr(pos[2]+1);
			}
			else if (u.GetFirstNonWhiteSpaceChar(JSON) == '[') {
				int pos[3];
				if (root->capacity == 0) {
					root->ptr = (Node **)malloc(300 * sizeof(Node *));
					root->capacity += 300;
					InitializePointer(root->ptr, root->count, root->capacity);
				}
				else {
					root->ptr = (Node **)realloc(root->ptr, root->capacity + 300);
					root->capacity += 300;
				}

				u.GetSyntacticalPos(JSON, '[', pos);
				pos[1]++;
				pos[2]--;
				std::string buff = JSON.substr(pos[1], pos[2]);
				std::vector<std::string> v = u.Vectorize(buff, ",");
				for (int i = 0; i < v.size(); i++, root->count++) {
					CreateTree( "\""+std::to_string(i)+"\":"+v[i], root->ptr[root->count]);
				}
				v.clear();
				JSON = JSON.substr(pos[2]+1);
			}
			else {
				int CommaPos = JSON.find(',');

				root->ptr = (Node **)malloc(1 * sizeof(Node *));
				root->capacity = 1;
				InitializePointer(root->ptr, root->count, root->capacity);

				if (CommaPos == -1) {
					CreateTree(JSON, root->ptr[root->count]);
				}
				else {
					CreateTree(JSON.substr(0, CommaPos), root->ptr[root->count]);
					JSON = JSON.substr(CommaPos);
				}
				root->count++;
				return;
			}
		}
		else if (u.GetFirstNonWhiteSpaceChar(JSON) == ',') {
			JSON = JSON.substr(JSON.find(','));
		}
		else {
			u.Erase(JSON, "\t\n ");
		}
	}
}


std::string JSONParser::FetchValue( std::string query ) {

	Node *pointer = root;
	Utilities u;
	std::vector<std::string> vec;

	for (int pos1 = query.find('['), pos2 = query.rfind(']'); pos1 != -1 && pos2 != -1;) {
		vec.push_back(query.substr(0, pos1));
		query = query.substr(pos1+1, pos2-pos1-1);
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
			if (vec.size() == 0) break;
			i = -1;
		}
	}
	return pointer->ptr[0]->name;
}

void JSONParser::InitializePointer(Node **ptr, int start, int end) {

	for (int i = start; i < end; i++) {
		ptr[i] = new Node();
	}
}
 JSONParser::~JSONParser() {
	 DestroyTree(root);
}

 void JSONParser::DestroyTree(Node *root) {
	 for (int i = 0; i < root->count; i++) {
		 if (root->ptr[i] != NULL) {
			 DestroyTree(root->ptr[i]);
		 }
		 else {
			 free(root->ptr[i]);
		 }
	 }
 }