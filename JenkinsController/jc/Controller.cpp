//
//  Controller.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 29/11/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "Controller.h"

USING_NS_JC;
using namespace std;

Controller* Controller::createController()
{
    Controller* controller = new Controller();
    if(!controller->loadConfiguration())
    {
        delete(controller);
        return nullptr;
    }
    return controller;
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
                              ("INSERT INTO configuration (ID, URL, LOCAL, VERSION) VALUES (1, '" + conf->getRemote() + "', 1, " CONFIGURATION_VERSION ");").c_str(),
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
                                  if(atoi(argv[2]))
                                  {
                                      ((Configuration*)conf)->setRemote(std::string(argv[1]));
                                  }
                                  else
                                  {
                                      ((Configuration*)conf)->setLocal();
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
    int result = sqlite3_open("jc.db", &db);
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