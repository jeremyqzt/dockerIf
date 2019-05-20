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

	if (!reader->parse(in, (in + len), &obj, &errs))
	{
		return -1;
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

			cout << imageList->dockerImages[images].repoTags[tags] << endl;
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

	return 1;

}

