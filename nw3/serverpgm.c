// Server
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define MAX 100

struct clientmsg
{
char username[25];
char recvname[25];
char message[20][1010];
char msgid[20][20];
int nomsg;
int msgflag;
int quit;
};

struct clientmsg cmsg[20];


char message[20][1010];

int listcount=0;
int usercount=1; 
struct sockaddr_in servaddr;
socklen_t len=sizeof(servaddr);
int soc_id;
char port[10];	



main(int argc,char *argv[])
{       
     
        char msg[MAX+1]; 
        char *str_ptr,*buf_ptr,*str;    
        int i,n,j,t;  
        char username[25];

        char temp[30],temp1[30],op; 
     

       strcpy(port,argv[1]); 
  

      //Server Socket Creation & Binding

        if((soc_id=socket(AF_INET,SOCK_DGRAM,0))<0)
	 {
		printf("Cannot Create Socket");
		exit(1);
  	 }
	printf("\n Server Socket Created\n");
	bzero((char*)&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(atoi(port));
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
      if(bind(soc_id,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	 {
		printf("Bind Failed");
		exit(1);
  	 }

 for(i=0;i<20;i++)
     cmsg[i].nomsg=0;

   while(1)
    {
      printf("\nWaiting for msg\n");
      n=recvfrom(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,&len);       
 	if(n==-1)
	 {
	  perror("Message not received");
	  exit(1);	 	
	 }
    printf("\nMsg received\n");
    msg[n]='\0';  
    printf("%s\n",msg);
    if(strstr(msg,"Register :<"))
      {
        sscanf(msg,"%*s%*s%s",username);       
        if(strlen(username)<20)      
           {
                for(i=1;i<usercount;i++)
       	         {
                 if(!strcmp(cmsg[i].username,username))
                    break;
                 } 
                 if(i==usercount)
                    {         
		     printf("\nReg ok\n");
                     sprintf(msg,"Registration Ok\n");
                     strcpy(cmsg[usercount++].username,username); 
                    }
                else
                  {
                    if(cmsg[i].quit==1)             
                    cmsg[i].quit=0;
                    else          
                    sprintf(msg,"Registration  error 002\n");         
                  }   

        sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  
          
           } 
       }
else if(strstr(msg,":Listuser"))
      {	
        sscanf(msg,"%s",temp1);         
        for(i=1;i<usercount;i++)
       	{
         if(!strcmp(cmsg[i].username,temp1))
           break;
        } 
       if((i==usercount)||cmsg[i].quit==1)
           {
           sprintf(msg,"Registration Required");
           sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
           }  
       else	
         {
		listcount=0;
        for(i=1;i<usercount;i++)
              if(cmsg[i].quit!=1)
                  listcount++;          
        sscanf(msg,"%*s%*s%*s%*s%s",temp);
       	sprintf(msg,"UserList  Reqid:%s  No of Users:%d ",temp,listcount);
       	sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));        
       	for(i=1;i<usercount;i++)
       	 { if(cmsg[i].quit!=1) 
           { sprintf(msg,"User %d : %s",i,cmsg[i].username);
            sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));        
            }
         }
         sprintf(msg,"#End");
         sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
                  
        }      
          
      }

    else if(strstr(msg,":Send Msg to"))
      {      
        sscanf(msg,"%s",temp1);         
        for(i=1;i<usercount;i++)
       	{
         if(!strcmp(cmsg[i].username,temp1))
           break;
        } 
       if(i==usercount)
           {
           sprintf(msg,"Registration Required");
           sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
           }  
        else	   
          {                                    	     
               
               sscanf(msg,"%*s%*s%*s%*s%s",temp);         
               for(i=1;i<usercount;i++)
	       	{
	       	 if(!strcmp(cmsg[i].username,temp))
	           break;
	        } 
	        if(i==usercount)
	           {
        	     sprintf(msg,"The Receiver is not Available");
        	   sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
        	   }  
        	else	
        	  {   
                   cmsg[i].msgflag=1;                        
      		       cmsg[i].nomsg++;	
                       strcpy(cmsg[i].recvname,temp1);
                       sscanf(msg,"%*s%*s%*s%*s%*s%*s%s",cmsg[i].msgid[cmsg[i].nomsg]);
      		  
     sprintf(msg,"Receiver is Available");


sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                                   
    n=recvfrom(soc_id,cmsg[i].message[cmsg[i].nomsg],MAX,0,(struct sockaddr*)&servaddr,&len);       	
 	if(n==-1)
	 {
	  perror("Message not received");
	  exit(1);	 	
	 }
    msg[n]='\0';  
    printf("%s\n",cmsg[i].message[cmsg[i].nomsg]);
            }  
         }
        }

 else if(strstr(msg,"<QUIT>"))
     {
        sscanf(msg,"%*s%s",temp1);         
        for(i=1;i<usercount;i++)
       	{
         if(!strcmp(cmsg[i].username,temp1))
           break;
        } 
       if(i==usercount)
           {
           sprintf(msg,"Not Regiserd");
           sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
           }  
        else	
           {
                     
	sprintf(msg,"Server Connection is Closing........\n");	
 	sendto(soc_id,msg,sizeof(msg),0,(struct sockaddr*)&servaddr,sizeof(servaddr));     
         cmsg[i].quit=1;
        listcount--;
            }
      }
else if(strstr(msg,":Receive Message"))
      {
	        sscanf(msg,"%s",temp1);         
	        for(i=1;i<usercount;i++)
	       	{
	         if(!strcmp(cmsg[i].username,temp1))
	           break;
	        } 
	        if(i==usercount)
	           {
	             sprintf(msg,"Registration Required");
	           sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));                     
	           }  
             else
              {
	         
           	     if(cmsg[i].msgflag==1)  
        		{
     		       for(j=cmsg[i].nomsg;j>=1;j--)
         		 	{         	       	          
           	             sprintf(msg," Message from < %s >  MsgId < %s >",cmsg[i].recvname,cmsg[i].msgid[j]);
		                sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));           
                              sprintf(msg,"%s",cmsg[i].message[j]);
		                sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));           
                              	              
		                }    

                              sprintf(msg,"No_Messages");
		                sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));        
  
               
           	        } 
           	     else
               		{
                     	  sprintf(msg,"No_Messages");
			  sendto(soc_id,msg,MAX,0,(struct sockaddr*)&servaddr,sizeof(servaddr));  
	               }             

              }
      }

}
}	
