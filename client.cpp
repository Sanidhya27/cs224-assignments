#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h> 
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace std;
#define MAX_INPUT_SIZE 256

string getMessage(int sockfd,string input){
	/* Write to server */
	char inputbuf[MAX_INPUT_SIZE];
	int n = write(sockfd,input.c_str(),input.size());
	if (n < 0) 
	  {
	    fprintf(stderr, "ERROR writing to socket %d\n", sockfd);
	  }
	bzero(inputbuf,MAX_INPUT_SIZE);
	n = read(sockfd,inputbuf,MAX_INPUT_SIZE-1);
	if (n < 0) 
	  {
	    fprintf(stderr, "ERROR reading from socket %d\n",sockfd);
		return NULL;
	  }
	string output(inputbuf);
	return output;
}
int main(int argc, char *argv[])
{
    int sockfd1,sockfd2,sockfd3, portnum, n;
    struct sockaddr_in server_addr;

    char inputbuf[MAX_INPUT_SIZE];
    if (argc < 3) {
       fprintf(stderr,"usage %s <server-ip-addr> <server-port>\n", argv[0]);
       exit(0);
    }

    portnum = atoi(argv[2]);

    /* Create client socket */
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    sockfd3 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) 
      {
	fprintf(stderr, "ERROR opening socket\n");
	exit(1);
      }
    if (sockfd2 < 0) 
      {
	fprintf(stderr, "ERROR opening socket\n");
	exit(1);
      }
    if (sockfd3 < 0) 
      {
	fprintf(stderr, "ERROR opening socket\n");
	exit(1);
      }
//	printf("3 Sockets created \n");
    /* Fill in server address */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(!inet_aton(argv[1], &server_addr.sin_addr))
      {
	fprintf(stderr, "ERROR invalid server IP address\n");
	exit(1);
      }
    server_addr.sin_port = htons(portnum);

    /* Connect to server */
    if (connect(sockfd1,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
      {
	fprintf(stderr, "ERROR connecting using socket 1\n");
	exit(1);
      }
    /* Test Cases For Single Client*/	
	printf("Client 1 Connected to server\n");
	printf("------------------------------------------\n");
	printf("TestCase 1 \n");
 	printf("Client 1: put 12345 54321  Output: %s\n", getMessage(sockfd1,"put 12345 54321").c_str());
 	printf("Client 1: get 12345 Output: %s\n", getMessage(sockfd1,"get 12345").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 2 \n");
 	printf("Client 1: get 123 Output: %s\n", getMessage(sockfd1,"get 123").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 3 \n");
 	printf("Client 1: del 12345 Output: %s\n", getMessage(sockfd1,"del 12345").c_str());
 	printf("Client 1: get 12345 Output: %s\n", getMessage(sockfd1,"get 12345").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 4 \n");
 	printf("Client 1: del 123 Output: %s\n", getMessage(sockfd1,"del 123").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 5 \n");
 	printf("Client 1: put 12345 54321  Output: %s\n", getMessage(sockfd1,"put 12345 54321").c_str());
 	printf("Client 1: put 12345 54321  Output: %s\n", getMessage(sockfd1,"put 12345 54321").c_str());
 	printf("Client 1: put 12345 100  Output: %s\n", getMessage(sockfd1,"put 12345 100").c_str());


	printf("=============================================\n");

    /* Connect Two More Clients*/	
    if (connect(sockfd2,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
      {
	fprintf(stderr, "ERROR connecting Client 2\n");
	exit(1);
      }
    printf("Client 2 Connected to server\n");
    if (connect(sockfd3,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0) 
      {
	fprintf(stderr, "ERROR connecting Client 3\n");
	exit(1);
      }
    printf("Client 3 Connected to server\n");
	printf("=============================================\n");


    /* Test the cases*/
	printf("------------------------------------------\n");
	printf("TestCase 6 \n");
 	printf("Client 1: put 1 1  Output: %s\n", getMessage(sockfd1,"put 1 1").c_str());
 	printf("Client 2: put 2 2  Output: %s\n", getMessage(sockfd2,"put 2 2").c_str());
 	printf("Client 3: put 3 3  Output: %s\n", getMessage(sockfd3,"put 3 3").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 7 \n");
 	printf("Client 1: get 2  Output: %s\n", getMessage(sockfd1,"get 2 ").c_str());
 	printf("Client 2: get 1  Output: %s\n", getMessage(sockfd2,"get 1 ").c_str());
 	printf("Client 3: get 3  Output: %s\n", getMessage(sockfd3,"get 3 ").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 8 \n");
 	printf("Client 1: del 2  Output: %s\n", getMessage(sockfd1,"del 2").c_str());
 	printf("Client 2: get 2  Output: %s\n", getMessage(sockfd2,"get 2").c_str());
 	printf("Client 3: del 3  Output: %s\n", getMessage(sockfd3,"del 3").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 9 \n");
 	printf("Client 1: put 5 5  Output: %s\n", getMessage(sockfd1,"put 5 5").c_str());
 	printf("Client 2: put 5 10  Output: %s\n", getMessage(sockfd2,"put 5 10").c_str());
 	printf("Client 3: get 5  Output: %s\n", getMessage(sockfd3,"get 5").c_str());
	printf("------------------------------------------\n");
	printf("TestCase 10 \n");
 	printf("Client 1: del 5  Output: %s\n", getMessage(sockfd1,"del 5").c_str());
 	printf("Client 2: del 5  Output: %s\n", getMessage(sockfd2,"del 5").c_str());
 	printf("Client 3: get 5  Output: %s\n", getMessage(sockfd3,"get 5").c_str());
	printf("------------------------------------------\n");
	printf("Final Testcase (Optional) \n");
 	printf("Client 1: Bye  Output: %s\n", getMessage(sockfd1,"Bye").c_str());
 	printf("Client 2: Bye  Output: %s\n", getMessage(sockfd2,"Bye").c_str());
 	printf("Client 3: Bye  Output: %s\n", getMessage(sockfd3,"Bye").c_str());
	close(sockfd1);
	close(sockfd2);
	close(sockfd3);
    return 0;
}
