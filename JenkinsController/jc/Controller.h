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
#include "jcMacros.h"
#include "Configuration.h"
#include "App.h"
#include "Device.h"
#include "Install.h"

NS_JC

class Controller
{
public:
    Controller();
    
    bool addApp(std::string appName, std::string appIdentifier, int version = 1);
    bool updateApp(std::string appName, int version);
    bool removeApp(std::string appName);
    std::vector<App> getAllApps();
    App getApp(std::string name);
    
    bool addDevice(std::string deviceName, std::string deviceIdentifier, std::string model, std::string osVersion);
    bool updateDevice(std::string deviceName, std::string osVersion);
    bool removeDevice(std::string deviceName);
    std::vector<Device> getAllDevices();
    Device getDevice(std::string name);
    
    bool updateInstall(std::string appName, std::string deviceName, int version = -1);
    bool removeInstall(std::string appName, std::string deviceName);
    std::vector<Install> getAllAppInstall(std::string appName);
    std::vector<Install> getAllDeviceInstall(std::string deviceName);
private:
    bool loadConfiguration();
    bool checkAppTableExist(sqlite3 *db);
    bool checkDeviceTableExist(sqlite3 *db);
    bool checkInstallTableExist(sqlite3 *db);
    Configuration* conf;
};

NS_JC_END

#endif /* defined(__JenkinsController__Controller__) */
