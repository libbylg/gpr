#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mo.h"


static mo_token  demo_lex_next(void* ctx, struct compile_t*  p, struct token_t* t, struct result_t* r)
{
    return MO_TOKEN_UNKNOWN;
}

int main(int argc, char* args[])
{
    struct lex_t*  lex  = mo_lex_new(NULL, demo_lex_next, 1024, 32);
    struct stream_t* stream = mo_stream_file_new("F:\\dev\\github.com\\libbylg\\mo\\test\\test1.proto");
    mo_lex_push_stream(lex, stream);

    struct token_t* k = mo_token_new();
    struct result_t* r = mo_result_new();
    mo_lex_next_token(lex, k, r);

    while (mo_result_ok(r)) {
        printf("[%d]\n", k->id);
    }

    return 0;
}

