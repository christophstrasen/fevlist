fevlist
===

Small tool to list event paths inside an FMOD `.bank` file.
Forked from original author zatherz/fevlist:master and full props to them.

This version has NOT been tested much. Use at own risk.

## Usage

```
help: fevlist -h|--help
usage: fevlist [-v|--verbose -d|--fmod-debug] PATH_TO_TARGET_BANK
```

## Building

### Linux

Building requires `make` and a C++ compiler like `g++`.

* Go to [the FMOD Download page](https://www.fmod.com/download) and sign in.
* Click on "FMOD Studio Suite", scroll down to "FMOD Engine", make sure you use a 2.x version from the list.
* Download the Linux archive.
* Unpack this archive somewhere.
* Go into the `fevlist` root directory with the `Makefile` and run `make CPU=... FMOD_STUDIO_PATH=... `, like so:
  ```
  make CPU=x86_64 FMOD_STUDIO_PATH=/home/zatherz/Misc/fmodstudioapi11020linux/
  ```
  `CPU` should be one of `[x86|x86_64|arm|armhf]`, and `FMOD_STUDIO_PATH` should be the path to the unpacked files you just downloaded.
* Done!

