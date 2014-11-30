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

Device::~Device()
{
    
}

void Device::update(std::string newOsVersion)
{
    this->osVersion = osVersion;
}