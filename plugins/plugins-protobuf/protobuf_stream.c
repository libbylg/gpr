#include "protobuf_stream.h"


static void protobuf_stream_del(void* obj)
{
    struct protobuf_stream_t* stream = (struct protobuf_stream_t*)(obj);
    if (NULL != stream->file)
    {
        fclose(stream->file);
    }
    if (NULL != stream->filename)
    {
        free(stream->filename);
    }
    if (NULL != stream->cache)
    {
        free(stream->cache);
    }

    free(stream);
}

static int  protobuf_stream_read(struct stream_t* m, char** pos, char* end)
{
    struct protobuf_stream_t* stream = (struct protobuf_stream_t*)(m);
    size_t size = fread(*pos, 1, end - *pos, stream->file);
    if (0 == size)
    {
        if (feof(stream->file))
        {
            return MO_READ_EOF;
        }

        if (ferror(stream->file))
        {
            return MO_READ_ERROR;
        }

        return MO_READ_ERROR;
    }

    *pos += size;
    return Mo_READ_OK;
}

MO_EXTERN   struct protobuf_stream_t*   protobuf_stream_new(int init_cache_size, char* filename)
{
    FILE* file = fopen(file, "r");
    if (NULL == file)
    {
        return NULL;
    }

    int filename_len = malloc(strlen(filename);

    struct protobuf_stream_t* stream = (struct protobuf_stream_t*)(malloc(sizeof(struct protobuf_stream_t)));
    stream->size        = sizeof(struct protobuf_stream_t);
    stream->cache       = (char*)malloc(sizeof(char) * init_cache_size);
    stream->del         = protobuf_stream_del;
    stream->read        = protobuf_stream_read;
    stream->filename    = (char*)malloc(filename_len);
    memcpy(stream->filename, filename, (filename_len + 1));
    stream->file        = file;

    return stream;
}

