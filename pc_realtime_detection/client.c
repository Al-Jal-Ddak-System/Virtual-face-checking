#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char msg[2];
    char on[2] = "1";
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        printf("ERROR CONNECT");
    return (sock);
}

int data(int number, int sock)
{
    char msg[2];
    snprintf(msg, 2, "%d\n", number);
    write(sock, msg, sizeof(msg));
}