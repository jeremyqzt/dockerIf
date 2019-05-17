#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dockerIf/include/dockerIf.h"
#include "include/dockerIfHeaderParser.h"



/**
 *  @brief               Parses the HTTP response code from
 *                       the Docker socket
 *
 *  @param dockeResp     String Response from the docker socket
 *
 *  @returns             HTTP response code or -1 for unknown error
 **/
int dockerIfParseHeader(char *dockerResp)
{
	char* endOfFirstLine = strchr(dockerResp, '\n');
	char httpLine[64] = {0};
	int header = -1;
	char *ptr = NULL;

	if (NULL != endOfFirstLine)
	{
		strncpy(httpLine, dockerResp,
			endOfFirstLine - dockerResp);
	}
	else
	{
		strcpy(httpLine, dockerResp);
	}

	//HTTP/1.1 404 Not Found
	ptr = strstr(httpLine, " ") + 1;
	ptr[4] = '\0';

	header = atoi(ptr);

	switch (header)
	{
		case(HTTP_OK):
		case(HTTP_NOT_FOUND):
		case(HTTP_SERVER_ERROR):
			return header;
		default:
			return HTTP_UNKNOWN;
	}

	return HTTP_UNKNOWN; //???
}