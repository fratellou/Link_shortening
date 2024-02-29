# Link_shortening

It is required to implement a service for shortening links. The service must accept requests over the http protocol. The service must have two endpoints: 
1. Link shortening URL: / Method: The POST request contains a link that needs to be shortened. The response contains an abbreviated link. 
2. Clicking on the shortened URL: / Method: GET Response should redirect the user to the original page (see redirects) A DBMS from the repository is used to store abbreviated links https://github.com/fratellou/NoSQL-database. Implement a DBMS client in the language in which you write the link shortening service.

## Contents

1. [Information](#information)
2. [Code](#code)

## Information

HTTP Protocol: HTTP (Hypertext Transfer Protocol) is a data transfer protocol on the Internet. It is used to transfer requests from the client to the server and responses from the server to the client. Requests contain methods (GET, POST, PUT, DELETE, etc.), headers, and, in the case of POST and PUT requests, the message body. The responses contain status codes, headers, and message body.

DBMS (Database Management System): It is a software that is used to create and manage databases. The NoSQL DBMS mentioned in the assignment is a database that does not use the traditional relational data model. Instead, it uses other models such as key-value, etc. In this case, it is assumed to use a NoSQL database to store abbreviated links.

Client-server architecture: The link shortening service operates on the principle of a client-server architecture. The server accepts HTTP requests from clients, processes them and returns the corresponding HTTP responses.

Compiling a program through a Makefile:
> make

Next, you need to start the server:
> ./server

The next step is to launch a link shortening service:
> python3 shortener.py

And finally, launching the client:
> python3 client.py

## Code

### database.c

`main()` is the main function of the server:
- Initializes the mutex to ensure the security of access to shared resources;
- Creates a socket for the server using the `socket()` function. If socket creation fails, the program terminates with an error;
- Configures the server address using the `server_address` structure, specifying the AF_INET address family, the INADDR_ANY IP address and the port that the server will listen to;
- Binds the socket to the server address using the `bind()` function. If the binding fails, the program terminates with an error;
- Starts listening for incoming connections using the `listen()` function. If the listening failed, the program ends with an error;
- In an infinite loop, accepts incoming connections from clients using the `accept()` function. Each time the client connects, it creates a new thread to process client requests in the `handle_client()` function;
- Upon completion, closes the server socket and destroys the mutex.

`handle_client()` is a function for processing the client's request. Accepts a pointer to the `arg` argument, which is the client socket:
- Receives a request from a client via a socket and parses it;
- Calls the `request()` function to process the request;
- Sends a response to the client via the socket and closes the connection.

`request()` - routes requests to the appropriate processing functions. Accepts the name of the database file, a request from the client, and a pointer to a variable for the response:
- Defines the type of request (in this case only HSET, HDEL, HGET) and calls the appropriate function to process the request;
- In case of an incorrect request or error, it causes an error output.

### struct_reader.c

`read_struct_from_file()` - the function is designed to read data of a certain structure from a database file. Accepts `line` - an array of pointers to strings into which structure data will be saved, `filename` - the name of the database file from which data will be read, `*isnt_empty` - a pointer to a variable that will be set to 1 if structure data is found in the file, `struct_name` - the name of the structure the data of which needs to be read, `*size` is a pointer to the variable in which the number of lines read will be written, `struct_type` is the type of structure to be read:
- The function tries to open the database file for reading;
- If the file is not found, the function tries to create it for later use;
- Next, the file is closed and reopened for reading;
- If the file is successfully opened, the function starts reading the file line by line;
- Each string is read and split into separate words using the `strtok()` function;
- When a string corresponding to the specified structure is found, the data of this string is added to the `line` array;
- After finishing reading the file, the function closes it;
- In case of an error, an appropriate error message is displayed when opening a file or reading data.

### hash.c

`hash()` - performs hashing operations:
- Allocates memory for an array of lines `line`, which will contain lines from the file;
- Creates a hash table using the `createHashTable()` function;
- Reads structures from a file using the `read_struct_from_file()` function;
- If the file is not empty (`isnt_empty == 1`), goes through the rows, extracts the key and value, and adds them to the hash table using the `HSET()` function;
- Executes commands related to the hash table using the `hash_commands()` function;
- Writes the hash-the table is returned to the file using the `write_hash()` function;
- Frees up the memory allocated for the `line` array and hash table.

`hash_commands()` - processes commands related to the hash table:
- Checks the type of command (HSET, HDEL, HGET) and calls the appropriate function to execute this command;
- Copies the result of the command execution to the `req` variable.

`createHashTable()` creates a new hash table of the specified size:
- Allocates memory for the `HashTable` structure;
- Allocates memory for an array of pointers to hash table nodes and initializes them with zeros.

`hash_calc()` calculates the hash code for a given key:
- Counts the sum of the ASCII codes of the key characters;
- Returns the remainder of dividing this amount by the size of the hash table.

`HSET()`, `HDEL()`, `HGET()` perform the corresponding operations (adding, deleting, receiving an element) in the hash table. They search for a node with a given key and perform the appropriate actions:
- HSET: If an element with this key already exists, updates its value, otherwise adds a new element to the end of the list;
- HDEL: Removes the element with the specified key from the hash table;
- HGET: Retrieves the value of the element with the specified key.

`write_hash()` - writes the contents of the hash table to a file:
- Opens a temporary file for recording;
- Reads the contents of the source file and writes it to a temporary file, excluding the data structure that has already been updated;
- Adds the updated contents of the hash table to the end of the temporary file;
- Replaces the original file with a temporary one and closes the files.

`destroyHashTable()` - frees up the memory occupied by the hash table and its nodes:
- Goes through each element of the array of pointers to nodes and frees up the memory allocated for nodes;
- Frees up the memory allocated for the array of pointers to nodes and the `HashTable` structure itself.

### shortener.py

`save_to_database()` - saves a shortened key and a long URL link in the database:
- Creates a socket to connect to the database server;
- Establishes a connection to the database server specified in the variables `DB_HOST` and `DB_PORT`;
- Prepares a command to save data in the database in the format `HSET hash {short_key} {long_url}`;
- Sends a command to the database server;
- Closes the connection to the server.

`get_from_database()` extracts a long URL link from the database using a shortened key:
- Creates a socket to connect to the database server;
- Establishes a connection to the database server specified in the variables `DB_HOST` and `DB_PORT`;
- Prepares a command to get data from the database in the format `HGET hash {short_key}`;
- Sends a command to the database server and receives a response;
- Closes the connection to the server;
- Returns the received response (a long URL link).

`shorten_url()` - is a handler for POST requests to the root URL:
- Gets a long URL link from the body of the POST request;
- Generates a unique shortened key using the SHA-1 hash from a long URL link;
- Saves the shortened key and the long URL link in the database using the `save_to_database()` function;
- Returns an abbreviated key as the answer.

`redirect_to_original()` is a GET request handler with a shortened key in the URL:
- Retrieves a long URL link from the database using a shortened key using the `get_from_database()` function;
- If a long URL link is found, a redirect to this link is performed using the `redirect()` function;
- If a long URL link is not found, the error message "URL not found" is returned.

### client.py

`shorten_url()` - sends a POST request to the URL shortening service:
- Sets the URL of the abbreviation service;
- Sends a POST request with the original URL as data;
- If a response with code 200 is received, it returns the response text (abbreviated URL), otherwise it returns the string "Error".

`redirect_to_original()` - redirects to the original URL using a shortened key:
- Generates the URL of the abbreviation service with the transmission of the abbreviated key;
- Sends a GET request for a redirect to the original URL with automatic redirection disabled;
- If a response with the code 302 (redirect) is received, extracts the absolute URL from the "Location" header and displays it on the screen, otherwise displays the response text.

In the if block `__name__ == '__main__'`: 
- Entering the original URL, shortening the URL, displaying the shortened URL;
- Formation of a complete shortened URL using the base address of the shortening service;
- Calling the `redirect_to_original()` function to redirect to the original URL;
- If an error occurs during URL shortening, the message "Error" is displayed.

>
> fratellou, 2023