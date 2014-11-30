//
//  main.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 28/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include <iostream>
#include "Controller.h"

#define VERSION 0.1

int main(int argc, const char * argv[])
{
    if(argc <= 1)
    {
        std::cout << "Jenkins Controller version " << VERSION << "\nUse jc help to see all available commands\nWarning, current version doesn't protect from SQL injection.\n";
#warning TODO: use prepared statements to protect from SQL injection
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
                std::cout << "Register an app or a device\n";
                std::cout << "Usage jc register device [device-name] [device-id] [model] [OS Version]\n";
                std::cout << "  [device-name] is the easily identifiable device name\n";
                std::cout << "  [device-id] is the unique device ID\n";
                std::cout << "  [model] is the device model (string)\n";
                std::cout << "  [OS Version] is the device OS Version (string)\n";
                std::cout << "Usage jc register app [app-name] [app-id]\n";
                std::cout << "  [app-name] is the short app name, must match Jenkins config\n";
                std::cout << "  [app-id] is the unique app ID\n";
            }
            else if(commandDetail == "udpate")
            {
                std::cout << "Update an app, device or install informations\n";
                std::cout << "Usage jc update device [device] [OS Version]\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "  [OS Version] is the new device OS version (string)\n";
                std::cout << "Usage jc update app [app] [version]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [version] is the new app version. Must be an integer\n";
                std::cout << "Usage jc update install [app] [device] [version]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "  [version] (optional) is the installed app version. Must be an integer\n";
            }
            else if(commandDetail == "remove")
            {
                std::cout << "Remove an app, device or install informations\n";
                std::cout << "Usage jc remove device [device]\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "Usage jc remove app [app]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "Usage jc remove install [app] [device]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [device] is the  device name\n";
            }
            std::cout << "Command unknown";
        }
        return 0;
    }
    jc::Controller* controller = NULL;
    try
    {
        controller = new jc::Controller();
    }
    catch (...)
    {
        std::cout << "Error initializing DB, aborting\n";
    }
    if(command == "build")
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
        if(argc >= 3 && std::string(argv[2]) == "-v")
        {
            std::cout << "Verbose option not implemented yet\n";
        }
        std::vector<jc::App> apps = controller->getAllApps();
        if(apps.size() == 0)
        {
            std::cout << "No app registered\n";
        }
        for(jc::App app : apps)
        {
            std::cout << app.getName() << ", version " + std::to_string(app.getLastVersion()) + ", identifier: " + app.getIdentifier() << "\n";
        }
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
        if(argc < 3)
        {
            std::cout << "missing arguments for command register, see jc help register\n";
        }
        else
        {
            std::string commandDetail = argv[2];
            if(commandDetail == "app")
            {
                if(argc >= 5)
                {
                    bool result;
                    if(argc >= 6 && atoi(argv[5]) > 0)
                    {
                        result = controller->addApp(argv[3], argv[4], atoi(argv[5]));
                    }
                    else
                    {
                        result = controller->addApp(argv[3], argv[4]);
                    }
                    if(result)
                    {
                        std::cout << "Registered app " << argv[3] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the app was not registered";
                    }
                }
                else
                {
                    std::cout << "missing arguments for command register app, see jc help register\n";
                }
            }
        }
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
    delete(controller);
    return 0;
}

