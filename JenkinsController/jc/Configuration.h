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
    bool isLocal() const;
    void setLocal(std::string url, std::string expansionUrl);
    void setRemote(std::string url, std::string expansionUrl);
    std::string getURL() const;
    std::string getExpansionURL() const;
    void setID(unsigned long id_);
    unsigned long getID() const;
private:
    bool local;
    std::string url;
    std::string expansionUrl;
    unsigned long id_;
};

NS_JC_END

#endif /* defined(__JenkinsController__Configuration__) */
