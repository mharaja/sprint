#ifndef SERVER_H
#define SERVER_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
 
#define PORT 8089
#define MAX_CLIENTS 5
 
enum LogLevel {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG
};
 
void *handle_client(void *arg);
void execute_netstat_command(int client_socket, int choice);
void send_error(int client_socket);
void log_message(enum LogLevel level, const char *message);
 
#endif
