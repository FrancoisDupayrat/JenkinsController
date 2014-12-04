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
    
private:
    App app;
    Device device;
    int appVersion;
};

NS_JC_END


#endif /* defined(__JenkinsController__Install__) */
