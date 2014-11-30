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

App::~App()
{
    
}

void App::update(int newVersion)
{
    lastVersion = newVersion;
}

std::string App::getName()
{
    return name;
}

std::string App::getIdentifier()
{
    return identifier;
}

int App::getLastVersion()
{
    return lastVersion;
}

std::string App::getTableFormat()
{
    return "("  \
    "ID          TEXT PRIMARY KEY  NOT NULL," \
    "NAME        TEXT UNIQUE      NOT NULL," \
    "VERSION     INT               NOT NULL);";
}

std::string App::getInsertSQL(std::string tableName)
{
    return "INSERT INTO " + tableName + " (ID, NAME, VERSION) VALUES ('" + identifier + "', '" + name + "', " + std::to_string(lastVersion) + ");";
}