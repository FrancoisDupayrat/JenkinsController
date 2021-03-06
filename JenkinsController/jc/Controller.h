//
//  Controller.h
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__Controller__
#define __JenkinsController__Controller__

#include <iostream>
#include <vector>
#include <map>
#include "jcMacros.h"
#include "Configuration.h"
#include "App.h"
#include "Device.h"
#include "Install.h"

NS_JC

/* The controller is in charge of interacting with the DB.
   Each method will open and then close a DB connection.
 */
class Controller
{
public:
    typedef enum
    {
        NoOption = 0,
        Force = 1,
        Reinstall = 2,
    } InstallOption;
    //Create the Controller. Caller is responsible for deleting the Controller. Returns nullptr if it was not successful.
    static Controller* createController();
    ~Controller();
    
    bool launchBuild(std::string appName);
    
    bool addApp(std::string appName, std::string appIdentifier, int version = 1);
    bool updateApp(std::string appName, int version);
    bool removeApp(std::string appName);
    std::vector<App> getAllApps();
    App getApp(std::string name);
    
    bool addDevice(std::string deviceName, std::string deviceIdentifier, std::string model, std::string osVersion);
    bool updateDevice(std::string deviceName, std::string osVersion);
    bool removeDevice(std::string deviceName);
    std::vector<Device> getAllDevices();
    std::vector<Device> getConnectedDevices();
    Device getDevice(std::string name);
    
    bool performInstall(App app, Device device, InstallOption = NoOption);
    bool performUninstall(App app, Device device);
    bool updateInstall(std::string appName, std::string deviceName, int version = -1);
    bool removeInstall(std::string appName, std::string deviceName);
    std::vector<Install> getAllAppInstall(std::string appName);
    std::vector<Install> getAllDeviceInstall(std::string deviceName);
    
    bool setConfig(bool local, std::string url, std::string expansionUrl);
    Configuration getConfiguration();
private:
    Controller();
    bool loadConfiguration();
    //Open DB connection. Return nullptr if it was not successful. Caller is responsible for calling sqlite3_close.
    sqlite3* openDB();
    bool checkAppTableExist(sqlite3 *db);
    bool checkDeviceTableExist(sqlite3 *db);
    bool checkInstallTableExist(sqlite3 *db);
    
    //Return a std:map where keys are serial ID, and corresponding values are Android ID
    std::map<std::string, std::string> getConnectedAndroidInfos();
    
    Configuration* conf;
};

NS_JC_END

#endif /* defined(__JenkinsController__Controller__) */
