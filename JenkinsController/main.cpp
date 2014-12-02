//
//  main.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 28/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include <iostream>
#include "Controller.h"
#include "WebServer.h"

#define VERSION 0.1

/* Return code :
 -1 not implemented
 0 successful
 1 invalid
 2 DB error
 */
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
            std::cout << "  startserver\tstart a JC server\n";
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
                std::cout << "/!\\ When not using -all, this command requires adb, and is slow because of adb.\n";
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
                std::cout << "Usage: jc register device [device-name] [device-id] [model] [OS Version]\n";
                std::cout << "  [device-name] is the easily identifiable device name\n";
                std::cout << "  [device-id] is the unique device ID\n";
                std::cout << "  [model] is the device model (string)\n";
                std::cout << "  [OS Version] is the device OS Version (string)\n";
                std::cout << "Usage: jc register app [app-name] [app-id]\n";
                std::cout << "  [app-name] is the short app name, must match Jenkins config\n";
                std::cout << "  [app-id] is the unique app ID\n";
            }
            else if(commandDetail == "udpate")
            {
                std::cout << "Update an app, device or install informations\n";
                std::cout << "Usage: jc update device [device] [OS Version]\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "  [OS Version] is the new device OS version (string)\n";
                std::cout << "Usage: jc update app [app] [version]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [version] is the new app version. Must be an integer\n";
                std::cout << "Usage: jc update install [app] [device] [version]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "  [version] (optional) is the installed app version. Must be an integer\n";
            }
            else if(commandDetail == "remove")
            {
                std::cout << "Remove an app, device or install informations\n";
                std::cout << "Usage: jc remove device [device]\n";
                std::cout << "  [device] is the  device name\n";
                std::cout << "Usage: jc remove app [app]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "Usage: jc remove install [app] [device]\n";
                std::cout << "  [app] is the app name\n";
                std::cout << "  [device] is the  device name\n";
            }
            else if(commandDetail == "startserver")
            {
                std::cout << "Start a JC server. This instance of JC will be stuck being a server\n";
                std::cout << "Usage: jc startserver [port]\n";
                std::cout << "  [port] (optional) port to start the server on. Default port is 8888\n";
            }
            else
            {
                std::cout << "Command unknown";
                return 1;
            }
        }
        return 0;
    }
    jc::Controller* controller = jc::Controller::createController();
    if(controller == nullptr)
    {
        std::cout << "Error initializing jc DB Controller, aborting\n";
        return 1;
    }
    //The code to return. See the details on main() documentation
    int returnCode = 0;
    if(command == "build")
    {
        std::cout << "Not implemented yet";
        returnCode = -1;
    }
    else if(command == "install")
    {
        std::cout << "Not implemented yet";
        returnCode = -1;
    }
    else if(command == "build-install")
    {
        std::cout << "Not implemented yet";
        returnCode = -1;
    }
    else if(command == "devices")
    {
        bool verbose = false;
        bool all = false;
        for(int i = 2; i < argc; i++)
        {
            std::string arg = argv[i];
            if(arg == "-v")
            {
                verbose = true;
            }
            else if (arg == "-all")
            {
                all = true;
            }
        }
        std::vector<jc::Device> devices = all ? controller->getAllDevices() : controller->getConnectedDevices();
        if(devices.size() == 0)
        {
            std::cout << "No device " << (all ? "registered" : "connected") << "\n";
        }
        for(jc::Device device : devices)
        {
            if(verbose)
            {
                std::cout << device.getName() << "  |  " << device.getModel() << "  |  " << device.getOsVersion() << "  |  " << device.getIdentifier() << "\n";
            }
            else
            {
                std::cout << device.getName() << "\n";
            }
        }
    }
    else if(command == "apps")
    {
        if(argc >= 3 && std::string(argv[2]) == "-v")
        {
            std::cout << "Verbose option not implemented yet\n";
            returnCode = -1;
        }
        std::vector<jc::App> apps = controller->getAllApps();
        if(apps.size() == 0)
        {
            std::cout << "No app registered\n";
        }
        for(jc::App app : apps)
        {
            std::cout << app.getName() << ", version " << std::to_string(app.getLastVersion()) << ", identifier: " << app.getIdentifier() << "\n";
        }
    }
    else if(command == "device")
    {
        if(argc >= 3)
        {
            std::string deviceName = argv[2];
            std::vector<jc::Install> installs = controller->getAllDeviceInstall(deviceName);
            if(installs.size() > 0)
            {
                std::cout << "Apps on " << deviceName << ":\n";
                for(jc::Install install : installs)
                {
                    std::cout << install.getApp().getName() << ", version " + std::to_string(install.getAppVersion()) << "\n";
                }
            }
            else
            {
                std::cout << "No app installed on " << deviceName << "\n";
            }
        }
        else
        {
            std::cout << "Please specify a device\n";
            returnCode = 1;
        }
    }
    else if(command == "app")
    {
        if(argc >= 3)
        {
            std::string appName = argv[2];
            std::vector<jc::Install> installs = controller->getAllAppInstall(appName);
            if(installs.size() > 0)
            {
                std::cout << "Installs for " << appName << ":\n";
                for(jc::Install install : installs)
                {
                    std::cout << install.getDevice().getName() << ", version " + std::to_string(install.getAppVersion()) << "\n";
                }
            }
            else
            {
                std::cout << "No install for " << appName << "\n";
            }
        }
        else
        {
            std::cout << "Please specify an app\n";
            returnCode = 1;
        }
    }
    else if(command == "configure")
    {
        std::cout << "Not implemented yet";
        returnCode = -1;
    }
    else if(command == "register")
    {
        if(argc < 3)
        {
            std::cout << "missing arguments for command register, see jc help register\n";
            returnCode = 1;
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
                        std::cout << "Error, the app was not registered\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command register app, see jc help register\n";
                    returnCode = 1;
                }
            }
            else if (commandDetail == "device")
            {
                if(argc >= 7)
                {
                    if(controller->addDevice(argv[3], argv[4], argv[5], argv[6]))
                    {
                        std::cout << "Registered device " << argv[3] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the device was not registered\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command register device, see jc help register\n";
                    returnCode = 1;
                }
            }
            else
            {
                std::cout << "unknown command register " + commandDetail + ", use jc help register to see all options\n";
                returnCode = 1;
            }
        }
    }
    else if(command == "update")
    {
        if(argc < 3)
        {
            std::cout << "missing arguments for command update, see jc help update\n";
            returnCode = 1;
        }
        else
        {
            std::string commandDetail = argv[2];
            if(commandDetail == "app")
            {
                if(argc >= 5)
                {
                    if(atoi(argv[4]) > 0)
                    {
                        if(controller->updateApp(argv[3], atoi(argv[4])))
                        {
                            std::cout << "Updated app " << argv[3] << "\n";
                        }
                        else
                        {
                            std::cout << "Error, the app was not updated\n";
                            returnCode = 2;
                        }
                    }
                    else
                    {
                        std::cout << "The app version is not valid, must be > 0\n";
                        returnCode = 1;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command update app, see jc help update\n";
                    returnCode = 1;
                }
            }
            else if(commandDetail == "device")
            {
                if(argc >= 5)
                {
                    if(controller->updateDevice(argv[3], argv[4]))
                    {
                        std::cout << "Updated device " << argv[3] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the device was not updated\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command update device, see jc help update\n";
                    returnCode = 1;
                }
            }
            else if(commandDetail == "install")
            {
                if(argc >= 5)
                {
                    bool result = false;
                    if(argc >= 6)
                    {
                        if(atoi(argv[5]) > 0)
                        {
                            result = controller->updateInstall(argv[3], argv[4], atoi(argv[5]));
                        }
                        else
                        {
                            std::cout << "The version must be > 0";
                            returnCode = 1;
                        }
                    }
                    else
                    {
                        result = controller->updateInstall(argv[3], argv[4]);
                    }
                    if(result)
                    {
                        std::cout << "Updated install for app " << argv[3] << " on device " << argv[4] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the install was not updated\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command update install, see jc help update\n";
                    returnCode = 1;
                }
            }
            else
            {
                std::cout << "unknown command update " + commandDetail + ", use jc help update to see all options\n";
                returnCode = 1;
            }
        }
    }
    else if(command == "remove")
    {
        if(argc < 3)
        {
            std::cout << "missing arguments for command update, see jc help update\n";
            returnCode = 1;
        }
        else
        {
            std::string commandDetail = argv[2];
            if(commandDetail == "app")
            {
                if(argc >= 4)
                {
                    if(controller->removeApp(argv[3]))
                    {
                        std::cout << "Removed app " << argv[3] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the app was not removed\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command remove app, see jc help remove\n";
                    returnCode = 1;
                }
            }
            else if(commandDetail == "device")
            {
                if(argc >= 4)
                {
                    if(controller->removeDevice(argv[3]))
                    {
                        std::cout << "Removed device " << argv[3] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the device was not removed\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command remove device, see jc help remove\n";
                    returnCode = 1;
                }
            }
            else if(commandDetail == "install")
            {
                if(argc >= 5)
                {
                    if(controller->removeInstall(argv[3], argv[4]))
                    {
                        std::cout << "Removed install for app " << argv[3] << " on device " << argv[4] << "\n";
                    }
                    else
                    {
                        std::cout << "Error, the install was not removed\n";
                        returnCode = 2;
                    }
                }
                else
                {
                    std::cout << "missing arguments for command remove install, see jc help remove\n";
                    returnCode = 1;
                }
            }
            else
            {
                std::cout << "unknown command remove " + commandDetail + ", use jc help remove to see all options\n";
                returnCode = 1;
            }
        }
    }
    else if(command == "startserver")
    {
        int port = 8888;
        if(argc >= 3)
        {
            port = atoi(argv[2]);
        }
        if(WebServer::startServer(port))
        {
            std::cout << "Server started on port " << port << ", press enter to stop it.";
            std::cin.get();
            WebServer::stopServer();
        }
        else
        {
            std::cout << "Error starting server";
            returnCode = 1;
        }
    }
    else
    {
        std::cout << "jc: '" << command << "' is not a jc command. See 'jc help'.\n";
        returnCode = 1;
    }
    delete(controller);
    return returnCode;
}

