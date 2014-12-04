//
//  Install.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Install.h"
#include "Controller.h"

USING_NS_JC;

Install::Install(App app, Device device, int appVersion)
{
    this->app = app;
    this->device = device;
    this->appVersion = appVersion == - 1 ? app.getLastVersion() : appVersion;
}

Install::Install(int argc, char** argv)
{
    Controller* delegate = Controller::createController();
    if(delegate != nullptr)
    {
        app = delegate->getApp(argv[0]);
        device = delegate->getDevice(argv[1]);
    }
    appVersion = atoi(argv[2]);
}

Install::~Install()
{
    
}

void Install::update(int newAppVersion)
{
    this->appVersion = newAppVersion;
}

App& Install::getApp()
{
    return app;
}

Device& Install::getDevice()
{
    return device;
}

int Install::getAppVersion()
{
    return appVersion;
}
