#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void geomprog(int b, int q, int N, int clientSocket)
{
    int* gp = malloc(N * sizeof(int));
    gp[0] = b;
    for (int i = 1; i < N; i++)
        gp[i] = gp[i - 1] * q;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    for (int i = 0; i < N; i++)
    {
        sprintf(buffer, "%s%d ", buffer, gp[i]);
    }
    send(clientSocket, buffer, strlen(buffer), 0);
    free(gp);
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        fprintf(stderr, "Failed to create socket\n");
        return 1;
    }
    
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
    {
        fprintf(stderr, "Failed to bind socket\n");
        close(serverSocket);
        return 1;
    }
    
    if (listen(serverSocket, 1) == -1)
    {
        fprintf(stderr, "Failed to listen on socket\n");
        close(serverSocket);
        return 1;
    }
    while (1)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1)
        {
            fprintf(stderr, "Failed to accept client connection\n");
            continue;
        }
        int b, q, N;
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        sscanf(buffer, "%d %d %d", &b, &q, &N);
        geomprog(b, q, N, clientSocket);
        close(clientSocket);
    }
    close(serverSocket);
    return 0;
}
