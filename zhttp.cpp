#include "zhttp.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>


#include "request.h"
#include "response.h"

#include <evhttp.h>

void evhttp_handler(struct evhttp_request* request, void* data)
{
    zhttp::ZHttpApp* app = static_cast<zhttp::ZHttpApp*>(data);
    assert( NULL != app );

    zhttp::Request req;
    int ret = req.ParseRequest(request);
    if( 0 != ret )
        return;

    zhttp::Response res(request);

    // callback
    zhttp::UriHandlersMap::const_iterator iter = app->GetUriHandlers().find(req.GetUrl());
    if( iter != app->GetUriHandlers().end() )
    {
        iter->second(req, res);
    }
}



namespace zhttp
{
    ZHttpApp::ZHttpApp()
    {

    }


    ZHttpApp& ZHttpApp::route(const std::string& uri, const UriHandler& handler)
    {
        UriHandlersMap::const_iterator iter = uri_handlers_.find(uri);
        if( iter != uri_handlers_.end() )
        {
            uri_handlers_[uri] = handler;
        }
        else
        {
            uri_handlers_.insert(std::pair<std::string, UriHandler>(uri, handler));
        }

        assert( uri_handlers_.size() > 0 );
        return *this;
    }


    ZHttpApp& ZHttpApp::listen(const std::string& ip, uint16_t port)
    {
        assert( ip.size() > 0 && port > 0 );

        ip_ = ip;
        port_ = port;
        return *this;
    }


    void ZHttpApp::run(int nthreads)
    {
        // 主线程监听
        int fd = bind_socket();
        if( fd <= 0 )
            return;

        pthread_t tids[nthreads];
        for(int i = 0; i < nthreads; ++i)
        {
            struct event_base* event_base = event_init();
            assert( NULL != event_base );

            struct evhttp* httpd = evhttp_new(event_base);
            if( NULL == httpd )
            {
                event_base_free(event_base);
                return;
            }

            UriHandlersMap::const_iterator iter = uri_handlers_.begin();
            for(; iter != uri_handlers_.end(); ++iter)
            {
                evhttp_set_cb(httpd, iter->first.c_str(), &evhttp_handler, this);
            }

            // 工作线程accept同一个listen fd
            int ret = evhttp_accept_socket(httpd, fd);
            if( 0 != ret )
            {
                evhttp_free(httpd);
                event_base_free(event_base);
                return;
            }

            ret = pthread_create(&tids[i], NULL, &ZHttpApp::Dispatch, event_base);
            if( 0 != ret )
            {
                printf("pthread_create error, ret: %d\n", ret);
                evhttp_free(httpd);
                event_base_free(event_base);
                return;
            }
        }


        for (int i = 0; i < nthreads; i++)
        {
            pthread_join(tids[i], NULL);
        }
    }

    int ZHttpApp::bind_socket()
    {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        assert( fd > 0 );

        int on = 1;
        int ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
        if( 0 != ret )
            return ret;

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(ip_.c_str());
        addr.sin_port = htons(port_);

        ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
        if( 0 != ret )
            return ret;

        ret = ::listen(fd, 10240);
        if( 0 != ret )
            return ret;

        int flags;
         if ((flags = fcntl(fd, F_GETFL, 0)) < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
            return -1;

        return fd;
    }

    void* ZHttpApp::Dispatch(void* arg)
    {
        struct event_base* base = static_cast<struct event_base*>(arg);
        event_base_dispatch(base);
        return NULL;
    }
}
