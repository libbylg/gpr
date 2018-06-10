#include "mo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stream_file_t
{
    struct stream_t base;
    FILE*           file;
    char*           filename;
};


static void mo_stream_file_del(void* obj)
{
    struct stream_file_t* stream = (struct stream_file_t*)(obj);
    if (NULL != stream->file)
    {
        fclose(stream->file);
    }
    if (NULL != stream->filename)
    {
        free(stream->filename);
    }

    free(stream);
}


static int  mo_stream_file_read(struct stream_t* m, char** pos, char* end)
{
    struct stream_file_t* stream = (struct stream_file_t*)(m);
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
    return MO_READ_OK;
}


MO_EXTERN   struct stream_t*   mo_stream_file_new(const char* filename)
{
    size_t filename_len = strlen(filename);
    char* p = (char*)malloc(filename_len + 1);
    if (NULL != p)
    {
        return NULL;
    }

    FILE* file = fopen(file, "r");
    if (NULL == file)
    {
        free(p);
        return NULL;
    }

    struct stream_file_t* stream = (struct stream_file_t*)(malloc(sizeof(struct stream_file_t)));
    stream->base.size        = sizeof(struct stream_file_t);
    stream->base.del         = mo_stream_file_del;
    stream->base.read        = mo_stream_file_read;
    stream->file             = file;
    memcpy(stream->filename, filename, filename_len + 1);

    return (struct stream_t*)stream;
}


