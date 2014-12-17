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
It is tested on Mac OS X 10.8

Install
=================
To install JenkinsController, copy the jc file in /usr/bin (or any other location defined in your PATH environment variable).

The database will be saved in ~/Library/Application Support/jc

If you can't run jc, check the permissions (usually, ```chmod 777 jc``` will fix the problem)

After installing, don't forget to configure it using ```jc configure```.

Run
=================
Run ```jc [command]``` to execute a command. See ```jc help``` for all the available commands

First, you should use jc configure to set the jc instance as either local or remote
* local: you need to point to an apps folder, which will contain the [app-name].(apk/ipa) files: ```jc configure local [path-to-your-apps-folder]```
* remote: [NOT IMPLEMENTED] you need to point to a remote jc server: ```jc configure remote [jc-server-url]```


Jenkins Configuration
=================

To register the new app build, your app job need to:
* check that jc is on local
* copy your .apk and .ipa to jc app path
* update the app in jc

Here is the script to do all that, using $BUILD\_NUMBER to tag the version, and assuming you set $APP\_NAME:
 ```
 #Check jc conf is on local
JC_CONF=$(jc configure -v | sed '1 s/Configuration: //' | sed '2d')
echo $JC_CONF
if [ $JC_CONF != local ]; then
echo "Please configure JC as local"
exit 1
fi

#Copy apps for jc and update the app version on jc
JC_APP_PATH=$(jc configure -v | sed '2 s/App folder: //' | sed '1d')
cp -f /path/to/your.apk "$JC_APP_PATH/$APP_NAME.apk"
cp -f /path/to/your.ipa "$JC_APP_PATH/$APP_NAME.ipa"
jc update app $APP_NAME $BUILD_NUMBER 
```
