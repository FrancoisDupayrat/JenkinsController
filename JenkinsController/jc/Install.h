//
//  Install.h
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__Install__
#define __JenkinsController__Install__

#include <iostream>
#include "jcMacros.h"
#include "App.h"
#include "Device.h"

NS_JC

class Install
{
public:
    Install(App app, Device device, int appVersion = -1);
    Install(int argc, char** argv);
    Install() { }
    ~Install();
    void update(int newAppVersion);
    
    App& getApp();
    Device& getDevice();
    int getAppVersion();
    
    static std::string getTableFormat();
    std::string getInsertSQL(std::string tableName);
    static std::string getUpdateVersionSQL(std::string app, std::string device, int version);
    static std::string getWhereSQL(std::string app, std::string device);
    static std::string getAllAppWhereSQL(std::string app);
    static std::string getAllDeviceWhereSQL(std::string device);
private:
    
    App app;
    Device device;
    int appVersion;
};

NS_JC_END


#endif /* defined(__JenkinsController__Install__) */
