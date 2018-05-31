#ifndef __protobuf_stream_
#define __protobuf_stream_

#include "mo.h"

struct protobuf_stream_t
{
    struct stream_t base;
    FILE*           file;
    char*           filename;
};


MO_EXTERN   struct protobuf_stream_t*   protobuf_stream_new(int init_cache_size);

#endif//__protobuf_stream_
