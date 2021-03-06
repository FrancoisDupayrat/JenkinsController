//
//  Device.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Device.h"

USING_NS_JC;

Device::Device(std::string name, std::string identifier, std::string model, std::string osVersion)
{
    this->name = name;
    this->identifier = identifier;
    this->model = model;
    this->osVersion = osVersion;
}

Device::Device(int argc, char** argv)
{
    identifier = std::string(argv[0]);
    name = std::string(argv[1]);
    model = std::string(argv[2]);
    osVersion = std::string(argv[3]);
}

Device::~Device()
{
    
}

void Device::update(std::string newOsVersion)
{
    this->osVersion = osVersion;
}

std::string Device::getName() const
{
    return name;
}

std::string Device::getIdentifier() const
{
    return identifier;
}

std::string Device::getModel() const
{
    return model;
}

std::string Device::getOsVersion() const
{
    return osVersion;
}