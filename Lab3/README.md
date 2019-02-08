This project consists of building an S&W (Stop and Wait) reliable protocol. TFv2 is going to be built on top
of UDP, and it is supposed to provide a reliable transport service to TFv1 (developed in week 3, which needs
to change to call your new send and receive functions and use buffers of same size). Messages are sent one
at a time, and each message needs to be acknowledged when received, before a new message can be sent.
TFv2 implements basically the protocol rdt2.2 presented in the text book.i

TFv2 consists of a client and a server. Communication is unidirectional, i.e., data flows from the client to
the server. The server starts first and waits for messages. The client starts the communication. Messages
have seq number 0 or 1 (start with zero). Before sending each message, a 1-byte checksum is calculated
and added to the header. After sending each message, the client waits for a corresponding ACK. When it
arrives, if it is not the corresponding ACK (or if the checksum does not match), the message is sent again. If
it is the corresponding ACK, the client changes state and returns to the application, which can now send
one more message. This means that TFv2 blocks on writes until an ACK is received.

The server, after receiving a message, checks its checksum. If the message is correct and has the right seq
number, the server sends an ACK0 or ACK1 message (according to the seq number) to the client, changes
state accordingly, and deliver data to the application.
The protocol should deal properly with duplicate data messages and duplicate ACK messages. Follow the
FSM in the book!

TFv2 message contains the header and the application data. No reordering is necessary, since TFv2 is
sending the exact message given by the application, one by one.
The checksum must be calculated for messages from the server and client. To calculate the checksum,
calculate the XOR of all the bytes (header + data of just header if there is no data) when member cksum
(see below) is zero. To verify your protocol, use the result of a random function to decide whether to send
the right checksum or just zero. This will fake the error effect.
