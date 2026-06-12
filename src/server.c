#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8081
#define BUFFER_SIZE 1024
void handle_request(int client_socket)
{
	char buffer[BUFFER_SIZE];
	ssize_t n = recv(client_socket,buffer,sizeof(buffer)-1,0);
	if (n<=0){close(client_socket);return;}
	buffer[n]='\0';
	char method[8],path[1024];
	if(sscanf(buffer,"%7s %1023s",method,path)!=2){close(client_socket);return;}
	if(strcmp(method,"GET")==0 && (strcmp(path,"/")==0 || strcmp(path,"/index.html")==0))
	{
		FILE *f=fopen("index.html","rb");
	if(!f){
		const char *resp404="HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
		send(client_socket,resp404,strlen(resp404),0);
	} else {
		fseek(f,0,SEEK_END);
		long size=ftell(f);
		fseek(f,0,SEEK_SET);
		char hdr[256];
		int hlen=snprintf(hdr,sizeof(hdr),
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html\r\n"
			"Content-Length: %ld\r\n"
			"Connection: close\r\n"
			"\r\n",size);
		send(client_socket,hdr,hlen,0);
		char filebuf[4096];
		size_t r;
	while((r=fread(filebuf,1,sizeof(filebuf),f))>0)
	{
		send(client_socket,filebuf,r,0);
	}
	fclose(f);
	}
	}else{
		const char *resp404 ="HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
		send(client_socket,resp404,strlen(resp404),0);
	}
	close(client_socket);
}
int main()
{
	// Socket Initialization
	int server_socket,client_socket;
	struct sockaddr_in server_addr,client_addr;
	socklen_t addr_len=sizeof(client_addr);
	server_socket=socket(AF_INET,SOCK_STREAM,0);
	if(server_socket==-1)
	{
		perror("Socket Failed");
		exit(EXIT_FAILURE);
	}
	// Server IP Initialization
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(PORT);
	
	if(bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
	{
		perror("Bind Failed");
		exit(EXIT_FAILURE);
	}
	
	if(listen(server_socket,10)<0)
	{
		perror("Listen Failed");
		exit(EXIT_FAILURE);
	}
	printf("Listening on port %d\n",PORT);
	// Waiting for connection
	while(1)
	{
		//
		client_socket=accept(server_socket,(struct sockaddr *)&client_addr,&addr_len);
		if(client_socket<0)
		{
			perror("Accept Failed");
			continue;
		}
		handle_request(client_socket);
	}
	close(server_socket);
	return 0;
}
