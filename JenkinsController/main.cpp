//
//  main.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 28/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include <iostream>

#define VERSION 0.1

int main(int argc, const char * argv[])
{
    if(argc <= 1)
    {
        std::cout << "Jenkins Controller v" << VERSION << "\nUse jc help to see all available commands\n";
        return 0;
    }
    std::string command = argv[1];
    if(command == "help")
    {
        if(argc == 2 || (argc == 3 && std::string(argv[2]) == "help"))
        {
            std::cout << "Available jc commands:\n";
            std::cout << "  help [command]\t\tshow more help on the command\n";
            std::cout << "  build [app]\t\t\tbuild the app\n";
            std::cout << "  install [app] [device]\tinstall app on specfic device\n";
            std::cout << "  build-install [app] [device]\tbuild the app then install on specfic device\n";
            std::cout << "  devices\t\t\tlist connected devices\n";
            std::cout << "  apps\t\t\t\tlist known apps\n";
            std::cout << "  device [device]\t\tshow device details and all apps installed\n";
            std::cout << "  app [app]\t\t\tshow app details and devices installed on\n";
            std::cout << "  configure\t\t\tto configure Jenkins location\n";
            std::cout << "\n";
            std::cout << "Local only commands, should only be used by Jenkins or an admin\n";
            std::cout << "  register\tregister a new app or device\n";
            std::cout << "  update\tupdate an app, device or install\n";
            std::cout << "  remove\tremove an app, device or install\n";
        }
        else
        {
            std::string commandDetail = argv[2];
            if(commandDetail == "build")
            {
                std::cout << "Build an app\n";
                std::cout << "Usage: jc build [app]\n";
                std::cout << "  [app] is the app name\n";
            }
            else if(commandDetail == "install")
            {
                std::cout << "Install an app on a device\n";
                std::cout << "Usage: jc install [app] -all OR [device]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  -all to install on all connected devices, exclusive with [device]\n";
                std::cout << "  [device] is the device name to install on, exclusive with -all\n";
            }
            else if(commandDetail == "build-install")
            {
                std::cout << "Build an app and install on a device\n";
                std::cout << "Usage: jc build-install [app] -all OR [device]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  -all to install on all connected devices, exclusive with [device]\n";
                std::cout << "  [device] is the device name to install on, exclusive with -all\n";
            }
            else if(commandDetail == "devices")
            {
                std::cout << "List connected devices\n";
                std::cout << "Usage: jc devices -v -all\n";
                std::cout << "  -v to have more details\n";
                std::cout << "  -all to see all registered devices as well\n";
            }
            else if(commandDetail == "apps")
            {
                std::cout << "List known apps\n";
                std::cout << "Usage: jc apps -v\n";
                std::cout << "  -v to have more details\n";
            }
            else if(commandDetail == "device")
            {
                std::cout << "List all apps on a device\n";
                std::cout << "Usage: jc device [device]\n";
                std::cout << "  [device] is the device name\n";
            }
            else if(commandDetail == "app")
            {
                std::cout << "List all devices with the app\n";
                std::cout << "Usage: jc app [app]\n";
                std::cout << "  [app] is the app name\n";
            }
            else if(commandDetail == "configure")
            {
                std::cout << "Configure Jenkins location\n";
                std::cout << "Usage: jc configure -v local remote [URL]\n";
                std::cout << "  -v to see current configuration\n";
                std::cout << "  local to set as a local Jenkins install\n";
                std::cout << "  remote to set as a remote Jenkins install, by specifing [URL]\n";
            }
            else if(commandDetail == "register")
            {
                std::cout << "Not implemented yet";
            }
            else if(commandDetail == "udpate")
            {
                std::cout << "Not implemented yet";
            }
            else if(commandDetail == "remove")
            {
                std::cout << "Not implemented yet";        
            }
            std::cout << "Command unknown";
        }
    }
    else if(command == "build")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "install")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "build-install")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "devices")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "apps")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "device")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "app")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "configure")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "register")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "udpate")
    {
        std::cout << "Not implemented yet";
    }
    else if(command == "remove")
    {
        std::cout << "Not implemented yet";        
    }
    else
    {
        std::cout << "jc: '" << command << "' is not a jc command. See 'jc help'.\n";
    }
    
    return 0;
}

