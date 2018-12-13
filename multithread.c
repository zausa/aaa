#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/poll.h>

#define PORT    "8080" /* Port to listen on */
#define BACKLOG     10  /* Passed to listen() */
#define CLIENTNUMBER 2



void *handle(void *pnewsock)
{
    /* send(), recv(), close() */

    free(pnewsock);

    return NULL;
}

int main(void)
{
    int sock;
    pthread_t thread;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */
    {{{{
    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        return 1;
    }

    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        return 0;
    }

    freeaddrinfo(res);

    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        return 0;
    }


  }}}}

    /* Main loop */

    int psock[CLIENTNUMBER];
    char npsock[CLIENTNUMBER][100] ;

    int t = CLIENTNUMBER;
    while (t--)
    {
        socklen_t size = sizeof(struct sockaddr_in);
        struct sockaddr_in their_addr;
        int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
        if (newsock == -1)
        {
            perror("accept");
        }
        else
          {

            int *safesock = malloc(sizeof(int));
            if (safesock)
            {

                *safesock = newsock;
                if (pthread_create(&thread, NULL, handle, safesock) != 0)
                    fprintf(stderr, "Failed to create thread\n");

                psock[CLIENTNUMBER-t-1] = *safesock;
                char rmm[100] = {};
                recv(psock[CLIENTNUMBER-t-1],rmm,sizeof(rmm),0);
                fprintf(stderr,"[info] get connection from username : %s\n",rmm);

                char rmmm[] ={"[Server]U do it!!!\n"};
                send(psock[CLIENTNUMBER-t-1],rmmm,sizeof(rmmm),0);

                strcpy(npsock[CLIENTNUMBER-t-1] , rmm);
            }
            else
                perror("malloc");
            }
    }


    struct pollfd ufds[CLIENTNUMBER];
    for(int i=0;i<CLIENTNUMBER;i++)
        ufds[i].fd = psock[i],ufds[i].events = POLLIN;


    char receiveMessage[100] = {};


    while(1)
    {
      int rv;
      rv = poll(ufds,CLIENTNUMBER, 10000);
      if(rv == -1) continue;
      for(int i=0;i<CLIENTNUMBER ;i++)
          if(ufds[i].revents & POLLIN )
          {

              char toall[] = "ta\n";
              char tosingle[] = "ts\n";
              char wholive[] = "wl\n";
              char sendd [] = "se\n";
              char noth[] = "\n";


             recv(psock[i],receiveMessage,sizeof(receiveMessage),0);

             if(strcmp(receiveMessage,sendd)==0)
             {

                 char rna[100] ={}; //recv name
                 char fna[100] ={}; //file name


                 char to[100]="Who do u want to send to\n";
                 send(psock[i],to,sizeof(to),0);
                 recv(psock[i],rna,sizeof(rna),0);

                 //fprintf(stderr,"xx1xx %s xxxx\n",rna);



                 char too[100]="File name ?\n";
                 send(psock[i],too,sizeof(too),0);
                 recv(psock[i],fna,sizeof(fna),0);


                 //fprintf(stderr,"xx2xx %s xxxx\n",fna);


                 int rnum = atoi(rna);
                 //fprintf(stderr,"xx3xx %d xxxx\n",rnum);

                 recv(psock[rnum],receiveMessage,sizeof(receiveMessage),0);

                 //fprintf(stderr,"xx4xx %s xxxx\n",receiveMessage);

                 char pass[] = "Y\n";  char npass[] = "N\n";
                 if(strcmp(receiveMessage,pass)==0)
                  {
                  char fff[100] = "ab.txt";
                        //fprintf(stderr,"xx4xx %d %d xxxx\n",sizeof(fna) - sizeof(char));
                  //strncpy(fff,fna,( sizeof(fna) / sizeof(fna[0]) -1 ) );
                        // fprintf(stderr,"xx4xx %s xxxx\n",fff);
                  FILE *fp = fopen(fff, "r");
                        // fprintf(stderr,"xx4xx %s xxxx\n",fp);

                  char rr[]="FILE";
                  send(psock[rnum],rr,sizeof(rr),0);
                  send(psock[rnum],fff,sizeof(fff),0);

                  char buffer[100] = {0};
                  int nCount;
                  while( (nCount = fread(buffer, 1, 100, fp)) > 0 )
                      send(psock[rnum], buffer, nCount, 0);
                  shutdown(psock[rnum], SHUT_WR);  //file over send fin signal

                  close(fp);
                   }
                    else if(strcmp(receiveMessage,npass)==0)
                      {
                          char buffer[100] = "Not recvice !\n";
                          send(psock[rnum],buffer,sizeof(buffer),0);
                      }

             }
             else if(strcmp(receiveMessage,tosingle)==0)
                    {
                        char mm[100] = "";
                        recv(psock[i],receiveMessage,sizeof(receiveMessage),0);
                        int num = atoi(receiveMessage) ;
                        recv(psock[i],receiveMessage,sizeof(receiveMessage),0);
                        strcpy(mm,"[User message(private)]");strncat(mm,npsock[i],sizeof(npsock));strcat(mm,"> "); strcat(mm,receiveMessage);
                        send(psock[num],mm,sizeof(mm),0);
                    }
                    else if(strcmp(receiveMessage, wholive)==0)
                        {
                            char mm[100] = "";

                            for(int k=0;k<CLIENTNUMBER;k++)
                            {{strcpy(mm,"[Server Info (ONLINE)]");strcat(mm,npsock[k]);}
                                send(psock[i],mm,sizeof(mm),0);}
                        }else if(strcmp(receiveMessage, noth)==0)
                              {
                                  continue;
                              }
                              else
                              {
                                        if(strcmp(receiveMessage," ")==0 ||
                                           strcmp(receiveMessage,"") ==0 ||
                                           strcmp(receiveMessage,"\n") ==0 ||
                                           strcmp(receiveMessage," \n") ==0 ||
                                           strcmp(receiveMessage,"") ==0
                                            ) break;
                                        char mm[100] = "";
                                        strcpy(mm,"[User message]");strncat(mm,npsock[i],sizeof(npsock));strcat(mm,"> "); strcat(mm,receiveMessage);

                                        //fprintf(stderr,"xxxxx %s \n",mm);
                                        for(int j=0;j<CLIENTNUMBER;j++)
                                            if(j!=i)    send(psock[j],mm,sizeof(mm),0);
                              }


        }
    }





    close(sock);

    return 0;
}
