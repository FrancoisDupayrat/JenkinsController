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


Controller::Controller()
{
    conf = nullptr;
    if(!loadConfiguration())
    {
        throw;
    }
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
    
    App app(appName, appIdentifier, version);
    
    int result = sqlite3_exec(db,
                              app.getInsertSQL("app").c_str(),
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
                              ("UPDATE app " + App::getUpdateVersionSQL(appName, version)).c_str(),
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
                              ("DELETE FROM app " + App::getWhereSQL(appName) + ";").c_str(),
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
                                  registeredApps->push_back(App(argc, argv));
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
                              ("SELECT * FROM app " + App::getWhereSQL(name) + ";").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<App>* matchingApp = (std::vector<App>*)ptr;
                                  matchingApp->push_back(App(argc, argv));
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
    
    Device device(deviceName, deviceIdentifier, model, osVersion);
    
    int result = sqlite3_exec(db,
                              device.getInsertSQL("device").c_str(),
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
                              ("UPDATE device " + Device::getUpdateVersionSQL(deviceName, osVersion)).c_str(),
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
                              ("DELETE FROM device " + Device::getWhereSQL(deviceName) + ";").c_str(),
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
                                  registeredDevices->push_back(Device(argc, argv));
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
                              ("SELECT * FROM device " + Device::getWhereSQL(name) + ";").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Device>* matchingDevice = (std::vector<Device>*)ptr;
                                  matchingDevice->push_back(Device(argc, argv));
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
                              ("SELECT * FROM install " + Install::getWhereSQL(appName, deviceName) + ";").c_str(),
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
                              ("UPDATE install " + Install::getUpdateVersionSQL(appName, deviceName, version)).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    else
    {
        Install install(app, getDevice(deviceName), version);
        result = sqlite3_exec(db,
                              install.getInsertSQL("install").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
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
                              ("DELETE FROM install " + Install::getWhereSQL(appName, deviceName) + ";").c_str(),
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
                              ("SELECT * FROM install " + Install::getAllAppWhereSQL(appName) + ";").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Install>* installs = (std::vector<Install>*)ptr;
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
                              ("SELECT * FROM install " + Install::getAllDeviceWhereSQL(deviceName) + ";").c_str(),
                              [](void *ptr, int argc, char **argv, char **azColName)
                              {
                                  std::vector<Install>* installs = (std::vector<Install>*)ptr;
                                  installs->push_back(Install(argc, argv));
                                  return 0;
                              },
                              &installs,
                              &errorMessage);
    RETURN_ON_SQL_ERROR(installs)
    
    sqlite3_close(db);
    return installs;
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
                              ("CREATE TABLE configuration" + Configuration::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
        
        result = sqlite3_exec(db,
                              conf->getInsertSQL("configuration").c_str(),
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
                                  ((Configuration*)conf)->loadFromSQL(argc, argv);
                                  return 0;
                              },
                              conf,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    
    sqlite3_close(db);
    return true;
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
                              ("CREATE TABLE app" + App::getTableFormat()).c_str(),
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
                              ("CREATE TABLE install" + Device::getTableFormat()).c_str(),
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
                              ("CREATE TABLE install" + Install::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    return true;
}