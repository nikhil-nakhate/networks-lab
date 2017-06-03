// Client
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#define MAX 100
main(int argc,char *argv[])
{      
	int n,soc_id,i=0;
	struct sockaddr_in servaddr;	      
    char username[25],temp[100],recvname[25];
	char buf[MAX+1];       
	char address_buf[MAX],msg_buf[MAX];
	char *str_ptr,*buf_ptr,*str;

        int reqid=0;
        int msgid=0;

        if(argc!=3)
	 {
         fprintf(stderr,"Should Give<Server Port >< Username >\n");
	 exit(1);	
	 }                
         strcpy(username,argv[2]);
        // Socket Creation	
	if((soc_id=socket(AF_INET,SOCK_DGRAM,0))<0)
	 {
		printf("Cannot Create Socket");
		exit(1);
  	 }   
          
	printf("\n Socket Created\n");
	bzero((char*)&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(argv[1]));	 
	servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	if(servaddr.sin_addr.s_addr==INADDR_NONE){
	    printf("\nWrong server address\n");
	    exit(0);
	}
	int z;
while(1)
 {
   printf("\n%s:",argv[2]);
   scanf("%s",temp); 
   //Send Regitration msg

   if(!strcmp(temp,"register"))
    {  
	printf("\nhi\n");
	sprintf(buf,"Register :< %s >< localHost >< %s >\n",username,argv[1]);
	z=sendto(soc_id,buf,strlen(buf),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
    	if(z<0){
	    printf("\n Cannot send\n");
	    exit(0);
	}
           
	if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
	  perror("UDP Read Error");
	  exit(1);
	 }
	buf[n]='\0';
	printf("Server:%s",buf);	  
   }
  if(strstr(temp,"list"))
   {   
          sprintf(buf,"%s :Listuser < Req_id: %d >",username,reqid++);
	  sendto(soc_id,buf,strlen(buf),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
	  perror("UDP Read Error");
	  exit(1);
	 }
	buf[n]='\0';
	printf("Server:%s",buf);	
       if(!strstr(buf,"Registration"))
       {       
         while(1)
          {   	         
	   if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
		       {
	          perror("UDP Read Error");
           	  exit(1);
               }
           buf[n]='\0';
           if(!strcmp(buf,"#End"))
             break; 
	   printf("\n %s",buf);	      
         }   
       }
   }

if(!strcmp(temp,"quit"))
    { 
	 sprintf(buf,"<QUIT> %s",username);
        sendto(soc_id,buf,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
       if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
  	  perror("UDP Read Error");
	  exit(1);
	 }
	buf[n]='\0';
	printf("Server:%s",buf);        
        break;
 }

if(strstr(temp,"sendmsg"))
    {    
        printf("Message to :");  
        scanf("%s",&recvname);         
	sprintf(buf,"%s :Send Msg to %s Msgid: %d",username,recvname,msgid++);
        sendto(soc_id,buf,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
        if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
  	  perror("UDP Read Error");
	  exit(1);
	 }
	buf[n]='\0';
	printf("Server:%s",buf);        
        if(strstr(buf,"Receiver is Available"))
         {
        printf("\nMessage:\n");
	bzero(temp,sizeof(temp));
	int i=0;
	scanf("%c",&temp[i]);
	scanf("%c",&temp[i]);
        while(temp[i]!='\n'){
	      i++;
	      scanf("%c",&temp[i]);
	}
	printf("\nGoing to send : %s\n",temp);
	sprintf(buf,"%s",temp);
        sendto(soc_id,buf,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
        }                         

   }
if(strstr(temp,"receivemsg"))
    {
	 sprintf(buf,"%s :Receive Message",username);
        sendto(soc_id,buf,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
        if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
  	  perror("UDP Read Error");
	  exit(1);
	 }
	buf[n]='\0';
	printf("Server:\n%s",buf);        
     while(1)
       {
        if(!strstr(buf,"No_Messages"))
         {
              if((n=recvfrom(soc_id,buf,MAX,0,NULL,NULL))==-1)
	 {
  	  perror("UDP Read Error");
	  exit(1);
	 }
	   buf[n]='\0';
         if(!strstr(buf,"No_Messages"))
             printf("\n%s",buf);         
         }                    
         else
           break;
       }     

   }


}
}
