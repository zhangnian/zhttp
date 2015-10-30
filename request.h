#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>

#include <evhttp.h>

#define CHECK_RET_ZERO(ret) \
    if( 0 != ret ){ \
        return ret; \
    }



namespace zhttp
{
    /**
     * HTTP请求对象
     */
    class Request
    {
        const static size_t kMaxBufSize = 1024 * 16;
    public:
        enum method_t
        {
            UNKNOWN,
            GET,
            POST
        };

        Request():
            method_(UNKNOWN)
        {
        }

        ~Request()
        {
            evhttp_clear_headers(&params_);
        }

        inline method_t GetMethod()
        {
            return method_;
        }

        inline std::string GetFullUrl()
        {
            return full_url_;
        }

        inline std::string GetUrl()
        {
            return url_;
        }

        int ParseRequest(struct evhttp_request* req)
        {
            assert( NULL != req );

            // 取出url
            const char* request_uri = evhttp_request_uri(req);
            assert( NULL != request_uri );
            full_url_ = request_uri;
            url_ = full_url_;
            size_t idx = full_url_.find("?");
            if( idx != std::string::npos )
            {
                url_ = full_url_.substr(0, idx);
            }

            // 取出请求方式
            evhttp_cmd_type type = req->type;
            if( type == EVHTTP_REQ_GET)
            {
                method_ = Request::GET;
            }
            else if( type == EVHTTP_REQ_POST )
            {
                method_ = Request::POST;
            }

            // 取出查询字符串
            int ret = evhttp_parse_query(request_uri, &params_);
            CHECK_RET_ZERO(ret);

            if( method_ == Request::POST )
            {
                static char buf[kMaxBufSize] = { 0 };
                size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
                if (post_size > 0 )
                {
                    size_t copy_size = post_size > kMaxBufSize ? kMaxBufSize : post_size;
                    memcpy(buf, EVBUFFER_DATA(req->input_buffer), copy_size);
                    buf[copy_size] = '\0';

                    body_.assign(buf, copy_size);
                    assert( body_.size() == copy_size );
                }
            }

            return 0;
        }

        std::string GetStr(const std::string& name, const std::string& default_val)
        {
            const char* val = evhttp_find_header(&params_, name.c_str());
            if( NULL == val )
                return default_val;
            return std::string(val);
        }

        int GetInt(const std::string& name, int default_val)
        {
            const char* val = evhttp_find_header(&params_, name.c_str());
            if( NULL == val )
                return default_val;
            return atoi(val);
        }

        inline std::string GetBody()
        {
            return body_;
        }

    private:
        std::string url_;
        std::string full_url_;
        std::string body_;
        method_t method_;
        struct evkeyvalq params_;
    };
}



#endif
