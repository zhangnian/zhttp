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
        static std::string MakeResponse(int code, const std::string& msg)
        {
            std::map<std::string, std::string> data;
            return MakeResponse(code, msg, data);
        }

        static std::string MakeResponse(int code, const std::string& msg, std::map<std::string, std::string>& data)
        {
            Document doc;
            Document::AllocatorType& allocator = doc.GetAllocator();

            Value root(kObjectType);

            Value field_code(code);
            Value field_msg(msg.c_str(), msg.size());
            Value field_data(kObjectType);

            root.AddMember("code", field_code, allocator);
            root.AddMember("message", field_msg, allocator);

            std::map<std::string, std::string>::iterator iter = data.begin();
            for(; iter != data.end(); ++iter)
            {
                const std::string& key =iter->first;
                const std::string& val = iter->second;

                Value field_k(key.c_str(), key.size());
                Value field_v(val.c_str(), val.size());

                field_data.AddMember(field_k, field_v, allocator);
            }

            root.AddMember("data", field_data, allocator);

            StringBuffer buf;
            Writer<StringBuffer> writer(buf);
            root.Accept(writer);

            return buf.GetString();
        }
    };
}


#endif
