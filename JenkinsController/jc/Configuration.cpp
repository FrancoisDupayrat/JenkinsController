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

bool Configuration::isLocal()
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

std::string Configuration::getTableFormat()
{
    return "("  \
    "ID INT PRIMARY KEY     NOT NULL," \
    "URL            TEXT    NOT NULL," \
    "LOCAL          INT     NOT NULL," \
    "VERSION        INT     NOT NULL);";
}

std::string Configuration::getInsertSQL(std::string tableName)
{
    return "INSERT INTO " + tableName + " (ID, URL, LOCAL, VERSION) VALUES (1, '" + remoteUrl + "', 1, " CONFIGURATION_VERSION ");";
}

bool Configuration::loadFromSQL(int argc, char** argv)
{
    remoteUrl = std::string(argv[1]);
    local = atoi(argv[2]);
    return strcmp(CONFIGURATION_VERSION, argv[3]) != 0;
}