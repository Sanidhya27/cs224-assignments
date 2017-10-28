#include <iostream>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <unistd.h>
#include <map>
#include <vector>
using namespace std;
void dostuff(int client_socket)
{char buffer[256];int i=0;map <string,string> requests;
   char response[1000];
   while(1)
   {string word[3]={"","",""};
   bzero(buffer,256);
      bzero(response,1000);
      read( client_socket,buffer,255 );
      string s(buffer);
         
   
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
   {  if(requests.count(word[1])==0)
      {requests[word[1]]=word[2];
      cout<<"okay\n";
      //strcpy(response,"OK");
      write(client_socket,"OK",2);
   }
      else
         {cout<<"already exists\n";
      //strcpy(response,"Key already exists");
      write(client_socket,"Key already exists",25);
      
}  
   }
   if(word[0]=="get")
   {  //cout<<word[1]<<word[0]<<word[2];
      if(requests.count(word[1])==0)
      {cout<<"no such element\n";
      //strcpy(response,"Key not found");
      write(client_socket,"Key not found",20);
      }
      else
         {cout<<requests[word[1]]+"\n";
      char q[100];
      strcpy(q,requests[word[1]].c_str());
      write(client_socket,q,100);}
   }
   if(word[0]=="del")
   {  if(requests.count(word[1])>0)
      {requests.erase(word[1]);
      cout<<"deleted\n";
      //strcpy(response,"OK");
      write(client_socket,"OK",2);}
      else
      {cout<<"does not exists\n";
      //strcpy(response,"Key not found");
      write(client_socket,"Key not found",25);
}
   }
   if(word[0]=="bye")
   {  cout<<"good bye\n";
      //strcpy(response,"Goodbye");
      write(client_socket,"Goodbye",10);
      break;
   }
   /*if(word[0]!="bye"||word[0]!="del"||word[0]!="put"||word[0]!="get")
   {  cout<<"invalid request\n";
      //strcpy(response,"Goodbye");
      write(client_socket,"Invalid ",10);
      
   }*/
//write(client_socket,server_message,sizeof(server_message));


}

   //close(server_socket);
   //return 0;
}
int main()
{
   char server_message[256]="hell0 u connected successfully to server";
   int server_socket=socket(AF_INET,SOCK_STREAM,0);int pid;
   
   struct sockaddr_in server_add,client_addr;
   server_add.sin_family=AF_INET;
   server_add.sin_port=htons(9002);
   server_add.sin_addr.s_addr=INADDR_ANY;
   bind(server_socket, (struct sockaddr *) &server_add, sizeof(server_add));
   listen(server_socket,5);
   socklen_t clilen=sizeof(client_addr);
   
   int client_socket=accept(server_socket,(struct sockaddr *) &client_addr, &clilen); 
   //bzero(buffer,256);
  // read( client_socket,buffer,255 );
   //recv(server_socket,&client_request,sizeof(client_request),0);
   
   while(1)
   {  int client_socket=accept(server_socket,(struct sockaddr *) &client_addr, &clilen); 
         /*if (client_socket < 0) 
             error("ERROR on accept");*/
         pid = fork();
         /*if (pid < 0)
             error("ERROR on fork");*/
         if (pid == 0)  {
             close(server_socket);
             dostuff(client_socket);
             exit(0);
      

}}}