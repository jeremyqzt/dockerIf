#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "include/dockerIfParser.h"
#include "../jsoncpp/json.h"
#include "../jsoncpp/json-forwards.h"

using namespace std;



/**
 *  @brief               Parses a return JSON string from a GET request
 *                       on images/json from the docker socket
 *
 *  Note: This function allocates memory, the corresponding dockerIfImageLsFree()
 *        Must be called after calling this function.
 *
 *  @param in            JSON string to parse
 *  @param len           Size of the string to parse
 *  @param imageList     Pointer to the output of the parsing
 *
 *  @returns             Status about the parsing (dockerIfParserCode)
 **/
extern "C" int dockerIfImageLs(char *in, size_t len, DockerImagesList *imageList)
{
	Json::CharReaderBuilder rbuilder;
	std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());

	Json::Value obj;
	JSONCPP_STRING errs;

	memset(imageList, 0, sizeof(DockerImagesList));

	if (!reader->parse(in, (in + len), &obj, &errs))
	{
		return PARSE_FAIL_JSON;
	}

	int totalImages = obj.size();
	time_t createdTime = time(NULL);
	imageList->dockerImages = (DockerImage*) malloc((totalImages + 1) * sizeof(DockerImage));
	imageList->totalImages = totalImages;

	for (int images = 0; images < totalImages; images++)
	{
		int repoTags = obj[images]["RepoTags"].size();
		imageList->dockerImages[images].numRepoTags = repoTags;

		for (int tags = 0; tags < repoTags && tags < MAX_REPO_TAGS; tags++)
		{
			snprintf(imageList->dockerImages[images].repoTags[tags],
					sizeof(imageList->dockerImages[images].repoTags[tags]),
					"%s",
					obj[images]["RepoTags"][tags].asString().c_str());
		}

		imageList->dockerImages[images].containers = obj[images]["Containers"].asInt();

		createdTime = obj[images]["Created"].asInt();
		struct tm *timeStruct = localtime(&createdTime);

		strftime(imageList->dockerImages[images].creatDateTime,
				sizeof(imageList->dockerImages[images].creatDateTime),
				"%Y-%m-%d %H:%M:%S", timeStruct);

		snprintf(imageList->dockerImages[images].containerId,
				sizeof(imageList->dockerImages[images].containerId),
				"%s",
				obj[images]["Id"].asString().c_str());

		snprintf(imageList->dockerImages[images].parentId,
				sizeof(imageList->dockerImages[images].parentId),
				"%s",
				obj[images]["ParentId"].asString().c_str());

		imageList->dockerImages[images].approximateSize = obj[images]["Size"].asInt();
		imageList->dockerImages[images].sharedSize = obj[images]["SharedSize"].asInt();
		imageList->dockerImages[images].virtualSize = obj[images]["VirtualSize"].asInt();

	}

	return PARSE_OKAY;

}



/**
 *  @brief               If necessary, frees the passed in pointer
 *                       for a dockerIfImageLs() call
 *
 *  @param imageList     Pointer to the docker image list to free
 *
 *  @returns             Void
 **/
extern "C" void dockerIfImageLsFree(DockerImagesList *imageList)
{
	if (imageList->dockerImages)
	{
		free(imageList->dockerImages);
	}
}

