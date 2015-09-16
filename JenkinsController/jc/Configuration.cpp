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
    expansionUrl = "";
    id_ = 1;
}

Configuration::Configuration(Configuration const &copy)
{
    local = copy.local;
    url = copy.url;
    expansionUrl = copy.expansionUrl;
    id_ = copy.id_;
}

bool Configuration::isLocal() const
{
    return local;
}

void Configuration::setLocal(std::string url, std::string expansionUrl)
{
    local = true;
    this->url = url;
    this->expansionUrl = expansionUrl;
}

void Configuration::setRemote(std::string url, std::string expansionUrl)
{
    local = false;
    this->url = url;
    this->expansionUrl = expansionUrl;
}

std::string Configuration::getURL() const
{
    return url;
}

std::string Configuration::getExpansionURL() const
{
    return expansionUrl;
}

void Configuration::setID(unsigned long id_)
{
    this->id_ = id_;
}

unsigned long Configuration::getID() const
{
    return id_;
}