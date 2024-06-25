#include "client.h"
 
int main() {
    run_client();
    return 0;
}
 
void run_client() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int choice;
 
    //create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    //converting IPv4 & IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }
 
    //connecting to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return;
    }
 
    //display menu
    printf("Menu:\n");
    printf("1. Print Protocol Statistics\n");
    printf("2. Print Routing Table Information\n");
    printf("3. Print Active TCP socket connections\n");
    printf("4. Print Active UDP socket connections\n");
    printf("5. Print all Internet connections using firefox/chrome\n");
    printf("6. Print all Internet connections where state is LISTEN/ESTABLISHED\n");
    printf("7. Exit\n");
    //get user choice
    printf("Enter your choice: ");
    scanf("%d", &choice);
 
    //send choice to server
    send(sock, &choice, sizeof(int), 0);
 
    if (choice == 7) {
        printf("Exiting...\n");
        close(sock);
        return;
    }
 
    //recieve and display server responce
    char buffer[1024] = {0};
    int bytes_received;
 
    while ((bytes_received = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }
 
    close(sock);
}
