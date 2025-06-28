/* 
   A concurrent web server in C using fork()
   Usage: ./myserver <port>
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

int requestFile(char *buffer, char *head, char *f_name, char *f_type) {
    int idx = 0, chk = 0, fn_idx = 0, ft_idx = 0;
    f_name[fn_idx++] = '.';

    while (1) {
        if (chk == 0 && buffer[idx] == ' ') chk = 1;
        else if (chk == 1 || chk == 2) {
            if (chk == 1 && buffer[idx] == '.') chk = 2;
            if (chk == 1 && buffer[idx] == ' ') return 0;
            if (chk == 2 && buffer[idx] == ' ') break;
            f_name[fn_idx++] = buffer[idx];
            if (chk == 2) f_type[ft_idx++] = buffer[idx];
        }
        idx++;
    }
    f_name[fn_idx] = 0;
    f_type[ft_idx] = 0;

    if (!strcmp(f_type, ".html")) {
        strncat(head, "text/html\n\n", 4096 - strlen(head) - 1);
        FILE *fp = fopen(f_name, "r");
        if (fp) {
            char b[1024];
            while (fgets(b, sizeof(b), fp) != NULL) {
                strncat(head, b, 4096 - strlen(head) - 1);
            }
            fclose(fp);
            return 1;
        } else return 0;
    } else {
        if (!strcmp(f_type, ".jpg"))
            strncat(head, "image/jpg\n\n", 4096 - strlen(head) - 1);
        else if (!strcmp(f_type, ".jpeg"))
            strncat(head, "image/jpeg\n\n", 4096 - strlen(head) - 1);
        else if (!strcmp(f_type, ".gif"))
            strncat(head, "image/gif\n\n", 4096 - strlen(head) - 1);
        else if (!strcmp(f_type, ".mp3"))
            strncat(head, "audio/mpeg3\n\n", 4096 - strlen(head) - 1);
        else if (!strcmp(f_type, ".pdf"))
            strncat(head, "application/pdf\n\n", 4096 - strlen(head) - 1);
        else if (!strcmp(f_type, ".ico"))
            strncat(head, "image/x-icon\n\n", 4096 - strlen(head) - 1);
        else return 1;

        FILE *fp = fopen(f_name, "rb");
        if (fp) {
            fclose(fp);
            return 2;
        } else return 0;
    }
}

void sendBinary(int newsockfd, char *head, const char *f_name) {
    FILE *fp = fopen(f_name, "rb");
    fseek(fp, 0, SEEK_END);
    int f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *reply = (char *)malloc(strlen(head) + f_size);
    strcpy(reply, head);

    unsigned char *buff = (unsigned char *)malloc(f_size);
    fread(buff, 1, f_size, fp);
    memcpy(reply + strlen(head), buff, f_size);

    write(newsockfd, reply, strlen(head) + f_size);
    printf("Send Complete!\n\n");

    fclose(fp);
    free(reply);
    free(buff);
}

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);

    while (1) {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");

        pid_t pid = fork();
        if (pid < 0) {
            error("ERROR on fork");
        } else if (pid == 0) {
            close(sockfd);
            char buffer[1024];
            bzero(buffer, 1024);
            int n = read(newsockfd, buffer, 1023);
            if (n < 0) error("ERROR reading from socket");

            char f_name[256] = {0}, f_type[256] = {0};

            printf("Client [%s] requested:\n%s\n", inet_ntoa(cli_addr.sin_addr), buffer);
            char notFound[1024] = "HTTP/1.1 404 Not Found\nContent-type: text/html\n\n<html>\n\t<body>\n\t\t<h1>Not Found</h1>\n\t\t<p>The requested URL was not found on this server.</p>\n\t</body>\n</html>\n";
            char head[4096] = "HTTP/1.1 200 OK\nAccept-Ranges: bytes\nContent-type: ";

            int ret = requestFile(buffer, head, f_name, f_type);
            if (ret == 1) {
                write(newsockfd, head, strlen(head));
            } else if (ret == 2) {
                sendBinary(newsockfd, head, f_name);
            } else {
                write(newsockfd, notFound, strlen(notFound));
            }
            close(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
    }
    close(sockfd);
    return 0;
}
