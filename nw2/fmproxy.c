#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#define myport 9998
#define buf_size 1000000
#define max_len 100
parse_URL(char *url, char *hostname, int *port, char *identifier)
{
    char protocol[max_len], scratch[max_len], *ptr=0, *nptr=0;
    
    strcpy(scratch, url);
    ptr = (char *)strchr(scratch, ':');
    if (!ptr)
    {
	fprintf(stderr, "Wrong url: no protocol specified\n");
	exit(1);
    }
    strcpy(ptr, "\0");
    strcpy(protocol, scratch);
    if (strcmp(protocol, "GET http"))
    {
	fprintf(stderr, "Wrong protocol: %s\n", protocol);
	//exit(1);
    }

    strcpy(scratch, url);
    ptr = (char *)strstr(scratch, "//");
    if (!ptr)
    {
	fprintf(stderr, "Wrong url: no server specified\n");
	exit(1);
    }
    ptr += 2;

    strcpy(hostname, ptr);
    nptr = (char *)strchr(ptr, ':');
    if (nptr==0)
    {
	*port = 80; /* use the default HTTP port number */
	nptr = (char *)strchr(hostname, '/');
    }
    else
    {	
	sscanf(nptr, ":%d", port);
	nptr = (char *)strchr(hostname, ':');
    }

    if (nptr)
      *nptr = '\0';

    nptr = (char *)strchr(ptr, '/');
    
	if (!nptr)
    {
	fprintf(stderr, "Wrong url: no file specified\n");
	exit(1);
    }
    
    strcpy(identifier, nptr);
}


int main()
{
	int sd,nsd,i,rsd,rc;
	int bi;
         char *url=0;
         char hostname[max_len],identifier[max_len];
	char content[100]="\0";
         int port;
        char buf[100],x[200];
        char *request=0;
          request=x;
        url=buf;
	struct sockaddr_in serv_addr,cl_addr,rserv_addr;
	if((sd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))==-1)
	{
		perror("socket");
		exit(1);
	}
         char BUFFER[buf_size];
       
         struct hostent *h;
	bzero((char*)&serv_addr,sizeof(serv_addr));
	printf("\nPORT ADDRESS IS :%d\n",myport);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(myport);
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);


		bi=bind(sd,((struct sockaddr *)&serv_addr),sizeof(serv_addr));
	if(bi==-1)
	{
		perror("bind");
		exit(1);
	}
	i=sizeof(cl_addr);
	socklen_t t;
	t=sizeof(struct sockaddr_in);
	listen(sd,i);
	printf("\n\t\t\t\t--------------\n\t\t\t\t PROXY MODULE\n\t\t\t\t--------------\n");
        while(1)
        {         printf("Waiting for the connection\n\n");
		   nsd=accept(sd,(struct sockaddr *)&cl_addr,&t);
		  if(nsd==-1)
		  {	
			  printf("\nCheck description parameters\n");
			  goto exit;
		  }
		  printf("\n\t\tConnection Established(client accepted)From :%s\n",inet_ntoa(cl_addr.sin_addr));      
		 
		  i=recv(nsd,content,40,0);
                 strcpy(url,"");
                 url=(char *)strcat(url,content);
               // printf("%s",url);
                 parse_URL(url,hostname,&port,identifier);
                  
                 printf("\nUSER IS REQUESTING FOR FOLLOWING: \n\t--Hostname =  %s    Port = %d   Identifier =  %s\n",hostname,port,identifier);
                 h=gethostbyname(hostname);
                 if(h==NULL)
                    {
                      printf("Unknown host :%s",hostname);
                       goto exit;
                    }
                   
                   rserv_addr.sin_family=h->h_addrtype;
                   memcpy((char *) &rserv_addr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
                   rserv_addr.sin_port=htons(port);
                  
                    rsd = socket(AF_INET, SOCK_STREAM, 0);
	if(rsd<0) 
	{
		perror("cannot open socket ");
		goto exit;
	}

	
	printf("Bind port number...  ");
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(0);
  
	rc = bind(rsd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
            if(rc<0) 
	       {
		printf(" cannot bind port TCP %u\n",port);//(LOCAL_SERVER_PORT);
		perror("error ");
		exit(1);
	       }
         printf("Connect to server...\n");
	rc = connect(rsd, (struct sockaddr *) &rserv_addr, sizeof(rserv_addr));
	if(rc<0) 
	{
		perror("cannot connect ");
		goto exit;
	}       
                    strcpy(request,"GET ");
                    strcpy(request,"GET / HTTP/1.0\r\nHOST:");
                    //request=(char *)strcat(request,":");
                  //request=(char *)strcat(request,identifier);
				   //request=(char *)strcat(request,":");
                    request=(char *)strcat(request,hostname);
                     request = (char *)strcat( request, "\r\n");
                     request=(char *)strcat(request,"Connection: Close\r\n");
                      
                      request = (char *)strcat( request, "\r\n");
                      request = (char *)strcat( request, "\r\n");
                   printf("Sending the request to the remote server :\n %s",request);
                    if(write(rsd,request,strlen(request))==-1)
                       {
                          perror("can't send the request");
                        }
                     bzero(BUFFER,buf_size);
                   
                     printf("      Sending the desired response.....\n ");
                    do
   	               {
		         bzero(BUFFER,buf_size);    //  init line 
    	                 rc = read(rsd, BUFFER, buf_size);
		         if( rc > 0)
		             { 
                                 printf("%s",BUFFER);
			        write(nsd,BUFFER,buf_size);
                             }
                    	}while(rc>0);
                   printf("\nResponse is sent");
         exit:        close(rsd);
		  bzero(content,sizeof content);
                   bzero(BUFFER,buf_size); 
		  close(nsd);  
                  printf("\n------------------------------ End of a session ---------------------------------------\n\n");
                    
                    
        }
	return(0);
}
