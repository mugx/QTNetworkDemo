# QTNetworkDemo

### Overview

QtNetworkDemo shows a client/server interaction using multithreading and tcp-sockets.  
Everytime you run the demo, you can choose if the current instance will be the Server side or the Client side.  
The server side solution uses a thread pool for the scheduling and you can easily change the maximum number of threads.  


### Simulation

Made by the following steps:

- The server loops and prints all the IP addresses of the connected clients.
- When a client connects to the given server port, a welcome message is sent to the client and printed on console.
- After 3 seconds the client sends a logout message to the server, it receives a goodbye message then ends the application.
.
