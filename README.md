# JSONCpp

A light weight JSON parser that can be used in applications.
The code is created using VC++ hence it is tested on Windows Platform.
Currently I am testing it for Linux and its limitations.

The program needs filename and JSON query as an Input.
Using JSONParser constructor you can pass the file name.
and query will help you fetch the result from the JSON file.

~~~~
	JSONParser j(<filename>);
	j.FetchValue(<query>);
~~~~

In above example <filename> and <query> are the standard string object (std::string).

Below is an example of how query is provided.
Consider following JSON example
~~~~
{
    "quiz": {
        "sport": {
            "q1": {
                "question": "Which one is correct team name in NBA?",
                "options": [
                    "New York Bulls",
                    "Los Angeles Kings",
                    "Golden State Warriros",
                    "Huston Rocket"
                ],
                "answer": "Huston Rocket"
            }
        }
	}
}
~~~~