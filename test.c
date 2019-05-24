#include <dockerIf.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "dockerIfParser/include/dockerIfParserC.h"

void testImageLs(DockerImagesList *imageList);


int main()
{
	char theString[10000] = {0};
	char *ptr = NULL;
	SocketContext ctx;
	DockerImagesList images;

	dockerIfInit(&ctx, "v1.24", "/var/run/docker.sock", NULL);
	if (0 > dockerIfGet(&ctx, "images/json", theString, sizeof(theString)))
	{
		printf("Failed to read docker images\n");
		exit(-1);
	}

	ptr = strchr(theString, '[');
	if (PARSE_OKAY != dockerIfImageLs(ptr, strlen(ptr), &images))
	{
		printf("Failed to parse docker images\n");
		exit(-1);
	}
	testImageLs(&images);

	//int size = strlen("{\"Image\": \"ubuntu\"}");
	//dockerIfPost(&ctx, "containers/create", "{\"Image\": \"ubuntu\"}", size, theString, sizeof(theString));

	//printf("%s", theString);
	dockerIfImageLsFree(&images);
	dockerIfDestroy(&ctx);

}



void testImageLs(DockerImagesList *imageList)
{
	for (int i = 0; i < imageList->totalImages; i++)
	{
		printf("===Container Number %d===\n", i);
		for (int tags = 0; tags < imageList->dockerImages[i].numRepoTags; tags++)
		{
			printf("Tags: %s\n", imageList->dockerImages[i].repoTags[tags]);
		}

		printf("Created: %s\n", imageList->dockerImages[i].creatDateTime);
		printf("ID: %s\n", imageList->dockerImages[i].containerId);
		printf("Parent ID:%s\n", imageList->dockerImages[i].parentId);
		printf("Size: %d\n", imageList->dockerImages[i].approximateSize);
		printf("Virtual Size: %d\n", imageList->dockerImages[i].virtualSize);
		printf("Shared Size: %d\n", imageList->dockerImages[i].sharedSize);

	}

}