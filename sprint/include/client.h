#ifndef CLIENT_H
#define CLIENT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
#define PORT 8089
 
void run_client();
 
#endif
