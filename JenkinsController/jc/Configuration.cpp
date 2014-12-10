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
    url = "";
    id_ = 1;
}

Configuration::Configuration(Configuration const &copy)
{
    local = copy.local;
    url = copy.url;
    id_ = copy.id_;
}

bool Configuration::isLocal() const
{
    return local;
}

void Configuration::setLocal(std::string url)
{
    local = true;
    this->url = url;
}

void Configuration::setRemote(std::string url)
{
    local = false;
    this->url = url;
}

std::string Configuration::getURL() const
{
    return url;
}

void Configuration::setID(unsigned long id_)
{
    this->id_ = id_;
}

unsigned long Configuration::getID() const
{
    return id_;
}