#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "json.h"
#include "json_parser.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

using namespace std;

int readDockerImages(char *in, size_t len, DockerImagesList *imageList)
{
	Json::CharReaderBuilder rbuilder;
	std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());

	Json::Value obj;
	JSONCPP_STRING errs;

	reader->parse(in, (in + len), &obj, &errs);

	int totalImages = obj.size();
	time_t createdTime = time(NULL);

	imageList->dockerImages = (DockerImage*) malloc((totalImages + 1) * sizeof(DockerImage));
	imageList->totalImages = totalImages;

	for (int images = 0; images < totalImages; images++)
	{

		snprintf(imageList->dockerImages[images].repoTags,
				sizeof(imageList->dockerImages[images].repoTags),
				"%s",
				obj[images]["RepoTags"][0].asString().c_str());

		createdTime = obj[images]["Created"].asInt();
		struct tm *timeStruct = localtime(&createdTime);

		strftime(imageList->dockerImages[images].creatDateTime,
				sizeof(imageList->dockerImages[images].creatDateTime),
				"%Y-%m-%d %H:%M:%S", timeStruct);

		snprintf(imageList->dockerImages[images].containerId,
				sizeof(imageList->dockerImages[images].containerId),
				"%s",
				obj[images]["Id"].asString().c_str());

		snprintf(imageList->dockerImages[images].containerId,
				sizeof(imageList->dockerImages[images].containerId),
				"%s",
				obj[images]["ParentId"].asString().c_str());

		imageList->dockerImages[images].approximateSize = obj[images]["Size"].asInt();

	}

	return 1;

}


int readDockerBackGroundContainers(char *in, size_t len, DockerContainerBackground *container)
{
	Json::CharReaderBuilder rbuilder;
	std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());

	Json::Value obj;
	JSONCPP_STRING errs;

	reader->parse(in, (in + len), &obj, &errs);

	snprintf(container->id, sizeof(container->id), "%s", obj["Id"].asString().c_str());

	return 1;

}

#ifdef __cplusplus
} // closing brace for extern "C"
#endif /* __cplusplus */
