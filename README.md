# LowLevelMiner: Yet another Minecraft clone

LowLevelMiner is a clone of the popular sandbox game Minecraft by Mojang
Studios. Its aim is to provide the developer of this project an understanding
of OpenGL using GLFW and GLAD, using [Learn OpenGL](https://learnopengl.com/)
as a guide.

## Building

Read this section if you don't trust me, or the provided release doesn't work
on your machine for whatever reason.

The project uses CMake as a build tool. Currently supported is only Linux as a
target system, and it will likely stay that way. If you want to, you can create
a pull request for Windows support. I will not check for correctness, only that
you don't put in anything malicious.

Before building, you need to make sure that the following are installed:
* CMake
* A build system of your choice (My choice is Make)
* A C++ compiler of your choice (My choice is GCC)
* The GLFW library and development files
* Maybe libx11?

### Debian

This is a list of commands one may execute if they're on Debian or a derivatve
like Ubuntu:

```
# apt-get install build-essential cmake libglfw3-dev
$ git clone http://github.com/Keheck/LowLevelMiner
$ cd LowLevelMiner
$ cmake --preset Linux-GCC-Debug
$ cd out/build/Linux-GCC-Debug
$ cmake --build .
$ ./LowLevelMiner
```

## Development stage

Here's a list of development stages the project has been and will go through.
It is by no means accurate, and will likely be expanded as I learn more. Some
steps may even be broken down into smaller steps

* [X] Hello Window: Get a window displayed
* [ ] Hello Triangle: Display a triangle on screen