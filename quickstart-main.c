#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mo.h"


static mo_token  demo_lex_next(void* ctx, struct compile_t*  p, struct token_t* t, struct result_t* r)
{

}

int main(int argc, char* args[])
{
    struct cache_t cache = {0};
    mo_cache_init(&cache, 1024, 32);

    struct lex_t*  lex  = mo_lex_new(NULL, demo_lex_next);
    struct sytx_t* sytx = mo_sytx_new(NULL, mo_sytx_drive_default);
    mo_cache_init(&cache, 1024, 32);
    mo_compile_new(NULL, )
    return 0;
}

