//
//  Controller.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Controller.h"
#include <sys/stat.h>
#include <curl/curl.h>

static CURL* curlHandle = NULL;
static int controllerCounts = 0;

USING_NS_JC;
using namespace std;

std::vector<std::string> exec(std::string cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    std::vector<std::string> result;
    if (!pipe) return result;
    char buffer[1024];
    while(!feof(pipe)) {
    	if(fgets(buffer, 1024, pipe) != NULL)
        {
            std::string line = std::string(buffer);
            line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
    		result.push_back(line);
        }
    }
    pclose(pipe);
    return result;
}

Controller* Controller::createController()
{
    Controller* controller = new Controller();
    if(curlHandle == NULL)
    {
        curlHandle = curl_easy_init();
    }
    if(!controller->loadConfiguration())
    {
        delete(controller);
        return nullptr;
    }
    controllerCounts++;
    return controller;
}

Controller::~Controller()
{
    controllerCounts--;
    if(controllerCounts == 0)
    {
        curl_easy_cleanup(curlHandle);
        curlHandle = NULL;
    }
}

bool Controller::launchBuild(std::string appName)
{
    App app = getApp(appName);
    if(app.getName() != appName)
    {
        fprintf(stderr, "The app %s is not registered in jc.\n", appName.c_str());
        return false;
    }
    curl_easy_setopt(curlHandle, CURLOPT_URL, ("http://localhost:8080/job/" + appName + "/build").c_str());
    curl_easy_perform(curlHandle);
    return true;
}


//Helper define to avoid repeating the error checking
#define RETURN_ON_SQL_ERROR(RETURN_VALUE) if( result != SQLITE_OK ) { \
fprintf(stderr, "SQL error: %s\n", errorMessage);\
sqlite3_free(errorMessage);\
sqlite3_close(db);\
return RETURN_VALUE; }

bool Controller::addApp(std::string appName, std::string appIdentifier, int version)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkAppTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("INSERT INTO app (ID, NAME, VERSION) VALUES ('" + appIdentifier + "', '" + appName + "', " + std::to_string(version) + ");").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

bool Controller::updateApp(std::string appName, int version)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkAppTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("UPDATE app SET VERSION=" + std::to_string(version) + " WHERE NAME='" + appName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}


bool Controller::removeApp(std::string appName)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkAppTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("DELETE FROM app WHERE NAME='" + appName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

std::vector<App> Controller::getAllApps()
{
    std::vector<App> registeredApps;
    sqlite3 *db;
    if((db = openDB()) == nullptr) return registeredApps;
    if(!checkAppTableExist(db)) return registeredApps;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              "SELECT * FROM app;",
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<App>* registeredApps = (std::vector<App>*)ptr;
                                  registeredApps->push_back(App(std::string(argv[1]), //Name
                                                                std::string(argv[0]), //Id
                                                                atoi(argv[2]))); //Version
                                  return 0;
                              },
                              &registeredApps,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(registeredApps)
    
    sqlite3_close(db);
    return registeredApps;
}

App Controller::getApp(std::string name)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return App();
    if(!checkAppTableExist(db)) return App();
    char *errorMessage = 0;
    
    std::vector<App> matchingApp;
    int result = sqlite3_exec(db,
                              ("SELECT * FROM app WHERE NAME='" + name + "';").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<App>* matchingApp = (std::vector<App>*)ptr;
                                  matchingApp->push_back(App(std::string(argv[1]), //Name
                                                             std::string(argv[0]), //Id
                                                             atoi(argv[2]))); //Version
                                  return 0;
                              },
                              &matchingApp,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(App())
    
    sqlite3_close(db);
    return matchingApp.size() > 0 ? matchingApp[0] : App();
}

bool Controller::addDevice(std::string deviceName, std::string deviceIdentifier, std::string model, std::string osVersion)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkDeviceTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("INSERT INTO device (ID, NAME, MODEL, VERSION) VALUES ('" + deviceIdentifier + "', '" + deviceName + "', '" + model + "', '" + osVersion + "');").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

bool Controller::updateDevice(std::string deviceName, std::string osVersion)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkDeviceTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("UPDATE device SET VERSION=" + osVersion + " WHERE NAME='" + deviceName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}


bool Controller::removeDevice(std::string deviceName)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkDeviceTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("DELETE FROM device WHERE NAME='" + deviceName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

std::vector<Device> Controller::getAllDevices()
{
    std::vector<Device> registeredDevices;
    sqlite3 *db;
    if((db = openDB()) == nullptr) return registeredDevices;
    if(!checkDeviceTableExist(db)) return registeredDevices;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              "SELECT * FROM device;",
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Device>* registeredDevices = (std::vector<Device>*)ptr;
                                  registeredDevices->push_back(Device(std::string(argv[1]), //Name
                                                                      std::string(argv[0]), //Identifier
                                                                      std::string(argv[2]), //Model
                                                                      std::string(argv[3]))); //OSVersion
                                  return 0;
                              },
                              &registeredDevices,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(registeredDevices)
    
    sqlite3_close(db);
    return registeredDevices;
}

std::vector<Device> Controller::getConnectedDevices()
{
    std::vector<Device> connectedDevice;
    //Get all devices IDs
    //For iOS, read connected USB devices, and the UDID is under "Serial Number", like any sane person would think of
    for(std::string iosUDID : exec("system_profiler SPUSBDataType | sed -n -E -e \'/(iPhone|iPad)/,/Serial/s/ *Serial Number: *(.+)/\\1/p\'"))
    {
        connectedDevice.push_back(Device("Unknown", iosUDID, "iOS device", "OS unknown"));
    }
    /*For Android, we can't read connected USB devices, because there is no way to differentiate an Android from other things (hub, mouse, keyboard ...). 
     Instead, use ADB, which is slow as fuck. And of course, it doesn't return the Android ID, but a useless "Serial Number", which is NOT A FUCKING SERIAL NUMBER, because some manufacturer are too lazy to change it. 
     So hope that you don't have 2 devices from a lazy manufacturer and use ADB SHELL to get the real Android ID with some mumbo jumbo, and hope Google doesn't decide to remove that access in the future...*/
    for(std::string androidID : exec("for serial in $(adb devices | sed s/\\	.*// | sed \"1 d\");do\nadb -s ${serial} shell content query --uri content://settings/secure --projection name:value --where \"name=\'android_id\'\" | sed s/Row:\\ [0-9]*\\ name=android_id,\\ value=//\ndone"))
    {
        //Last character is garbage, remove it
        connectedDevice.push_back(Device("Unknown", androidID.substr(0, androidID.length() - 1), "Android device", "OS unknown"));;
    }
    
    //Find all devices infos, if we have them
    sqlite3 *db;
    if((db = openDB()) == nullptr) return connectedDevice;
    if(!checkDeviceTableExist(db)) return connectedDevice;
    char *errorMessage = 0;
    std::vector<Device> matchingDevices;
    for(int i = 0; i < connectedDevice.size(); i++)
    {
        Device& d = connectedDevice.at(i);
        sqlite3_exec(db,
                     ("SELECT * FROM device WHERE ID='" + d.getIdentifier() + "';").c_str(),
                     [](void *ptr, int argc, char **argv, char **azColName)
                     {
                         std::vector<Device>* matchingDevices = (std::vector<Device>*)ptr;
                         matchingDevices->push_back(Device(std::string(argv[1]), //Name
                                                          std::string(argv[0]), //Identifier
                                                          std::string(argv[2]), //Model
                                                          std::string(argv[3]))); //OSVersion
                         return 0;
                     },
                     &matchingDevices,
                     &errorMessage);
    }
    sqlite3_close(db);
    
    //Merge both arrays
    for(Device d : matchingDevices)
    {
        connectedDevice.erase(std::remove_if(connectedDevice.begin(),
                                             connectedDevice.end(),
                                             [&] (const Device& unknownDevice)
                                             {
                                                 return unknownDevice.getIdentifier() == d.getIdentifier();
                                             }));
    }
    for(Device d : connectedDevice)
    {
        matchingDevices.push_back(d);
    }
    
    return matchingDevices;
}

Device Controller::getDevice(std::string name)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return Device();
    if(!checkDeviceTableExist(db)) return Device();
    char *errorMessage = 0;
    
    std::vector<Device> matchingDevice;
    int result = sqlite3_exec(db,
                              ("SELECT * FROM device WHERE NAME='" + name + "';").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Device>* matchingDevice = (std::vector<Device>*)ptr;
                                  matchingDevice->push_back(Device(std::string(argv[1]), //Name
                                                                   std::string(argv[0]), //Identifier
                                                                   std::string(argv[2]), //Model
                                                                   std::string(argv[3]))); //OSVersion
                                  return 0;
                              },
                              &matchingDevice,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(Device())
    
    sqlite3_close(db);
    return matchingDevice.size() > 0 ? matchingDevice[0] : Device();
}

bool Controller::performInstall(App app, Device device, InstallOption option)
{
    bool isAndroid = device.getIdentifier().length() < 40;
    std::string appPath = conf->getURL() + (conf->getURL()[conf->getURL().length() - 1] == '/' ? "" : "/") + app.getName() + (isAndroid ? ".apk" : ".ipa");
    struct stat buffer;
    if(stat (appPath.c_str(), &buffer) != 0)
    {
        fprintf(stderr, "Can't install %s, %s is not readable.\n", app.getName().c_str(), appPath.c_str());
        return false;
    }
    bool deviceFound = false;
    bool appInstalled = false;
    std::string installError;
    if(isAndroid)
    {
        for(std::string serialAndAndroidID : exec("for serial in $(adb devices | sed s/\\	.*// | sed \"1 d\");do\necho ${serial},$(adb -s ${serial} shell content query --uri content://settings/secure --projection name:value --where \"name=\'android_id\'\" | sed s/Row:\\ [0-9]*\\ name=android_id,\\ value=//)\ndone"))
        {
            std::string serial = serialAndAndroidID.substr(0, serialAndAndroidID.find(","));
            std::string androidID = serialAndAndroidID.substr(serialAndAndroidID.find(",") + 1, serialAndAndroidID.length() - serialAndAndroidID.find(",") - 2);
            if(device.getIdentifier() == androidID)
            {
                deviceFound = true;
                if(option == Reinstall)
                { //Uninstall app first. Don't use the method as it would re-run the serial detection, which is very time consuming
                    std::vector<std::string> appInstalledResult = exec("adb shell pm list packages | grep " + app.getIdentifier());
                    bool appInstalled = appInstalledResult.size() > 0;
                    if(appInstalled)
                    {
                        std::vector<std::string> results = exec("adb -s " + serial + " uninstall " + app.getIdentifier());
                        std::string success = "Success";
                        std::string execResult = results.at(results.size() - 1);
                        if(execResult.compare(0, success.length(), success) != 0)
                        {
                            installError = execResult + "\n";
                        }
                        else
                        {
                            std::cout << "App uninstalled, reinstalling...\n";
                        }
                    }
                }
                std::vector<std::string> results = exec("adb -s " + serial + " install -r " + appPath);
                std::string success = "Success";
                std::string execResult = results.at(results.size() - 1);
                if(execResult.compare(0, success.length(), success) == 0)
                {
                    appInstalled = true;
                }
                else
                {
                    if(execResult.find("[INSTALL_FAILED_VERSION_DOWNGRADE]") != std::string::npos
                           || execResult.find("[INSTALL_PARSE_FAILED_INCONSISTENT_CERTIFICATES]") != std::string::npos)
                    {
                        bool shouldReinstall = option == Force;
                        if(!shouldReinstall)
                        {
                            std::string userInput;
                            do
                            {
                                if (userInput != "") std::cout << "Invalid input. ";
                                std::cout << "Cannot do an update, reason: " << execResult << "\nDo you want to reinstall the app? [y/n]\n";
                                std::cin >> userInput;
                                std::transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
                            }while (userInput != "yes"
                                    && userInput != "no"
                                    && userInput != "y"
                                    && userInput != "n");
                            if(userInput == "yes" || userInput == "y")
                            {
                                shouldReinstall = true;
                            }
                            else
                            {
                                installError = "App cannot be updated, you refused the resintall.";
                            }
                        }
                        else
                        {
                            std::cout << "Can't update app, starting reinstall...\n";
                        }
                        if(shouldReinstall)
                        {
                            std::vector<std::string> uninstallResults = exec("adb -s " + serial + " uninstall " + app.getIdentifier());
                            std::string success = "Success";
                            std::string execResult = uninstallResults.at(uninstallResults.size() - 1);
                            if(execResult.compare(0, success.length(), success) != 0)
                            {
                                installError = execResult + "\n";
                            }
                            else
                            {
                                std::cout << "App uninstalled, reinstalling...\n";
                            }
                            
                            std::vector<std::string> results = exec("adb -s " + serial + " install -r " + appPath);
                            execResult = results.at(results.size() - 1);
                            if(execResult.compare(0, success.length(), success) == 0)
                            {
                                appInstalled = true;
                            }
                            else
                            {
                                installError += execResult;
                            }
                        }
                    }
                    else
                    {
                        installError += execResult;
                    }
                }
            }
        }
    }
    else
    {
        std::vector<std::string> results = exec("ideviceinstaller -u " + device.getIdentifier() + " -i " + appPath + " 2>&1");
        std::string noDeviceString = "No iOS device found, is it plugged in?";
        if(results.size() >= 1 && results[0].compare(0, noDeviceString.length(), noDeviceString) == 0)
        {
            deviceFound = false;
        }
        else
        {
            deviceFound = true;
            appInstalled = true;
        }
    }
    if(!deviceFound)
    {
        fprintf(stderr, "Can't install %s, device %s was not found\n", app.getName().c_str(), device.getName().length() > 0 && device.getName() != "Unknown" ? device.getName().c_str() : device.getIdentifier().c_str());
    }
    else if(!appInstalled)
    {
        fprintf(stderr, "Can't install %s, error during installation: %s\n", app.getName().c_str(),
                installError.c_str());
    }
    else if(device.getName().length() > 0 && device.getName() != "Unknown")
    {
        updateInstall(app.getName(), device.getName());
    }
    return deviceFound;
}

bool Controller::performUninstall(App app, Device device)
{
    bool isAndroid = device.getIdentifier().length() < 40;
    bool deviceFound = false;
    bool appUninstalled = false;
    std::string uninstallError;
    if(isAndroid)
    {
        for(std::string serialAndAndroidID : exec("for serial in $(adb devices | sed s/\\	.*// | sed \"1 d\");do\necho ${serial},$(adb -s ${serial} shell content query --uri content://settings/secure --projection name:value --where \"name=\'android_id\'\" | sed s/Row:\\ [0-9]*\\ name=android_id,\\ value=//)\ndone"))
        {
            std::string serial = serialAndAndroidID.substr(0, serialAndAndroidID.find(","));
            std::string androidID = serialAndAndroidID.substr(serialAndAndroidID.find(",") + 1, serialAndAndroidID.length() - serialAndAndroidID.find(",") - 2);
            if(device.getIdentifier() == androidID)
            {
                deviceFound = true;
                std::vector<std::string> appInstalledResult = exec("adb shell pm list packages | grep " + app.getIdentifier());
                bool appInstalled = appInstalledResult.size() > 0;
                if(appInstalled)
                {
                    std::vector<std::string> results = exec("adb -s " + serial + " uninstall " + app.getIdentifier());
                    std::string success = "Success";
                    std::string execResult = results.at(results.size() - 1);
                    if(execResult.compare(0, success.length(), success) == 0)
                    {
                        appUninstalled = true;
                    }
                    else
                    {
                        uninstallError = execResult;
                    }
                }
                else
                {
                    std::cout <<  app.getName() << " was already not installed on " << (device.getName().length() > 0 && device.getName() != "Unknown" ? device.getName() : device.getIdentifier()) << "\n";
                    appUninstalled = true;
                }
            }
        }
    }
    else
    {
        std::vector<std::string> results = exec("ideviceinstaller -u " + device.getIdentifier() + " -U " + app.getIdentifier() + " 2>&1");
        std::string noDeviceString = "No iOS device found, is it plugged in?";
        if(results.size() >= 1 && results[0].compare(0, noDeviceString.length(), noDeviceString) == 0)
        {
            deviceFound = false;
        }
        else
        {
            deviceFound = true;
            appUninstalled = true;
        }
    }
    if(!deviceFound)
    {
        fprintf(stderr, "Can't uninstall %s, device %s was not found\n", app.getName().c_str(), device.getName().length() > 0 && device.getName() != "Unknown" ? device.getName().c_str() : device.getIdentifier().c_str());
    }
    else if(!appUninstalled)
    {
        fprintf(stderr, "Can't uninstall %s, error during installation: %s\n", app.getName().c_str(),
                uninstallError.c_str());
    }
    else if(device.getName().length() > 0 && device.getName() != "Unknown")
    {
        removeInstall(app.getName(), device.getName());
    }
    return deviceFound;
}

bool Controller::updateInstall(std::string appName, std::string deviceName, int version)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkInstallTableExist(db)) return false;
    char *errorMessage = 0;
    
    bool installExist = false;
    int result = sqlite3_exec(db,
                              ("SELECT * FROM install WHERE APP='" + appName + "' AND DEVICE='" + deviceName + "';").c_str(),
                              [](void *installExist, int argc, char **argv, char **azColName)
                              {
                                  *(bool*)installExist = true;
                                  return 0;
                              },
                              &installExist,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    App app = getApp(appName);
    if(version <= 0)
    {
        version = app.getLastVersion();
    }
    
    if(installExist)
    {
        result = sqlite3_exec(db,
                              ("UPDATE install SET VERSION=" + std::to_string(version) + " WHERE APP='" + appName + "' AND DEVICE='" + deviceName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    else
    {
        Install install(app, getDevice(deviceName), version);
        if(install.getApp().getName().length() > 0 && install.getDevice().getName().length() > 0)
        {
            result = sqlite3_exec(db,
                                  ("INSERT INTO install (APP, DEVICE, VERSION) VALUES ('"
                                   + install.getApp().getName() + "', '"
                                   + install.getDevice().getName() + "', "
                                   + std::to_string(install.getAppVersion()) + ");").c_str(),
                                  [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                                  0,
                                  &errorMessage);
            RETURN_ON_SQL_ERROR(false)
        }
        else
        {
            fprintf(stderr, "Invalid app or device for this install");
            sqlite3_close(db);
            return false;
        }
    }
    
    sqlite3_close(db);
    return true;
}

bool Controller::removeInstall(std::string appName, std::string deviceName)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    if(!checkInstallTableExist(db)) return false;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("DELETE FROM install WHERE APP='" + appName + "' AND DEVICE='" + deviceName + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

std::vector<Install> Controller::getAllAppInstall(std::string appName)
{
    std::vector<Install> installs;
    sqlite3 *db;
    if((db = openDB()) == nullptr) return installs;
    if(!checkInstallTableExist(db)) return installs;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("SELECT * FROM install WHERE APP='" + appName + "';").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Install>* installs = (std::vector<Install>*)ptr;
                                  //Assume all installs from DB are valid
                                  installs->push_back(Install(argc, argv));
                                  return 0;
                              },
                              &installs,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(installs)
    
    sqlite3_close(db);
    return installs;
}

std::vector<Install> Controller::getAllDeviceInstall(std::string deviceName)
{
    std::vector<Install> installs;
    sqlite3 *db;
    if((db = openDB()) == nullptr) return installs;
    if(!checkInstallTableExist(db)) return installs;
    char *errorMessage = 0;
    
    int result = sqlite3_exec(db,
                              ("SELECT * FROM install WHERE DEVICE='" + deviceName + "';").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Install>* installs = (std::vector<Install>*)ptr;
                                  //Assume all installs from DB are valid
                                  installs->push_back(Install(argc, argv));
                                  return 0;
                              },
                              &installs,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(installs)
    
    sqlite3_close(db);
    return installs;
}


bool Controller::setConfig(bool local, std::string url)
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    char *errorMessage = 0;
    
    if(local)
    {
        conf->setLocal(url);
    }
    else
    {
        conf->setRemote(url);
    }
    int result = sqlite3_exec(db,
                              ("UPDATE configuration SET LOCAL=" + std::string(conf->isLocal() ? "1" : "0") + ", URL='" + conf->getURL() + "' WHERE ID='" + std::to_string(conf->getID()) + "';").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    sqlite3_close(db);
    return true;
}

Configuration Controller::getConfiguration()
{
    return *conf;
}

Controller::Controller()
{
    conf = nullptr;
}

bool Controller::loadConfiguration()
{
    sqlite3 *db;
    if((db = openDB()) == nullptr) return false;
    char *errorMessage = 0;
    
    if(conf != nullptr)
    {
        delete(conf);
        conf = nullptr;
    }
    conf = new Configuration();
    
    bool tableExists = false;
    int result = sqlite3_exec(db,
                              "SELECT name FROM sqlite_master WHERE type='table' AND name='configuration';",
                              [](void *tableExists, int argc, char **argv, char **azColName)
                              {
                                  *(bool*)tableExists = true;
                                  return 0;
                              },
                              &tableExists,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              "CREATE TABLE configuration("  \
                              "ID INT PRIMARY KEY     NOT NULL," \
                              "URL            TEXT    NOT NULL," \
                              "LOCAL          INT     NOT NULL," \
                              "VERSION        INT     NOT NULL);",
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
        
        result = sqlite3_exec(db,
                              ("INSERT INTO configuration (ID, URL, LOCAL, VERSION) VALUES (" + std::to_string(conf->getID()) + ", '" + conf->getURL() + "', " + std::string(conf->isLocal() ? "1" : "0")  + ", " CONFIGURATION_VERSION ");").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
        std::cout << "Wrote default configuration into DB\n";
    }
    else
    {
        result = sqlite3_exec(db,
                              "SELECT * from configuration",
                              [](void *conf, int argc, char **argv, char **azColName)
                              {
                                  ((Configuration*)conf)->setID(atoi(argv[0]));
                                  if(atoi(argv[2]))
                                  {
                                      ((Configuration*)conf)->setLocal(std::string(argv[1]));
                                  }
                                  else
                                  {
                                      ((Configuration*)conf)->setRemote(std::string(argv[1]));
                                  }
                                  
                                  if(strcmp(CONFIGURATION_VERSION, argv[3]) != 0)
                                  {
                                      fprintf(stderr, "Invalid configuration version, expected : %s, in database : %s", CONFIGURATION_VERSION, argv[3]);
                                  }
                                  return strcmp(CONFIGURATION_VERSION, argv[3]);
                              },
                              conf,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    
    sqlite3_close(db);
    return true;
}

sqlite3* Controller::openDB()
{
    sqlite3 *db;
    struct stat st = {0};
    std::string jcFolderPath = std::string(getenv("HOME")) + "/Library/Application Support/jc";
    if (stat(jcFolderPath.c_str(), &st) == -1)
    {
        std::cout << "Creating jc folder in " << getenv("HOME") << "/Application Support\n";
        mkdir(jcFolderPath.c_str(), 0700);
    }
    int result = sqlite3_open((jcFolderPath+"/jc.db").c_str(), &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return nullptr;
    }
    return db;
}

bool Controller::checkAppTableExist(sqlite3 *db)
{
    char *errorMessage = 0;
    bool tableExists = false;
    int result = sqlite3_exec(db,
                              "SELECT name FROM sqlite_master WHERE type='table' AND name='app';",
                              [](void *tableExists, int argc, char **argv, char **azColName)
                              {
                                  *(bool*)tableExists = true;
                                  return 0;
                              },
                              &tableExists,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              "CREATE TABLE app("  \
                              "ID          TEXT PRIMARY KEY  NOT NULL," \
                              "NAME        TEXT UNIQUE       NOT NULL," \
                              "VERSION     INT               NOT NULL);",
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    return true;
}

bool Controller::checkDeviceTableExist(sqlite3 *db)
{
    char *errorMessage = 0;
    bool tableExists = false;
    int result = sqlite3_exec(db,
                              "SELECT name FROM sqlite_master WHERE type='table' AND name='device';",
                              [](void *tableExists, int argc, char **argv, char **azColName)
                              {
                                  *(bool*)tableExists = true;
                                  return 0;
                              },
                              &tableExists,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              "CREATE TABLE device("  \
                              "ID          TEXT PRIMARY KEY  NOT NULL," \
                              "NAME        TEXT UNIQUE       NOT NULL," \
                              "MODEL       TEXT              NOT NULL," \
                              "VERSION     TEXT              NOT NULL);",
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    return true;
}

bool Controller::checkInstallTableExist(sqlite3 *db)
{
    char *errorMessage = 0;
    bool tableExists = false;
    int result = sqlite3_exec(db,
                              "SELECT name FROM sqlite_master WHERE type='table' AND name='install';",
                              [](void *tableExists, int argc, char **argv, char **azColName)
                              {
                                  *(bool*)tableExists = true;
                                  return 0;
                              },
                              &tableExists,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(false)
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              "CREATE TABLE install("  \
                              "APP          TEXT    NOT NULL," \
                              "DEVICE       TEXT    NOT NULL," \
                              "VERSION      INT     NOT NULL," \
                              "PRIMARY KEY(APP, DEVICE));",
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    return true;
}