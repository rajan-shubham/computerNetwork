# Computer Networks

This repository contains a collection of basic network programming in C. These programs demonstrate core concepts of computer networking such as TCP and UDP communication, echo servers, and chat servers.

## 📁 Contents

- `tcp_echo_client.c` – A simple TCP echo client.
- `tcp_echo_server.c` – A TCP echo server that sends received messages back to the client.
- `udp_echo_client.c` – A simple UDP echo client.
- `udp_echo_server.c` – A UDP echo server that echoes back messages.
- `tcp_chat_server.c` – A basic multi-client chat server using TCP.
- `udp_chat_server.c` – A simple chat server using UDP.
- `client.c` – General-purpose client code (for TCP/UDP chat/echo).
- `Makefile` – Easy build configuration for all programs.

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- Linux or Unix-based OS (recommended for sockets)

### Build

Use the provided `Makefile` to compile all programs:

```bash
make
