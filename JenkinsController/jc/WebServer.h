//
//  WebServer.h
//  JenkinsController
//
//  Created by François Dupayrat on 02/12/14.
//  Copyright (c) 2014 Auticiel. All rights reserved.
//

#ifndef __JenkinsController__WebServer__
#define __JenkinsController__WebServer__

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include "microhttpd.h"

/*Requires gnustl and libgcrypt to compile. microhttpd universal binary is included
 On mac, use port to install them :
 sudo port install libmicrohttpd +universal
 sudo port install gnustl +universal
 sudo port install libgcryt +universal
 They will be installed on opt/local/lib, which is already in the library search path
 */
class WebServer
{
public:
    static bool startServer(int port);
    static void stopServer();
    static int answer_to_connection (void *cls, struct MHD_Connection *connection,
                                     const char *url,
                                     const char *method, const char *version,
                                     const char *upload_data,
                                     size_t *upload_data_size, void **con_cls);
private:
    static WebServer* getInstance();
    WebServer();
    struct MHD_Daemon *daemon;
    int port;
};

#endif /* defined(__JenkinsController__WebServer__) */
