#ifndef ZHTTP_H
#define ZHTTP_H

#include <map>

#include <boost/function.hpp>
#include <boost/bind.hpp>

#include "request.h"
#include "response.h"


namespace zhttp
{
    typedef boost::function<void(Request& req, Response& res)> UriHandler;
    typedef std::map<std::string, UriHandler> UriHandlersMap;

    class ZHttpApp
    {
    public:
        ZHttpApp();

        /**
         * 设置URI映射到的处理函数
         */
        ZHttpApp& route(const std::string& uri, const UriHandler& handler);

        /**
         * 监听
         */
         ZHttpApp& listen(const std::string& ip, uint16_t port);

         /**
          * 开始事件循环
          */
         void run(int nthreads = 1);

    public:
        inline UriHandlersMap& GetUriHandlers()
        {
            return uri_handlers_;
        }

    private:
        int bind_socket(uint16_t port);

        static void* Dispatch(void* arg);

    private:
        UriHandlersMap uri_handlers_;
        std::string ip_;
        uint16_t port_;

    };
}


#endif
