-UNIX Socket Programming Project

a. Name: Yicheng Zheng

c. Tasks Completed: 
	1. Established TCP connection between AWS server and monitor, and client. 
	2. Established UDP connection between AWS server 
	3. Extra credit suffix function has been implemented. 
	   Took user input from the console, and then implemented suffix, prefix, and search functions in Server A, B, and C in their associated dictionary. Additional processing was done on the user input word such that it will be case insensitive on the first character. (i.e., ./client prefix Al will be the same as ./client prefix al).
	4. Results obtained from each backend server were sent to AWS and then fed to both client and monitor for display. 
	5. Makefile completed per project requirement. 
	   make clean will delete all executable previously generated. 



d. Files included and their main functions: 

1. aws.cpp
	- Created TCP socket for both client and monitor, binded each TCP socket to its respective port number and IP address, and then listen for incoming connections.
	- Maintained accepting TCP connections.
	- Received user input word and function from client side via TCP connection.
	- Sent input word and function to backend server (A, B, and C) and received back list of words via UDP connection. This is done in sequential order (Server A, then Server B, and finally Server C).
	- Aggregated list of words from each backend server in a vector and sent it to client and monitor via TCP. Due to difference in search and prefix/suffix, the send and receive function for search and prefix/suffix is coded differently.
	- If there's no matching words from all three backend servers, sent message to client and monitor mentioning no results found.
	- After received results from all three backend servers, closed UDP socket. 


2. serverA.cpp
	- Created UDP socket and binded to associated IP address and port number. 
	- Read in dictionary file backendA.txt line by line. 
	- Parsed dictionary information into keyword and its associated definition and stored them into separate vectors. 
	- Kept receiving data from AWS server on user input function and words.
	- Separate helper functions created to perform search, suffix, and prefix. 
	- Depending on input function, performed search on the vectors created from reading dictionary file. Gathered all matching words and sent back to AWS via UDP connection. 

3. serverB.cpp
	- Created UDP socket and binded to associated IP address and port number. 
	- Read in dictionary file backendB.txt line by line. 
	- Parsed dictionary information into keyword and its associated definition and stored them into separate vectors. 
	- Kept receiving data from AWS server on user input function and words.
	- Separate helper functions created to perform search, suffix, and prefix. 
	- Depending on input function, performed search on the vectors created from reading dictionary file. Gathered all matching words and sent back to AWS via UDP connection. 

4. serverC.cpp
	- Created UDP socket and binded to associated IP address and port number. 
	- Read in dictionary file backendC.txt line by line. 
	- Parsed dictionary information into keyword and its associated definition and stored them into separate vectors. 
	- Kept receiving data from AWS server on user input function and words.
	- Separate helper functions created to perform search, suffix, and prefix. 
	- Depending on input function, performed search on the vectors created from reading dictionary file. Gathered all matching words and sent back to AWS via UDP connection. 


5. client.cpp
	- Take user input function argument for word and function to be performed. 
	- Created TCP socket and connected to the AWS server. 
	- Sent user input data to AWS via TCP. 
	- Received list of matching words from AWS, and displayed related matching words information. 
	- Closed TCP socket.

6. monitor.cpp
	- Created TCP socket and connected to the AWS server. 
	- Received list of matching words from AWS, and displayed words and definition depending on the function performed. 
	- Maintained TCP connection with the AWS via a infinite loop


e. Format of message exchanged. 
	- Client to AWS
		* Function to be performed and word to be searched were sent as character to the AWS. 

	- AWS to client
		* All matching words from the each backend server were aggregated in a String vector. Prior to sending words in the vector to the client, the size of vector is sent first so that the number of send/receive pair would be known. 
		* Each string in the matching word vector was sent via loop from AWS to client. 
		* If the function performed is search, only send one word and associated definition to the client. 

	- AWS to backend server A, B, C
		* The function to be performed and word to be searched were sent as character to each backend server via UDP. 

	- Backend server A, B, C to AWS
		* After finding all matching words in the associated dictionary file, the words were appended in a vector, as well as their associated definition if the function to be performed is search. 
		* Content in the vectors were sent to the AWS in a loop. 

	- AWS to monitor
		* AWS gathered all matching words and definition from all three backend servers in two separate vectors. 
		* Content in the vectors were sent to monitor in a loop. 
		* After received all words from AWS, monitor displayed required list of words and definition.

g. Limitation: All messages sent were default to size of 101 byte. Therefore, if there're too many results to be sent continuously (>500 messages), the heap memory of the VM will run out and cause the project to fail. 

h. Reused Code: 
	- Beej & GeeksforGeeks, Socket Programming in C/C++ (https://www.geeksforgeeks.org/socket-programming-cc/)
		socket()
		bind()
		listen()
		recv()
		sendto()
		send()
		recvfrom()
		
	- Socket Communication Tutorial (http://users.pja.edu.pl/~jms/qnx/help/tcpip_4.25_en/prog_guide/sock_advanced_tut.html)
		struct sockaddr_in




