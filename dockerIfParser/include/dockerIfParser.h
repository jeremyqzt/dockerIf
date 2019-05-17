#ifndef DOCKER_IF_PARSER_H
#define DOCKER_IF_PARSER_H

#include "dockerIfHeaderParser.h"

extern "C" int dockerIfImageLs(char *in, size_t len, DockerImagesList *imageList);
//extern "C" int readDockerBackGroundContainers(char *in, size_t len, DockerContainerBackground *container);

#endif
