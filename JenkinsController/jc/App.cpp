//
//  App.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "App.h"

USING_NS_JC;

App::App(std::string name, std::string identifier, int version)
{
    this->name = name;
    this->identifier = identifier;
    lastVersion = version;
}

App::App(int argc, char** argv)
{
    identifier = std::string(argv[0]);
    name = std::string(argv[1]);
    lastVersion = atoi(argv[2]);
}

App::~App()
{
    
}

void App::update(int newVersion)
{
    lastVersion = newVersion;
}

std::string App::getName() const
{
    return name;
}

std::string App::getIdentifier() const
{
    return identifier;
}

int App::getLastVersion() const
{
    return lastVersion;
}