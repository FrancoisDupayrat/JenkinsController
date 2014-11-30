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
    App(std::string name, std::string identifier);
    ~App();
    void update(int newVersion);
    int getLastVersion();
    
private:
    App() { }
    
    std::string name;
    std::string identifier;
    int lastVersion;
};

NS_JC_END

#endif /* defined(__JenkinsController__App__) */
