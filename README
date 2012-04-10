Welcome to the chain wiki!

Background
==========
This is the course project of my UI class. 

It is a small game uses Kinect and runs on Mac OS X. The game itself is not very attractive, but the source code contains all the libraries and frameworks in order for the Kinect to work in Mac OS X. I could be a good start point for other Kinect project that need to run on Mac OS X instead of Microsoft Windows. 

Platform
========
I only tested it on Lion and I also believe the pre-compiled libraries in the system will only work on Lion and later version of Mac OS X. It took me a long time to modify the libraries source code and recompile them, because some of the libraries hard-coded file paths inside the source code.

Modules
=======
The project used several open source project on github. I modified some of the codes to glue them together. Here is the list of modules in my project:

* [Box2d](http://box2d.org/): A 2d physically based simulation engine.
* [libusb](http://www.libusb.org/): A usb driver library. I used that to control the tilting angle of the Kinect camera and the LED light on it. I ripped that part of code from the OpenKinect Library.
* [OpenNI](https://github.com/OpenNI/OpenNI): A user interaction library that provides basic interface to drive the kinect. I changed and recompile its source code to change the hard-coded path in the library to make it portable. The whole project and be deploy independently without installing OpenNI.
* [SensorKinect](https://github.com/avin2/SensorKinect): The Kinect driver used by the OpenNI. Recompiled for Mac OS X Lion.
* [NITE](http://www.openni.org/downloadfiles/openni-compliant-middleware-binaries/33-latest-unstable): The NITE library which provides skeleton recognition algorithm. I am sure that I didn't get the source code from the official download. I must have it ripped off some other open source project on github. And yes, I modified the source and recompiled it to make it portable too.

Portability
===========
I tried very hard to make the project portable. What I mean is you should just need to copy the "chain.app" into your application folder and double click to run it (of course, you need to have a Kinect connected to your machine). You don't need to install any OpenNI, SensorKinect, and NITE library to you machine.

Build
=====
## Install Xcode
Go to the app store and get Xcode. 

## Install [homebrew](http://mxcl.github.com/homebrew/)
The easiest thing to do, Paste this at a shell prompt:
```
/usr/bin/ruby -e "$(/usr/bin/curl -fksSL https://raw.github.com/mxcl/homebrew/master/Library/Contributions/install_homebrew.rb)"
```

## Install cmake
```
sudo brew install cmake
```

## Create xcode project and build
```
cd chain
./cmake-xcode.sh
```
Build with Xcode. You will find the chain.app under `chain/bin/Release/`

## 
Demo
====
Come later...

