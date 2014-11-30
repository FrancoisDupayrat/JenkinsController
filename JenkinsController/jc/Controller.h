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
    std::vector<App> getAllApps();
private:
    bool loadConfiguration();
    Configuration* conf;
};

NS_JC_END

#endif /* defined(__JenkinsController__Controller__) */
