# Docker Interface for C

A C API to interact with the Docker socket, abstracts some common uses cases for me


Slowly Adding


Installing:

A makefile is provided, so the usual steps of make and make install

```
make
make install
```

# How to use
```
//Create contex
SocketContext ctx;

//Initialize Context
dockerIfInit(&ctx, "v1.24", "/var/run/docker.sock", NULL);

//Read Images
dockerIfGet(&ctx, "images/json", theString, sizeof(theString));

//Freeing
dockerIfImageLsFree(&images);
dockerIfDestroy(&ctx);
```
