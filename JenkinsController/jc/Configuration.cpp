//
//  Configuration.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Configuration.h"

USING_NS_JC;

Configuration::Configuration()
{
    local = true;
    remoteUrl = "localhost";
}

Configuration::Configuration(Configuration const &copy)
{
    local = copy.local;
    remoteUrl = copy.remoteUrl;
    id_ = copy.id_;
}

bool Configuration::isLocal() const
{
    return local;
}

void Configuration::setLocal()
{
    local = true;
}

void Configuration::setRemote(std::string url)
{
    local = false;
    remoteUrl = url;
}

std::string Configuration::getRemote() const
{
    return remoteUrl;
}