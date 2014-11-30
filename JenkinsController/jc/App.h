//
//  App.h
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__App__
#define __JenkinsController__App__

#include <iostream>
#include "jcMacros.h"

NS_JC

class App
{
public:
    App(std::string name, std::string identifier, int version = 1);
    App(int argc, char** argv);
    ~App();
    void update(int newVersion);
    
    std::string getName();
    std::string getIdentifier();
    int getLastVersion();
    
    static std::string getTableFormat();
    std::string getInsertSQL(std::string tableName);
    static std::string getUpdateVersionSQL(std::string name, int version);
    static std::string getDeleteSQL(std::string name);
private:
    App() { }
    
    std::string name;
    std::string identifier;
    int lastVersion;
};

NS_JC_END

#endif /* defined(__JenkinsController__App__) */
