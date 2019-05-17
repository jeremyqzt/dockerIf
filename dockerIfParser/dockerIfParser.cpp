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

extern "C" int dockerIfImageLs(char *in, size_t len, DockerImagesList *imageList)
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

