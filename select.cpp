#include <iostream>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> 
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>    //close 
#include <map>
#include <vector>
using namespace std;

map <string,string> requests;		// Global Map Variable for Storing

int main()
{	
	fd_set readfds;  
	int client_socket[30], max_clients = 30, activity, max_sd, i, valread, sd, new_socket; 
	char message[256]="hell0 u connected successfully to server";		//Display message
	int server_socket=socket(AF_INET,SOCK_STREAM,0);
	char buffer[256];
	
	struct sockaddr_in server_add,client_addr;
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(9002);      //defined on port 9002
	server_add.sin_addr.s_addr=INADDR_ANY;

	bind(server_socket, (struct sockaddr *) &server_add, sizeof(server_add));
	listen(server_socket,30);
	socklen_t clilen=sizeof(client_addr);

	for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;         //initialising all to zero
    }  
	
	char response[1000];

	while(1)
	{
	//clear the socket set 
        FD_ZERO(&readfds);  
    
        //add server socket to set 
        FD_SET(server_socket, &readfds);  
        max_sd = server_socket;  
            
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }  
            
        //If something happened on the server socket , 
        //then its an incoming connection 
        if (FD_ISSET(server_socket, &readfds))  
        {  
            if ((new_socket = accept(server_socket,(struct sockaddr *)&client_addr, (socklen_t*)&clilen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
            
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(server_add.sin_addr) , ntohs
                  (server_add.sin_port));  
          
            
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                        
                    break;  
                }  
            }  
        }  
            
        //else its some IO operation on some other socket
        for (int o = 0; o < max_clients; o++)  
        {  
            sd = client_socket[o];  
                
            if (FD_ISSET( sd , &readfds))  
            {  
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&client_addr , \
                        (socklen_t*)&clilen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(server_add.sin_addr) , ntohs(server_add.sin_port));  
                        
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[o] = 0;  
                }  
                    
                //Echo back the message that came in 
                else
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0'; 
                    string s(buffer); 
                    string word[3]={"","",""};
	int n=0;
	for(int i=0;i<s.length()-1;i++)
	{ 
		if(s[i]!=' ')
		{
			word[n]+=s[i];
		}
		else
			n++;
	}
	
	if(word[0]=="put")
	{	if(requests.count(word[1])==0)
		{requests[word[1]]=word[2];
		cout<<"okay\n";
		//strcpy(response,"OK");
		write(client_socket[o] ,"OK",2);
	}
		else
			{cout<<"already exists\n";
		//strcpy(response,"Key already exists");
		write(client_socket[o],"Key already exists",25);
		
}
	}
	if(word[0]=="get")
	{	cout<<word[1]<<word[0]<<word[2];
        //printf("get \n");
        //printf(word[1]);
		if(requests.count(word[1])==0)
		{cout<<"no such element\n";
		//strcpy(response,"Key not found");
		write(client_socket[o],"Key not found",20);
		}
		else
			{cout<<requests[word[1]]+"\n";
		char q[100];
		strcpy(q,requests[word[1]].c_str());
		write(client_socket[o],q,100);}
	}
	if(word[0]=="del")
	{	if(requests.count(word[1])>0)
		{requests.erase(word[1]);
		cout<<"deleted\n";
		//strcpy(response,"OK");
		write(client_socket[o],"OK",2);}
		else
		{cout<<"does not exists\n";
		//strcpy(response,"Key not found");
		write(client_socket[o],"Key not found",25);
}
	}
	if(word[0]=="bye")
	{	cout<<"good bye\n";
		//strcpy(response,"Goodbye");
		write(client_socket[o],"Goodbye",10);
		sd=0;
		client_socket[o]=0;
		//break;
	}
                    //send(sd , buffer , strlen(buffer) , 0 );  
                }  
            }  
        }  
     
		


}

	//close(server_socket);
	return 0;

} 

