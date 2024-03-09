---
title: "Operating Systems (Part 4): Networking"
categories:
  - it basics
tags:
  - operating system
---

This post is part of a series on _Operating Systems_:
1. [File Systems, System Calls, Multiprocessing]({% post_url 2024-03-04-operating-systems-part1 %})
2. [Signals, and Race Conditions]({% post_url 2024-03-05-operating-systems-part2 %})
3. [Multithreading]({% post_url 2024-03-06-operating-systems-part3 %})
4. This post

Machines connecting over the internet is ever-present in our daily lives.
We will explore in this post how those connections are done on a low C level.

# Client-Server

A common scenario is that a machine is waiting on the internet for incoming connections and another machine initiates the contact.
The waiting machine is called a _server_ whereas the connection initiator the _client_.

Machines can be specified via a numeric name, i.e. their _IP addresses_.
The older IPv4 has 32 bits, e.g. `192.168.0.1`, and the newer IPv6 consists of a 128-bit number, e.g. `2001:db8:0:1234:0:567:8:`.

Additionally, a 16-bit _port_ number must be specified.
There are conventions which services await at which port, e.g. at port 80 a http server.

The server can also be specified via a domain name (e.g. www.facebook.com).
That name will be translated to an IP address via the domain name service (_DNS_).

# Implementation

## C

Connections are handled via _sockets_.
For a client program one would mostly specify the server's ip address and port to connect to.
A server program on the other hand would `bind()` to a specific port, `listen()` for incoming connections, and `accept()` them.
Data transmission is possible via `read()`, `write()` functions.

Note, this API is not trivial to use as one needs to check how much of the requested data has been sent/read.
Furthermore, there are many options to specify, and one has to check for many different error codes.

Documentation: man pages are available for specific commands (e.g. [`socket()`](https://www.man7.org/linux/man-pages/man2/socket.2.html), [`listen()`](https://www.man7.org/linux/man-pages/man2/listen.2.html)) as well as a [general overview](https://www.man7.org/linux/man-pages/man7/ip.7.html).

## C++

There are good abstractions available on-top of the low-level C socket library.
Quite often one builds streams around the sockets that handle the connection via the RAII idiom together with well-specified `operator<<()`, `operator>>()`.

For example libraries see this [list in an SO answer](https://stackoverflow.com/questions/118945/best-c-c-network-library).
Also note that networking is a current prospect for standardization.

## C example: "Hello world"

The following shows the basic case where a server waits on localhost, port 12345 for an incoming connection.
The client connects and receives a message from the server.

Note this is by no means production ready, e.g. error codes are not fully accounted for.

Server code:
```c
{% include operating-systems-part4_server.c %}
```

Client code:
```c
{% include operating-systems-part4_client.c %}
```

# FAQ

## How can multiple clients connect to the same server port/socket?

The server has one socket to listen for incoming connections.
The real connection is then established via a port/socket that is different to the listening port as well as different for each client.
