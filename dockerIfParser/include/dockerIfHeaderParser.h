#ifndef DOCKER_IF_HEADER_PARSER_H
#define DOCKER_IF_HEADER_PARSER_H

#include <stdbool.h>

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_SERVER_ERROR 500
#define HTTP_UNKNOWN -1


typedef struct
{
	char *ptr;
	size_t len;
} DockerOutStringStruct;

typedef struct
{
	char containerId[128];
	char parentId[128];
	char repoTags[128];
	char labels[128];
	char creatDateTime[32];
	int approximateSize;
} DockerImage;


typedef struct
{
	DockerImage *dockerImages;
	int totalImages;
} DockerImagesList;


typedef struct
{
	bool init;
	char version[8];
	DockerOutStringStruct dockerString;
} libDockerCtx;


typedef struct
{
	char id[32];
	char warnings[32];
} DockerContainerBackground;

int dockerIfParseHeader(char *dockerResp);

#endif