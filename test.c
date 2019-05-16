#include <dockerIf.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "dockerIfParser/include/dockerIfParser.h"


int main()
{
	char theString[10000] = {0};
	SocketContext ctx;

	dockerIfInit(&ctx, "v1.24", "/var/run/docker.sock", NULL);

	dockerIfGet(&ctx, "images/json", theString, sizeof(theString));
	printf("%d\n", dockerIfParseHeader(theString));

	int size = strlen("{\"Image\": \"ubuntu\"}");
	//dockerIfPost(&ctx, "containers/create", "{\"Image\": \"ubuntu\"}", size, theString, sizeof(theString));

	//printf("%s", theString);

	dockerIfDestroy(&ctx);

}
