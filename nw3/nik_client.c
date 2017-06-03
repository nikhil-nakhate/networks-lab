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
void main(int argc,char *argv[])                                            //port and username is passed at execution time
{      
	int n,soc_id,i=0;
	struct sockaddr_in servaddr;	      
        char username[25],command[100],recvname[25];
	char buf[MAX+1];       
	char address_buf[MAX],msg_buf[MAX];
	char *str_ptr,*buf_ptr,*str;

        int reqid=0;
        int msgid=0;

        if(argc!=3)                                                         // the three arguments would be the file name port no and username
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
   scanf("%s",command); 
   //Send Regitration msg

   if(!strcmp(command,"register"))
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
	buf[n]='\0';							 // end of buffer
	printf("Server:%s",buf);	  
   }
  if(strstr(command,"list"))                                             //lists  the users currently registered to the server 
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

if(!strcmp(command,"quit"))
    { 
	 sprintf(buf,"<QUIT> %s",username);                                        //command to exit
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

if(strstr(command,"sendmsg"))                                               //command to send a message to a client registered with teh server
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
	bzero(command,sizeof(command));
	int i=0;
	scanf("%c",&command[i]);
	scanf("%c",&command[i]);
        while(command[i]!='\n'){
	      i++;
	      scanf("%c",&command[i]);
	}
	printf("\nGoing to send : %s\n",command);
	sprintf(buf,"%s",command);
        sendto(soc_id,buf,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
        }                         

   }
if(strstr(command,"receivemsg"))                                          //command to receive a message sent by a another client
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
	   buf[n]='\0';                                             // end of buffer
         if(!strstr(buf,"No_Messages"))
             printf("\n%s",buf);         
         }                    
         else
           break;
       }     

   }


}
}
