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
    loadConfiguration();
}

//Helper define to avoid repeating the error checking
#define RETURN_ON_SQL_ERROR if( result != SQLITE_OK ) { \
fprintf(stderr, "SQL error: %s\n", errorMessage);\
sqlite3_free(errorMessage);\
sqlite3_close(db);\
return false; }

bool Controller::loadConfiguration()
{
    sqlite3 *db;
    char *errorMessage = 0;
    
    if(conf != nullptr)
    {
        delete(conf);
        conf = nullptr;
    }
    conf = new Configuration();
    
    int result = sqlite3_open("jc.db", &db);
    
    if(result)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    bool hasResult = false;
    result = sqlite3_exec(db,
                          "SELECT name FROM sqlite_master WHERE type='table' AND name='configuration';",
                          [](void *hasResult, int argc, char **argv, char **azColName)
                          {
                              *(bool*)hasResult = true;
                              return 0;
                          },
                          &hasResult,
                          &errorMessage);
    
    RETURN_ON_SQL_ERROR
    
    if(!hasResult)
    {
        result = sqlite3_exec(db,
                              ("CREATE TABLE configuration" + Configuration::getTableFormat()).c_str(),
                              [](void *hasResult, int argc, char **argv, char **azColName) { return 0; },
                              0,
                              &errorMessage);
        RETURN_ON_SQL_ERROR
        
        result = sqlite3_exec(db,
                              ("INSERT INTO configuration (ID, URL, LOCAL, VERSION)"  \
                              "VALUES " + conf->getInsertSQL()).c_str(),
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