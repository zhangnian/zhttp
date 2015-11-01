#ifndef ZK_HELPER_H
#define ZK_HELPER_H

#include <string>

#include <zookeeper/zookeeper.h>
#include <zookeeper/zookeeper_log.h>

namespace zhttp
{
    #define ZK_INS ZkHelper::instance()

    class ZkHelper
    {
    public:
        static ZkHelper* instance();

        int registe_service(const std::string& sevice_name, const std::string& endpoint);

    private:
        zhandle_t* zk_;
    };
}


#endif
