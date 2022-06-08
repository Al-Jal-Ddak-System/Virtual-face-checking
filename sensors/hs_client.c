#include "gpio.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include<linux/spi/spidev.h>
#include<linux/types.h>
#include <sys/ioctl.h>
#define POUT 17 


// 이 문장은 서버 통신 -클라이언트 부분입니다.
int sock;
struct sockaddr_in serv_addr;
char msg2[4];
char msg[4];
char on[2] = "1";
int str_len;

// 이 문장은 MCP3008 - 아날로그 데이터 인식 부분입니다. 
#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])
static const char *DEVICE = "/dev/spidev0.0";
static uint8_t MODE = SPI_MODE_0;
static uint8_t BITS = 8;
static uint32_t CLOCK = 1000000;
static uint16_t DELAY = 5;

uint8_t control_bits_differential(uint8_t channel){
  return (channel & 7) << 4;
}

// (SGL/DIF = 1, D2=D1=D0=0)
uint8_t control_bits(uint8_t channel){
  return 0x8 | control_bits_differential(channel);
}

static int prepare(int fd){

  if (ioctl(fd, SPI_IOC_WR_MODE, &MODE) == -1){
    perror("Can't set MODE");
    return -1;
  }
  if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &BITS) == -1){
    perror("Can't set number of BITS");
    return -1;
  }
  if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &MODE) == -1){
    perror("Can't set write CLOCK");
    return -1;
  }
  if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &MODE) == -1){
    perror("Can't set read CLOCK");
    return -1;
  }

  return 0;
}

// read spi
int readadc(int fd, uint8_t channel){
  uint8_t tx[] = {1, control_bits(channel), 0};
  uint8_t rx[3];

  struct spi_ioc_transfer tr = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = ARRAY_SIZE(tx),
    .delay_usecs = DELAY,
    .speed_hz = CLOCK,
    .bits_per_word = BITS,
  };

  if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) == 1){
    perror("IO Error");
    abort();
  }

  return ((rx[1] << 8) & 0x300) | (rx[2] & 0xFF);
}

void *sound_thd()
{
    int sound = 0;
    while(1)
    {
        sound = GPIORead(POUT);   
        snprintf(msg, 4, "s=%d", sound);
        write(sock, msg, sizeof(msg));     
        usleep(100 * 1000);
    }
}
void *heart_thd()
{
    int fd;
    fd = open(DEVICE, O_RDWR);
    
    
    if (fd <= 0){
        printf("Device %s not found\n", DEVICE);
    }
    int heart = 0;
    while(1)
    {
        char buf[10];
        char buf2[10];
        int BPM;
        FILE *fp = fopen("heart.txt","w");
        FILE *fq = fopen("heart_BPM.txt","r");
        heart = readadc(fd,0); //아날로그 데이터 읽기
        /*숫자를 문자열로 변환하여 BUF에 쓰기
         이를 통해서 heart.txt에 heart 값이 write된다.*/
        sprintf(buf,"%d",heart);
        fwrite(buf,strlen(buf),1,fp);
        //buf2에는 BFM 값이 써진다. 이를 읽어온다. 
        fgets(buf2, sizeof(buf2), fq);
        BPM = atoi(buf2);

        snprintf(msg2, 5, "h=%d", BPM);  //소켓 통신
        write(sock, msg2, sizeof(msg2));
        fclose(fp);
        fclose(fq);
        usleep(100 * 1000);
    }
    
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[])
{

    pthread_t p_ultra;
    pthread_t p_led;
    int thr_id;
    int thr_id2;
    int status;
    char pM[] = "thread_m";
    char pN[] = "thread_n";
    
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
  
    // Enable GPIO pins
    if (-1 == GPIOExport(POUT))
        return (1);
    // Set GPIO directions
    if (-1 == GPIODirection(POUT, IN))
        return (2);

    thr_id2 = pthread_create(&p_ultra, NULL, sound_thd, NULL);
    if (thr_id2 < 0){
        perror("thread create error : ");
        exit(0);
    }
    usleep(10000);
    
    thr_id = pthread_create(&p_led, NULL, heart_thd, NULL);
    if (thr_id < 0){
        perror("thread create error : ");
        exit(0);
    }

    usleep(100000 * 60);

    pthread_join(p_ultra, (void **)&status);
    pthread_join(p_led, (void **)&status);

    close(sock);

    // Disable GPIO pins
    if (-1 == GPIOUnexport(POUT))
        return (4);

    return (0);
}