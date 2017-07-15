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
