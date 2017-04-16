# Simple-Network-Programming
Client - Server Connection :-

Both the server and the client were run on:
OS X Yosemite Version 10.10.5 ; Processor 1.3 GHz Intel Core i5

Steps to run:
1) Download server1.c, client1.c, and the two text files attached and store them in a folder.
2) Now, open two terminal windows (one for server, and one for client).
3) In one window, compile server1.c by typing:
	gcc server1.c -o server
4) In another window, compile client1.c by typing:
	gcc client1.c -o client
5) To run server, type:
	./server 5500
6) To run client, type:
	./client localhost 5500
7) Now the server and client are connected.
8) Execute the following commands:
	-> BOUNCE This should be displayed
	-> GET Test1.txt
	-> EXIT
