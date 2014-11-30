//
//  Device.h
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__Device__
#define __JenkinsController__Device__

#include <iostream>
#include "jcMacros.h"

NS_JC

class Device
{
public:
    Device(std::string name, std::string identifier, std::string model, std::string osVersion);
    Device(int argc, char** argv);
    ~Device();
    void update(std::string newOsVersion);
    
    std::string getName();
    std::string getIdentifier();
    std::string getModel();
    std::string getOsVersion();
    
    static std::string getTableFormat();
    std::string getInsertSQL(std::string tableName);
    static std::string getUpdateVersionSQL(std::string name, std::string osVersion);
    static std::string getDeleteSQL(std::string name);
private:
    Device() { }
    
    std::string name;
    std::string identifier;
    std::string model;
    std::string osVersion;
};

NS_JC_END


#endif /* defined(__JenkinsController__Device__) */
