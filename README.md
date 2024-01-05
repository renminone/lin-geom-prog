[В ПРОЦЕССЕ]

Для работы c программой запустите 2 терминала.

В первом напишите следующее:

`gcc - o server server.c`

`./server`

В втором напишите следующее:

`gcc - o client client.c`

`./client`

.

.

.

СЕРВЕРНАЯ ЧАСТЬ:

Функция geomprog получает на вход первоначальный элемент, шаг прогрессии и количество элементов.

Сначала создаётся массив для хранения элементов: `int* gp = malloc(N * sizeof(int));`. В качестве первого элемента записывается переменная b: `gp[0] = b;`

После этого запускается цикл записи элементов прогрессии в массив:

`for (int i = 1; i < N; i++)`

`   gp[i] = gp[i - 1] * q;`

После записи массива он преобразуется в строку buffer:

`for (int i = 0; i < N; i++) {`

`    sprintf(buffer, "%s%d ", buffer, gp[i]);`

`}`

Полученная строка отправляется на клиентский сокет: `send(clientSocket, buffer, strlen(buffer), 0);`, а память под массив gp высвобождается: `free(gp);`

Функция main является основным телом программы. Сначала происходит создание серверного сокета: `int serverSocket = socket(AF_INET, SOCK_STREAM, 0);`. Если не удаётся, выводится сообщение об ошибке, и программа завершает работу.


int main() {
 int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
 if (serverSocket == -1) {
 fprintf(stderr, "Failed to create socket\n");
 return 1;
 }
 struct sockaddr_in serverAddr;
 memset(&serverAddr, 0, sizeof(serverAddr));
 serverAddr.sin_family = AF_INET;
 serverAddr.sin_port = htons(8080);
 serverAddr.sin_addr.s_addr = INADDR_ANY;
 if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
 fprintf(stderr, "Failed to bind socket\n");
 close(serverSocket);
 return 1;
 }
 if (listen(serverSocket, 1) == -1) {
 fprintf(stderr, "Failed to listen on socket\n");
 close(serverSocket);
 return 1;
 }
 while (1) {
 int clientSocket = accept(serverSocket, NULL, NULL);
 if (clientSocket == -1) {
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



























