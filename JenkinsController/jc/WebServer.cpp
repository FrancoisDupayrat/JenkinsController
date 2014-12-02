//
//  WebServer.cpp
//  JenkinsController
//
//  Created by François Dupayrat on 02/12/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#include "WebServer.h"
#include <string.h>

static WebServer* _instance = NULL;

bool WebServer::startServer(int port)
{
    getInstance()->port = port;
    getInstance()->daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, port, NULL, NULL,
                               &WebServer::answer_to_connection, NULL, MHD_OPTION_END);
    return getInstance()->daemon != NULL;
}

void WebServer::stopServer()
{
    if(getInstance()->daemon != NULL)
    {
        MHD_stop_daemon (getInstance()->daemon);
        getInstance()->daemon = NULL;
    }
}

int WebServer::answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    const char *page  = "<html><body>Welcome to JC Web interface. It is not implemented yet!</body></html>";
    struct MHD_Response *response;
    int ret;
    
    response = MHD_create_response_from_buffer (strlen (page),
                                                (void*) page, MHD_RESPMEM_PERSISTENT);
    
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    
    return ret;
}

WebServer* WebServer::getInstance()
{
    if (!_instance)
    {
        _instance = new WebServer();
    }
    return _instance;
}

WebServer::WebServer()
{
    daemon = NULL;
}