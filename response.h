#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>

#include <evhttp.h>


namespace zhttp
{
    class Response
    {
    public:
        enum ContentType
        {
            kJSON,
            kXML,
            kTEXT,
        };

        Response(struct evhttp_request* req)
            : req_(req)
        {

        }

        void set_content_type(ContentType type)
        {
            // Content-Type : text/html; charset=utf-8
            if( type == kJSON )
                add_header("Content-Type", "application/json; charset=utf-8");
            else if(type == kXML )
                add_header("Content-Type", "application/xml; charset=utf-8");
            else if(type == kTEXT )
                add_header("Content-Type", "text/html; charset=utf-8");
        }

        void add_header(const std::string& key, const std::string& val)
        {
            evhttp_add_header(req_->output_headers, key.c_str(), val.c_str());
        }

        void writeJson(const std::string& data)
        {
            set_content_type(kJSON);
            write(data);
        }

        void write(const std::string& data)
        {
            struct evbuffer* buf = evhttp_request_get_output_buffer(req_);
            assert( NULL != buf );
            evbuffer_add_printf(buf, "%s", data.c_str());
            evhttp_send_reply(req_, HTTP_OK, "OK", buf);
        }

    private:
        struct evhttp_request* req_;
    };
}


#endif
