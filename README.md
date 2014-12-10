JenkinsController
=================
JenkinsController, also called jc, is an utility to manage the testing of an app suite on a fleet on device.
The main goal is to know which version of which app is on which device.
It is designed to be integrated with a running Jenkins instance.

Requirements
=================
* the command ```jc devices``` requires command-line tools: 
	* ```system_profiler``` (pre-installed on Mac), 
	* ```sed``` (pre-installed on Mac and Unix)
	* ```adb``` part of Android tools

Build
=================
JenkinsController focus on Mac platform for now. You can build it using Xcode.

It requires gnustl and libgcrypt to compile because of libmicrohttpd library. libmicrohttpd universal binary is included
On mac, use port to install them :
```
 sudo port install libmicrohttpd +universal
 sudo port install gnutls +universal
 sudo port install libgcrypt +universal
 ```
They will be installed on ```opt/local/lib```, which is already in the library search path

It is tested on Mac OS X 10.8.

Install
=================
[TODO]

Run
=================
Run ```jc [command]``` to execute a command. See ```jc help``` for all the available commands
