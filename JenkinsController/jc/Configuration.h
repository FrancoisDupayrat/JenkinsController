//
//  Configuration.h
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__Configuration__
#define __JenkinsController__Configuration__

#include <iostream>
#include "jcMacros.h"

//Must be a string for SQL
#define CONFIGURATION_VERSION "1"

NS_JC

class Configuration
{
public:
    Configuration();
    Configuration(Configuration const& copy);
    bool isLocal();
    void setLocal();
    void setRemote(std::string url);
    std::string getRemote();
    
private:
    bool local;
    std::string remoteUrl;
    unsigned long id_;
};

NS_JC_END

#endif /* defined(__JenkinsController__Configuration__) */
