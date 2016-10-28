#include<stdio.h>
#include<stdlib.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    

int main(int argc , char *argv[])
{
	int socket_desc,new_socket,c;
	int answer[8];
	int counta,countb,countA,countB;

	struct sockaddr_in server,client;
	char *message;
	char reply[8],compare[8],message2[50];

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8787 );
//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
//Listen
	listen(socket_desc , 3);
//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");

		countA=0; countB=0;
		srand( time(NULL) ); 
		for(counta=0;counta<4;counta++)
		{
  			answer[counta]=rand()%10; 
  			for(countb=0;countb<counta;countb++) 
  			{ 
     				if(answer[counta]==answer[countb]) 
     				{	
     					counta--; 
     				} 
  			}
		} 
	printf("Answer:%d%d%d%d\n",answer[0],answer[1],answer[2],answer[3]);
//Reply to the client
	while(1)
	{
		bzero(reply,8);
		read(new_socket,reply,8);
		compare[0]=(reply[0]-'0');
		compare[1]=(reply[1]-'0');
		compare[2]=(reply[2]-'0');
		compare[3]=(reply[3]-'0');
	
		for(counta=0;counta<4;counta++)
		{
			if(answer[counta]==compare[counta]) 
			{
				countA+=1;
			}
		}
	
		for(countb=0;countb<=3;countb++)
		{
			for(counta=0;counta<=3;counta++)
			{
				if(compare[counta]==answer[countb] && counta!=countb)
				{
					countB++;
				}
			}
		}

		if(countA==4)
		{
			message="Congratulations!!";
			write(new_socket , message, strlen(message)+1);
			return 1;
		}
		else
		{
			sprintf(message2,"%dA%dB",countA,countB);
			write(new_socket , message2, strlen(message2)+1);
			countA=0;countB=0;
		}
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
		return 0;
	}
