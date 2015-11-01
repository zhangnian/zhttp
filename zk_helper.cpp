#include "zk_helper.h"

#include <errno.h>



namespace zhttp
{
    static ZkHelper ins_;

    void g_zk_watcher(zhandle_t*, int type, int state, const char* path, void* ctx)
    {
        if( type == ZOO_SESSION_EVENT )
        {
            if( state == ZOO_CONNECTED_STATE )
                printf("连接到zookeeper成功\n");
            if( state == ZOO_EXPIRED_SESSION_STATE )
                printf("连接到zookeeper失败\n");
        }
    }

    void acreate_completion(int rc, const char* value, const void* data)
    {
        if( rc == 0 )
            printf("服务注册成功，服务地址: %s\n", (char*)data);
        else
        {
            if( rc == ZNODEEXISTS )
            {
                printf("服务已存在，注册失败\n");
            }
            else
            {
                printf("服务注册失败, rc: %d\n", rc);
            }
        }

    }

    ZkHelper* ZkHelper::instance()
    {
        return &ins_;
    }

    int ZkHelper::registe_service(const std::string& sevice_name, const std::string& endpoint)
    {
        zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
        zk_ = zookeeper_init("127.0.0.1:2181", &g_zk_watcher, 1000, 0, 0, 0);
        if( NULL == zk_ )
        {
            printf("zookeeper_init error\n");
            return errno;
        }

        int flag = ZOO_EPHEMERAL;
        int rc = zoo_acreate(zk_, sevice_name.c_str(), endpoint.c_str(), endpoint.size(),
                            &ZOO_OPEN_ACL_UNSAFE, flag, acreate_completion, endpoint.c_str());
        return rc;
    }
}
