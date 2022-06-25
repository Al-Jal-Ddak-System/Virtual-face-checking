#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "gpio.h"

#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define PIN 20
#define POUT 21

pthread_mutex_t mutex_lock;

void reset(int *a,int *b,int *c) {
    pthread_mutex_lock(&mutex_lock);
    *a = 0;
    *b = 0;
    *c = 0;
    pthread_mutex_unlock(&mutex_lock);
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
void* camera(void *data)
{
    int heart=0;
    int sound=0;
    int emotion=0;
   
    int sockfd = *((int *) data);
    int readn;
    socklen_t addrlen;
    char buf[1024];
    struct sockaddr_in clnt_addr;
    memset(buf, 0x00, 1024);
    addrlen = sizeof(clnt_addr);
    getpeername(sockfd, (struct sockaddr *)&clnt_addr, &addrlen);
    
    int std_bpm = 50;
    while((readn = read(sockfd, buf, 1024)) > 0) {
        char *msg;
        FILE *fp = fopen("lcd.txt", "w");
        // printf("DATA : %s (%d) : %s", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port), buf);
        if(buf[0] == 'h') {
            // h++;
            char *hh = strtok(buf, "=");
            hh = strtok(NULL, " ");
            pthread_mutex_lock(&mutex_lock);
            heart = atoi(hh);
            pthread_mutex_unlock(&mutex_lock);
        }
        if(buf[0] == 's') {
            char *ss = strtok(buf, "=");
            ss = strtok(NULL, " ");
            pthread_mutex_lock(&mutex_lock);
            sound = atoi(ss);
            pthread_mutex_unlock(&mutex_lock);
        }
        if(buf[0] == 'c' && heart>0) {
            char *cc = strtok(buf, "=");
            cc = strtok(NULL, " ");
            pthread_mutex_lock(&mutex_lock);
            emotion = atoi(cc);
            pthread_mutex_unlock(&mutex_lock);
            int avg_bpm = heart;
            if(emotion == 0) {
                if(avg_bpm >= std_bpm) {
                    //printf("Angry\n");
                    msg = "Angry";
                } else if(avg_bpm < std_bpm) {
                    //printf("Disgust\n");
                    msg = "Disgust";
                } else {
                    //printf("Angry\n");
                    msg = "Angry";
                }
            } else if(emotion == 1) {
                //printf("Disgust\n");
                msg = "Disgust";
            } else if(emotion == 2) {
                //printf("Fear\n");
                msg = "Faer";
            } else if(emotion == 3) {
                if(sound > 0) {
                    //printf("Laughing\n");
                    msg = "Laughing";
                } else {
                    //printf("Happy\n");
                    msg = "Happy";
                }
            } else if(emotion == 4) {
                if(avg_bpm >= std_bpm+10) {
                    //printf("Fear\n");
                    msg = "Fear";
                } else {
                    //printf("Neutral\n");
                    msg = "Neutral";
                }
            } else if(emotion == 5) {
                if(sound > 0) {
                    //printf("Crying\n");
                    msg = "Crying";
                } else {
                    //printf("Sad\n");
                    msg = "Sad";
                }
            } else if(emotion ==6){
                if( avg_bpm >= std_bpm) {
                    //printf("Fear\n");
                    msg = "Fear";
                } else if(avg_bpm >= std_bpm && sound > 0) {
                    //printf("Screaming\n");
                    msg = "Screaming";
                } else {
                    // printf("Surprised\n");
                    msg = "Surprised";
                }
            } else{
                msg = "Neutral";
            }
            printf("표정: %d, 심박수: %d, 데시벨: %d => 결과: %s\n",emotion,avg_bpm,sound,msg);
            fwrite(msg, strlen(msg), 1, fp);
            reset(&heart, &sound,&emotion);
        }
        memset(buf, 0x00, 1024);
        fclose(fp);
        usleep(100000);
    }
    close(sockfd);

    return 0;
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock = -1;
    struct sockaddr_in serv_addr, clnt_addr;

    int listen_fd, client_fd;
    socklen_t addrlen;
    pthread_t thread_id;
    // 심박 증가 => 불안
    // class_labels=['Angry','Disgust', 'Fear', 'Happy','Neutral','Sad','Surprise']
    //

    // Enable GPIO pins
    if (-1 == GPIOExport(PIN) || -1 == GPIOExport(POUT))
        return (1);
    // Set GPIO directions
    if (-1 == GPIODirection(PIN, IN) || -1 == GPIODirection(POUT, OUT))
        return (2);
    if (-1 == GPIOWrite(POUT, 1))
        return (3);
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
    }

    // ============[ create socket ]
    listen_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(listen_fd, 5) == -1)
        error_handling("listen() error");

    while (1)
    {
        addrlen = sizeof(clnt_addr);
        client_fd = accept(listen_fd, (struct sockaddr *)&clnt_addr, &addrlen);
        if(client_fd == -1) {
            error_handling("accept error");
        } else {
            pthread_create(&thread_id, NULL, camera, (void *) &client_fd);
            pthread_detach(thread_id);
        }
        // str_len = read(clnt_sock, msg, sizeof(msg));
        // if (str_len == -1)
        //     error_handling("read() error");

        // str_len = read(clnt_sock2, msg2, sizeof(msg2));
        // if (str_len == -1)
        //     error_handling("read() error");

        // str_len = read(clnt_sock2, msg3, sizeof(msg3));
        // if (str_len == -1)
        //     error_handling("read() error");

        // // strcat(msg, " ");
        // // strcat(msg, msg2);
        // printf("%s %s %s\n", msg, msg2, msg3);
        // FILE *fp = fopen("lcd.txt", "w");
        // fwrite(msg, strlen(msg), 1, fp);
        // fclose(fp);

        // usleep(100000);
    }

    close(clnt_sock);
    close(serv_sock);
    // Disable GPIO pins
    if (-1 == GPIOUnexport(PIN) || -1 == GPIOUnexport(POUT))
        return (4);

    return (0);
}
