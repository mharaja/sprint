#include "server.h"
 
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t thread_id;
 
    //creating server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_message(ERROR, "Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //initializing socket address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 

    //bind socket to address 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        log_message(ERROR, "Bind failed");
        exit(EXIT_FAILURE);
    }
 
    //listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        log_message(ERROR, "Listen failed");
        exit(EXIT_FAILURE);
    }
 
    printf("Server listening on port %d\n", PORT);
 
    //accepting incoming connections & handling using threads
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            log_message(ERROR, "Accept failed");
            continue;
        }
 
        printf("New connection, socket fd is %d, IP is: %s, port: %d\n",
               new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
 
	//creating thread for handling clients
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) != 0) {
            log_message(ERROR, "Thread creation failed");
            close(new_socket);
            continue;
        }
 
	//detaching the thread
        pthread_detach(thread_id);
    }

    log_message(FATAL,"Server failed unexpectedly");
 
    return 0;
}

//function to handle client request 
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    int choice;
 
    //receive client choice
    if (recv(client_socket, &choice, sizeof(int), 0) < 0) {
        log_message(ERROR, "Receive failed");
        close(client_socket);
        return NULL;
    }
 
    if (choice < 1 || choice > 7) {
        log_message(WARNING, "Invalid menu choice received");
        send_error(client_socket);
        close(client_socket);
        return NULL;
    }
 
    if (choice == 7) {
        log_message(INFO, "Client requested to exit");
        close(client_socket);
        return NULL;
    }
 
    //execute netstat commands
    execute_netstat_command(client_socket, choice);

    //close client 
    close(client_socket);
 
    return NULL;
}
 
//execute netstat commands
void execute_netstat_command(int client_socket, int choice) {
    FILE *fp;
    char buffer[1024];
 
    switch(choice) {
        case 1:
            system("netstat -s > out.txt");
            break;
        case 2:
            system("netstat -r > out.txt");
            break;
        case 3:
            system("netstat -tn > out.txt");
            break;
        case 4:
            system("netstat -un > out.txt");
            break;
        case 5:
            system("netstat -tanp | grep -i curl > out.txt");
            break;
        case 6:
            system("netstat -tanp | grep -iE ' (listen|established) ' > out.txt");
            break;
        default:
            log_message(WARNING, "Invalid menu choice received");
	    send_error(client_socket);
            return;
    }

    //sending output back to client 
    fp = fopen("out.txt", "r");
    if (fp == NULL) {
        log_message(ERROR, "Failed to open output file");
        return;
    }
 
    send(client_socket, "Response:\n", strlen("Response:\n"), 0);
 
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }
 
    fclose(fp);
}
 
//error message
void send_error(int client_socket) {
    char *error_message = "Invalid option. Please choose an option between 1 to 7.\n";
    send(client_socket, error_message, strlen(error_message), 0);
}
 
//log message function
void log_message(enum LogLevel level, const char *message) {
    const char *level_str;
    switch (level) {
        case FATAL:
            level_str = "FATAL";
            break;
        case ERROR:
            level_str = "ERROR";
            break;
        case WARNING:
            level_str = "WARNING";
            break;
        case INFO:
            level_str = "INFO";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }
    printf("[%s] %s\n", level_str, message);
}
