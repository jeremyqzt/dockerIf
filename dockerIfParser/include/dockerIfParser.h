#ifndef DOCKER_IF_HEADER_PARSER_H
#define DOCKER_IF_HEADER_PARSER_H

#include <stdbool.h>

#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_SERVER_ERROR 500
#define HTTP_UNKNOWN -1

#define COMMON_LEN 128
#define MAX_REPO_TAGS 10

enum
{
	PARSE_OKAY = 0,
	PARSE_FAIL_JSON,
	PARSE_FAIL_GENERIC
} dockerIfParserCode;

typedef struct
{
	int containers;
	char containerId[COMMON_LEN];
	char parentId[COMMON_LEN];
	//10 repo tags populated maximum
	int numRepoTags;
	char repoTags[MAX_REPO_TAGS][32];
	char labels[COMMON_LEN];
	char creatDateTime[COMMON_LEN];
	int approximateSize;
	int sharedSize;
	int virtualSize;
} DockerImage;


typedef struct
{
	DockerImage *dockerImages;
	int totalImages;
} DockerImagesList;


typedef struct
{
	bool init;
	char version[COMMON_LEN];
} libDockerCtx;


typedef struct
{
	char id[COMMON_LEN];
	char warnings[COMMON_LEN];
} DockerContainerBackground;


#endif