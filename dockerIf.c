#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "include/dockerIf.h"



/**
 *  @brief               Initializes the context for this library, this
 *                       function must be called and return DOCKER_IF_OK
 *                       before this library is used.
 *
 *  @param ctx           Pointer to the context to initialize
 *  @param version       Docker API version string
 *  @param path          Docker socket path, defaults to /var/run/docker.sock
 *  @param host          Docker host name, and defaults to docker.sock
 *
 *  @returns             Status about the initialization (dockerIfStatusCode)
 **/
int dockerIfInit(SocketContext *ctx, char *version, char *path, char *host)
{

	struct sockaddr_un server;
	int returnCode = -1;

	snprintf(ctx->version,  sizeof(ctx->version),
		"%s", version == NULL ? DEFAULT_VERSION: version);

	snprintf(ctx->sockPath, sizeof(ctx->sockPath),
		"%s", path == NULL ? DEFAULT_SOCK: path);

	snprintf(ctx->sockHost, sizeof(ctx->sockHost),
		"%s", host == NULL ? DEFAULT_HOST: host);

	//As i've learned... only TCP works...
	ctx->dockerSock = socket(AF_UNIX, SOCK_STREAM, 0);

	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, ctx->sockPath);

	if (0 >= ctx->dockerSock)
	{
		return -DOCKER_IF_NO_SOCKET;
	}


	int flag = 1;
	if (0 != setsockopt(ctx->dockerSock, IPPROTO_TCP,
			TCP_NODELAY, (char *) &flag, sizeof(int)))
			{
				return -DOCKER_IF_NO_SOCKET;
			}

	returnCode = connect(ctx->dockerSock,
						(struct sockaddr *) &server,
						sizeof(struct sockaddr_un));

	if (0 == returnCode)
	{
		return DOCKER_IF_OK;
	}

	return -DOCKER_IF_NO_CONNECTION;
}



/**
 *  @brief               Performs a HTTP GET request on the socket
 *
 *  @param ctx           Pointer to the context of the action
 *  @param request       Path of the GET request on the server
 *  @param outString     Reply of the socket, HTTP Code and Output
 *  @param outStringLen  Size of the outString
 *
 *  @return              Number of Bytes Received on success
 *                       DOCKER_IF_HTTP on failure
 *                       outString contains HTTP error code
 **/
int dockerIfGet(SocketContext *ctx, char *request,
				char *outString, size_t outStringLen)
{
	char sendLine[REQ_MAX_LEN] = {0};
	int received = -1;

	snprintf(sendLine,
		sizeof(sendLine),
		"GET /%s/%s HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Host: %s\r\n"
		"\r\n",
		ctx->version,
		request,
		ctx->sockHost);

	received = send(ctx->dockerSock, sendLine, sizeof(sendLine), 0);

	if (0 < received)
	{

		received = recv(ctx->dockerSock, outString, outStringLen, 0);

		if (0 < received)
		{
			return received;
		}
	}

	return -DOCKER_IF_HTTP;
}



/**
 *  @brief               Sends a HTTP POST request to the socket
 *
 *  @param ctx           Pointer to the context of the action
 *  @param request       Path of the POST request on the server
 *  @param postContent   Content associated with the post request
 *  @param contentLen    Length of the POST request content
 *  @param outString     Reply of the socket, HTTP Code and Output
 *  @param outStringLen  Size of the outString
 *
 *  @return              Number of Bytes Received on success
 *                       DOCKER_IF_HTTP on failure
 *                       outString contains HTTP error code
 **/
int dockerIfPost(SocketContext *ctx, char *request, char *postContent,
				int contentLen, char *outString, size_t outStringLen)
{
	char sendLine[REQ_MAX_LEN] = {0};
	int received = -1;

	snprintf(sendLine,
		sizeof(sendLine),
		"POST /%s/%s HTTP/1.1\r\n"
		"Content-Type: application/json\r\n"
		"Host: %s\r\n"
		"Content-Length: %d\r\n"
		"\r\n"
		"%s\r\n",
		ctx->version,
		request,
		ctx->sockHost,
		contentLen + 2,
		postContent);

	received = send(ctx->dockerSock, sendLine, sizeof(sendLine), 0);

	if (0 < received)
	{
		received = recv(ctx->dockerSock, outString, outStringLen, 0);

		if (0 < received)
		{
			return received;
		}
	}

	return -DOCKER_IF_HTTP;
}



/**
 *  @brief               Sends a HTTP POST request to the socket
 *
 *  @param ctx           Pointer to the context of the action
 *  @param request       Path of the POST request on the server
 *  @param postContent   Content associated with the post request
 *  @param contentLen    Length of the POST request content
 *  @param outString     Reply of the socket, HTTP Code and Output
 *  @param outStringLen  Size of the outString
 *
 *  @return              Number of Bytes Received on success
 *                       DOCKER_IF_HTTP on failure
 *                       outString contains HTTP error code
 **/
int dockerIfPostTar(SocketContext *ctx, char *request,
				char *path, char *outString, size_t outStringLen)
{
	char sendLine[REQ_MAX_LEN] = {0};
	int received = -1;
	int len = -1;
	FILE *filePointer;
	char tarStream[REQ_MAX_LEN] = {0};
	char boundry[REQ_MAX_LEN] = {0};

	snprintf(boundry, sizeof(boundry),
		"---------------------------99614912995\r\n");

	snprintf(sendLine,
		sizeof(sendLine),
		"POST /build?t=%s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Content-Type: application/x-tar; boundry=%s"
		"\r\n",
		request,
		ctx->version,
		boundry);

	filePointer = fopen(path, "rb");
	printf(sendLine);
	received = send(ctx->dockerSock, sendLine, sizeof(sendLine), 0);

	while((len = fread(tarStream, 1, REQ_MAX_LEN, filePointer)))
	{
		received = send(ctx->dockerSock, tarStream, len, 0);

		if(-1 == received)
		{
			fclose(filePointer);
			return -DOCKER_IF_HTTP;
		}
	}

	received = send(ctx->dockerSock, boundry, sizeof(boundry), 0);

	fclose(filePointer);

	if (0 < received)
	{
		received = recv(ctx->dockerSock, outString, outStringLen, 0);
		if (0 < received)
		{
			return received;
		}
	}

	return -DOCKER_IF_HTTP;

}




/**
 *  @brief               Destroys the context for this library
 *
 *  @param ctx           Pointer to the context to destroy
 *
 *  @return              DOCKER_IF_NO_SOCKET if failure
 *                       DOCKER_IF_OK if success
 **/
int dockerIfDestroy(SocketContext *ctx)
{
	return (0 == close(ctx->dockerSock)) ?
			DOCKER_IF_OK : DOCKER_IF_NO_SOCKET;
}


