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
#define RETURN_ON_SQL_ERROR if( result != SQLITE_OK ) { \
fprintf(stderr, "SQL error: %s\n", errorMessage);\
sqlite3_free(errorMessage);\
sqlite3_close(db);\
return false; }


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
    RETURN_ON_SQL_ERROR
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              ("CREATE TABLE app" + App::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR
        std::cout << "Table created";
    }
    
    App app(appName, appIdentifier, version);
    
    result = sqlite3_exec(db,
                          app.getInsertSQL("app").c_str(),
                          [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                          0,
                          &errorMessage);
    RETURN_ON_SQL_ERROR
    
    sqlite3_close(db);
    std::cout << "Added app " << app.getName() << "\n";
    return true;
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
    RETURN_ON_SQL_ERROR
    
    if(!tableExists)
    {
        result = sqlite3_exec(db,
                              ("CREATE TABLE configuration" + Configuration::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR
        
        result = sqlite3_exec(db,
                              conf->getInsertSQL("configuration").c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR
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
        RETURN_ON_SQL_ERROR
    }
    sqlite3_close(db);
    return true;
}