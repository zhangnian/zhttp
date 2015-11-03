#ifndef JSON_UTILS_H
#define JSON_UTILS_H


#include <string>
#include <map>

#include <document.h>
#include <prettywriter.h>

using namespace rapidjson;


namespace zhttp
{
    class JsonUtils
    {
    public:
        static std::string MakeResponse(int status, const std::string& info)
        {
            std::map<std::string, std::string> data;
            return MakeResponse(status, info, data);
        }

        static std::string MakeResponse(int status, const std::string& info, std::map<std::string, std::string>& data)
        {
            Document doc;
            Document::AllocatorType& allocator = doc.GetAllocator();

            Value root(kObjectType);

            Value field_header(kObjectType);
            Value field_status(status);
            Value field_info(info.c_str(), info.size());

            field_header.AddMember("status", field_status, allocator);
            field_header.AddMember("info", field_info, allocator);

            Value field_body(kObjectType);

            std::map<std::string, std::string>::iterator iter = data.begin();
            for(; iter != data.end(); ++iter)
            {
                const std::string& key =iter->first;
                const std::string& val = iter->second;

                Value field_k(key.c_str(), key.size());
                Value field_v(val.c_str(), val.size());

                field_body.AddMember(field_k, field_v, allocator);
            }

            root.AddMember("response_header", field_header, allocator);
            if( data.size() > 0 )
                root.AddMember("response", field_body, allocator);

            StringBuffer buf;
            Writer<StringBuffer> writer(buf);
            root.Accept(writer);

            return buf.GetString();
        }
    };
}


#endif
