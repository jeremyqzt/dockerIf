#ifndef SOCKET_IF_H
#define SOCKET_IF_H

#include <stdio.h>

//Some defaults
#define DEFAULT_SOCK "/var/run/docker.sock"
#define DEFAULT_VERSION "v1.37"
#define DEFAULT_HOST "docker.sock"

//Some Constants
#define SOCK_COMMON_MAX_LEN 64
#define REQ_MAX_LEN 1024

#define HTTP_VER "HTTP/1.1"

//Status
enum
{
	DOCKER_IF_OK = 0,
	DOCKER_IF_NO_SOCKET,
	DOCKER_IF_NO_CONNECTION,
	DOCKER_IF_HTTP,
	C_DOCK_WTF
} dockerIfStatusCode; 


typedef struct
{
	char version[SOCK_COMMON_MAX_LEN];
	char sockPath[SOCK_COMMON_MAX_LEN];
	char sockHost[SOCK_COMMON_MAX_LEN];
	int dockerSock;
} SocketContext;

int dockerIfDestroy(SocketContext *ctx);
int dockerIfPost(SocketContext *ctx, char *request, char *postContent, int contentLen, char *outString, size_t outStringLen);
int dockerIfPostTar(SocketContext *ctx, char *request, char *path, char *outString, size_t outStringLen);
int dockerIfGet(SocketContext *ctx, char *request, char *outString, size_t outStringLen);
int dockerIfInit(SocketContext *ctx, char *version, char *path, char *host);

#endif

