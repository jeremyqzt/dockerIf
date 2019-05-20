# python-traceroute

A C API to interact with the Docker socket, abstracts some common uses cases for me

Note: This uses JSONCPP from https://github.com/open-source-parsers/jsoncpp

Slowly Adding


Installing:

A makefile is provided, so the usual steps of make and make install

```
make
make install
```

# How to use

//Create contex
SocketContext ctx;
DockerImagesList images;

//Initialize Context
dockerIfInit(&ctx, "v1.24", "/var/run/docker.sock", NULL);

//Read Images
dockerIfGet(&ctx, "images/json", theString, sizeof(theString));

//Parsing
ptr = strchr(theString, '[');
dockerIfImageLs(ptr, strlen(ptr), &images);

