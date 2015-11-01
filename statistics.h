#ifndef STATISTICS_H
#define STATISTICS_H

#include "atomic.h"

namespace zhttp
{
    /**
     * 统计指标
     */
    struct Statistics
    {
        int start_ts_;
        AtomicInt64 total_req_;
        AtomicInt64 total_res_;
    };

}


#endif
