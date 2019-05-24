#ifndef DOCKER_IF_PARSER_H
#define DOCKER_IF_PARSER_H

#include "dockerIfHeaderParser.h"

enum
{
	PARSE_OKAY = 0,
	PARSE_FAIL_JSON,
	PARSE_FAIL_GENERIC
} dockerIfParserCode;


int dockerIfImageLs(char *in, size_t len, DockerImagesList *imageList);
int dockerIfImageLsFree(DockerImagesList *imageList);
//extern "C" int readDockerBackGroundContainers(char *in, size_t len, DockerContainerBackground *container);

#endif
