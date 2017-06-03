#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netdb.h>
#define myport 8467
#define buf_size 10000
#define max_len 100
#define label exit
int nsd;
int stat=0;
struct site_status {
  int num;
  char name[100];
}s[10];

void initialise(){
int i=0;
while(i<10){
strcpy(s[i].name,"NULL");
s[i].num=0;
i++;
}
}

int search(char content[]) {
int i=0;
stat=0;
while(strcmp(s[i].name,"NULL")!=0){
if(!strcmp(s[i].name,content)){
s[i].num++;
stat=s[i].num;
return 1;
}
i++;
}
return 0;
}
void add(char content[]){
int i=0;
if(search(content)){printf("Search Found");}
else {
printf("ENTERED\n\n");
while(strcmp(s[i].name,"NULL")!=0){i++;}
printf("%d\n\n",i);
strcpy(s[i].name,content);
s[i].num+=1;
stat=1;
    }
}
parse_URL(char *url, char *hostname, int *port, char *identifier)
{
    char protocol[max_len], scratch[max_len], *ptr=0, *nptr=0;
    
    strcpy(scratch, url);
    ptr = (char *)strchr(scratch, ':');
    if (!ptr)
    {
	fprintf(stderr, "Wrong url: no protocol specified\n");
        send(nsd,"Wrong url: no protocol specified",sizeof("Wrong url: no protocol specified"),0);
	 exit(1);
        
    }
    strcpy(ptr, "\0");
    strcpy(protocol, scratch);
    if (strcmp(protocol, "http"))
    {
	fprintf(stderr, "Wrong protocol: %s\n", protocol);
        send(nsd,"Wrong protocol",sizeof("Wrong protocol"),0);
	 exit(1);
    }

    strcpy(scratch, url);
    ptr = (char *)strstr(scratch, "//");
    if (!ptr)
    {
	fprintf(stderr, "Wrong url: no server specified\n");
         send(nsd,"Wrong url: no server specified",sizeof("Wrong url: no server specified"),0);
	 exit(1);
    }
    ptr += 2;

    strcpy(hostname, ptr);
    nptr = (char *)strchr(ptr, ':');
    if (!nptr)
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
         send(nsd,"Wrong url: no file specified",sizeof("Wrong url: no file specified"),0);
	 exit(1);
    }
    
    strcpy(identifier, nptr);
}


int main()
{
	int sd,i,rsd,rc;
	int bi;
         char *url=0;
         char hostname[max_len],identifier[max_len];
	char content[100]="\0";
         int port;
        char buf[100],x[200];
        char *request=0;
          request=x;
        url=buf;
		 // char ch[30]="www.skype.com";
	struct sockaddr_in serv_addr,cl_addr,rserv_addr;
initialise();
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
		  send(nsd,"URL",strlen("URL"),0);
		  i=recv(nsd,content,40,0);
add(content);
                 strcpy(url,"http://");
                 url=(char *)strcat(url,content);
                 url=(char *)strcat(url,":80/");
                 printf("%s",url);
                 parse_URL(url,hostname,&port,identifier);
                 
                  
                 printf("\nUSER IS REQUESTING FOR FOLLOWING: \n\t--Hostname =  %s    Port = %d   Identifier =  %s\n",hostname,port,identifier);
                 h=gethostbyname(hostname);
                 if(h==NULL)
                    {
                      printf("Unknown host :%s",hostname);
                         send(nsd,"unknowmn host",sizeof("Unknown host"),0);
                       goto exit;
                    }
					
                   send(nsd,"done",sizeof("done"),0);
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
                     strcpy(request,"GET / HTTP/1.0\r\nHOST:");
                     //strcpy(request,"GET");
                   //request = (char *)strcat( request, identifier);
	          //request = (char *)strcat( request, " HTTP/1.1\r\nHOST: ");
	          request = (char *)strcat( request, hostname);
	          request = (char *)strcat( request, "\r\n");
                    request=(char *)strcat(request,"Connection: Close\r\nUser-agent :Mozilla/3.0");
                      request = (char *)strcat( request, "\r\n");
                      request = (char *)strcat( request, "\r\n");
                   printf("Sending the request to the remote server :\n %s",request);
                   printf("\n\nNumber of times the website is visited : %d",stat);
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
