#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#define myport 8467
#define buf_size 100000
#define max_len 500
int main()
	 {
           int sockfd,nob,i;
           struct sockaddr_in serv_addr;
           char *request=0;
           char buf[max_len];
           int len,size;
           char message[100]="\0";
           char s_time;
           char BUFFER[buf_size];
           int head_flag=0,date_flag=0, default_flag=0;
           request=buf;
  
           printf("\n\t\t\t\t--------------\n\t\t\t\t USER MODULE\n\t\t\t\t--------------\n");
           if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)
              {
                 perror("socket");
                 exit(1);
              }
             bzero((char*)&serv_addr,sizeof(serv_addr));
             printf("PORT ADDRESS IS :%d\n",myport);
	         serv_addr.sin_family=AF_INET;
             serv_addr.sin_port=htons(myport);
             serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
             
            // memset(serv_addr.sin_zero,'\0',sizeof serv_addr.sin_zero);
             if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))==-1)
               {
                 perror("connect");
                 exit(1);
               }
            printf("\n\t\t ENTER THE DESIRED URL  \n");
            nob=recv(sockfd,message,30,0);
            printf(" %s : ",message);
            bzero(message,sizeof message);
            gets(message);
            send(sockfd,message,100,0);
            bzero(message,sizeof message);
            recv(sockfd,message,100,0);
            if(strcmp(message,"done")!=0)
                   {
                        printf("\n%s\n",message);
                          exit(1);
                   }
           // strcpy(request,"GET/HTTP/1.0\nHOST:");
            //request=(char *)strcat(request,message);
            //request=(char *)strcat(request,"\nConnection: close\nUser-agent:Mozilla Firefox 3.0.13");
           // printf("-- Send HTTP request to the PROXY SERVER :\n\n%s\n\n", request);
            printf("\n\n                              RESPONSE     \n\n ");
            int rc;
             do
               	{
		bzero(BUFFER,buf_size);
    	          rc = read(sockfd, BUFFER, buf_size);
		if( rc > 0)
		{ 
			printf("%s",BUFFER);
			size +=rc;
		}
		
	}while(rc>0);
          printf("\n   Total recieved response bytes: %d\n",size);
              
            
         printf("\nConnection closed\n");
          close(sockfd);
  }
