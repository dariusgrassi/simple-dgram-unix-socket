# simple-dgram-unix-socket
A Unix datagram socket server (in C) and client (in Python)

```sh
$ gcc server.c -o server
$ ./server &
$ python client.py
```

Output:
```
Server is listening on /tmp/dgram_server_socket
Sending: Message 1 from Python client
Received message: Message 1 from Python client
Received: Message 1 from Python client
Sending: Message 2 from Python client
Received message: Message 2 from Python client
Received: Message 2 from Python client
...
Received message: Message 10 from Python client
Received: Message 10 from Python client
Client socket closed
```

