//
//  Install.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Install.h"

USING_NS_JC;

Install::Install(App* app, Device* device, int appVersion)
{
    this->app = app;
    this->device = device;
    this->appVersion = appVersion == - 1 ? app->getLastVersion() : appVersion;
}

Install::~Install()
{
    
}

void Install::update(int newAppVersion)
{
    this->appVersion = newAppVersion;
}