//
//  App.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "App.h"

USING_NS_JC;

App::App(std::string name, std::string identifier)
{
    this->name = name;
    this->identifier = identifier;
}

App::~App()
{
    
}

void App::update(int newVersion)
{
    lastVersion = newVersion;
}

int App::getLastVersion()
{
    return lastVersion;
}