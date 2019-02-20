# This is the README.md for Lab 2 of the COEN 146 Computer Networks course

In this course, we will write several programs for transferring files in different situations, by changing the protocol underneath and the network conditions.

This is FTv1, File Transfer - version 1. It consists of a client and a server that communicate to transfer one file from the client to the server using TCP.

The client accepts 4 arguments: the name of the two files (<input> and <output>), and the IP address and the port number of the server. The server starts first and receives one argument, the port number.

The server waits for a connection request (listen and accept). The client requests a connection (connect) and then sends the name of file <output> (string + ‘\0’). To simplify, the name of the output file should have at most 9 characters. The server receives the name of the file, sends back to the client a Null character (‘\0’), opens the file, and waits for data.

The client receives the null character and opens file <input>. Then it reads file <input> and sends the data in chunks of 10 bytes to the server. The server receives the data in chunks of 5 bytes and writes the data to file <output>. 

After sending the file, the client closes the connection and exits.  The server closes the connection when no more data is available.

After executing, <input> and <output> should look the same. Your FTPv1 should be built on TCP. You must use C and Linux. You will need to use the socket library. There is a link in the course webpage that will help you get started. The man pages in the Unix/Linux systems have a lot of useful information. Start with <man socket>. There is a man page for each function in the socket library. 

To handle the files, you must use functions fread and fwrite only, since your FTPv1 should be able to transfer binary files as well as text files.
