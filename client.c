#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }
 
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        fprintf(stderr, "Failed to connect to server\n");
        close(clientSocket);
        return 1;
    }
    int b, q, N;
    printf("Server: Введите b0.\nClient: ");
    scanf("%d", &b);
    printf("Server: Введите q.\nClient: ");
    scanf("%d", &q);
    printf("Server: Введите N.\nClient: ");
    scanf("%d", &N);

    if (b == 0) printf("Server: ERROR! b = 0!\n");
    else
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d %d %d", b, q, N);
        send(clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        printf("Server: %s\n", buffer);
    }
    close(clientSocket);
    return 0;
}
