#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int operation(int first, int sec, char *operator)
{
    int res;
    if (strcmp(operator, "+") == 0)
    {
        res = first + sec;
    }
    else if (strcmp(operator, "-") == 0)
    {
        res = first - sec;
    }
    else if (strcmp(operator, "*") == 0)
    {
        res = first * sec;
    }
    else if (strcmp(operator, "/") == 0)
    {
        res = first / sec;
    }
    return res;
}

int main(int argc, char *argv[])
{
    // Variable to store the host/server domain.
    struct sockaddr_in sa;
    char buf[200];
    int first, sec;
    char *operator;

    char *host = argv[1];
    int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error creating socket\n");
        return -1;
    }
    // bzero((char*)&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = inet_addr(argv[3]);

    char mess_serv[100];
    strcpy(mess_serv, "cs230 HELLO ");
    strcat(mess_serv, host);
    strcat(mess_serv, "\n");

    int cn = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    if (cn < 0)
    {
        printf("Error connecting to server\n");
        return -1;
    }

    int s = send(sockfd, mess_serv, strlen(mess_serv), 0);
    if (s < 0)
    {
        printf("Error sending messages to sever\n");
        return -1;
    }

    while (true)
    {
        int r = recv(sockfd, buf, 200, 0);
        if (r < 0)
        {
            printf("Error receiving messages from server\n");
            return -1;
        }
        char *tempbuf = strtok(buf, " ");
        tempbuf = strtok(NULL, " ");

        if (strcmp(tempbuf, "STATUS") != 0)
        {
            break;
        }

        first = atoi(strtok(NULL, " "));
        operator= strtok(NULL, " ");
        sec = atoi(strtok(NULL, " "));

        int res = operation(first, sec, operator);

        // if (strcmp(operator, "+") == 0)
        // {
        //     res = first + sec;
        // }
        // else if (strcmp(operator, "-") == 0)
        // {
        //     res = first - sec;
        // }
        // else if (strcmp(operator, "*") == 0)
        // {
        //     res = first * sec;
        // }
        // else if (strcmp(operator, "/") == 0)
        // {
        //     res = first / sec;
        // }

        strcpy(mess_serv, "cs230 ");
        char str[50];
        sprintf(str, "%d", res);
        strcat(mess_serv, str);
        strcat(mess_serv, "\n");

        int se = send(sockfd, mess_serv, strlen(mess_serv), 0);
        if (se < 0)
        {
            puts("Error sending message to server\n");
            return -1;
        }
    }

    close(sockfd);
    return 0;
}
