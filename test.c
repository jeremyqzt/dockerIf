#include <dockerIf.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/dockerIf.h"


int main()
{
	char theString[2048] = "";
	int ret = -1;
	SocketContext ctx;

	dockerIfInit(&ctx, "v1.24", "/var/run/docker.sock", NULL);

	if (0 > (ret = dockerIfGet(&ctx, "images/json", theString, sizeof(theString))))
	{
		printf("Failed to read docker images, ret: %d\n", ret);
		exit(-1);
	}

	//int size = strlen("{\"Image\": \"ubuntu\"}");
	//dockerIfPost(&ctx, "containers/create", "{\"Image\": \"ubuntu\"}", size, theString, sizeof(theString));

	//dockerIfPostTar(&ctx, "build?t=test3", "/root/dockerIf/Dockerfiles/dockerfile.tar.gz", theString, sizeof(theString));
	//sleep(3);
	printf("%s\n", theString);
	//dockerIfImageLsFree(&images);
	dockerIfDestroy(&ctx);

}

