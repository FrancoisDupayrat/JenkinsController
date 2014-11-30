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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    bool tableExists = false;
    result = sqlite3_exec(db,
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
    
    App app(appName, appIdentifier, version);
    
    result = sqlite3_exec(db,
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    result = sqlite3_exec(db,
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    result = sqlite3_exec(db,
                          ("DELETE FROM app " + App::getDeleteSQL(appName)).c_str(),
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return registeredApps;
    }
    
    result = sqlite3_exec(db,
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
    
    
    return registeredApps;
}

bool Controller::addDevice(std::string deviceName, std::string deviceIdentifier, std::string model, std::string osVersion)
{
    sqlite3 *db;
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    bool tableExists = false;
    result = sqlite3_exec(db,
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
                              ("CREATE TABLE device" + Device::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR(false)
    }
    
    Device device(deviceName, deviceIdentifier, model, osVersion);
    
    result = sqlite3_exec(db,
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    result = sqlite3_exec(db,
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    result = sqlite3_exec(db,
                          ("DELETE FROM device " + Device::getDeleteSQL(deviceName)).c_str(),
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
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return registeredDevices;
    }
    
    result = sqlite3_exec(db,
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
    
    
    return registeredDevices;
}

bool Controller::loadConfiguration()
{
    sqlite3 *db;
    char *errorMessage = 0;
    int result = sqlite3_open("jc.db", &db);
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    
    if(conf != nullptr)
    {
        delete(conf);
        conf = nullptr;
    }
    conf = new Configuration();
    
    bool tableExists = false;
    result = sqlite3_exec(db,
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