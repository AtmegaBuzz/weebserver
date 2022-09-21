#ifndef _SERVER_H
#define _SERVER_H

void error(char* log,int status_code);

struct Server{
    char* ip;
    int port;
    int domain;
    int type;
    int protocol;
    int MAX_CONNEXTIONS;
    struct sockaddr_in address;

    int socket;
};

struct Server* serverCreate(
    char* ip,
    int port,
    int max_connections,
    int domain,
    int type,
    int protocol
);

int runserver(struct Server* server);


char* http_response = "HTTP/1.1 200 OK \n
    Date: Mon, 27 Jul 2009 12:28:53 GMT \n
    Server: Apache/2.2.14 (Win32)\n
    Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n
    Content-Length: 88\n
    Content-Type: text/html\n
    <html><body><h1>Hello, World!</h1></body></html>";


#endif