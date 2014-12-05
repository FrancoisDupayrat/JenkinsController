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
    Device() { }
    ~Device();
    void update(std::string newOsVersion);
    
    std::string getName() const;
    std::string getIdentifier() const;
    std::string getModel() const;
    std::string getOsVersion() const;
    
private:
    std::string name;
    std::string identifier;
    std::string model;
    std::string osVersion;
};

NS_JC_END


#endif /* defined(__JenkinsController__Device__) */
