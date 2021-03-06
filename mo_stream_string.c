#include "mo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stream_string_t
{
    struct stream_t base;
    char*           data;
    int             auto_free;
    char*           end;
    char*           r;
};


static void stream_string_del(void* obj)
{
    struct stream_string_t* stream = (struct stream_string_t*)(obj);
    if (stream->auto_free) {
        if (NULL != stream->data) {
            free(stream->data);
        }
    }

    free(stream);
}

static int  stream_string_read(struct stream_t* m, char** pos, char* end)
{
    struct stream_string_t* stream = (struct stream_string_t*)(m);
    int cache_size   = end - *pos;
    int remain_size  = stream->end - stream->r;
    if (0 == remain_size)
    {
        return MO_READ_EOF;
    }

    int copy_len = ((cache_size < remain_size)?cache_size:remain_size);
    memcpy(*pos, stream->data, (size_t)copy_len);

    *pos += copy_len;
    return MO_READ_OK;
}


MO_EXTERN   struct stream_t*   mo_stream_string_new(char* str, int size, int auto_free)
{
    struct stream_string_t* stream = (struct stream_string_t*)(malloc(sizeof(struct stream_string_t)));
    stream->base.prev = stream;
    stream->base.typeid = mo_define_class("stream_string_t", stream_string_del);
    stream->base.read = stream_string_read;
    stream->base.anchor.prev = &(stream->base.anchor);
    stream->base.anchor.typeid = mo_typeid_of("anchor");
    stream->base.anchor.name = strdup("string");
    stream->base.anchor.line = NULL;
    stream->base.anchor.lino = 0;
    stream->data = str;
    stream->auto_free = auto_free;

    stream->end = str + size;
    stream->r = str;

    return (struct stream_t*)stream;
}

