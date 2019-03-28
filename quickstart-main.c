#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mo.h"
#include "mo_utils.h"








static mo_token  demo_lex_next(void* ctx, struct lex_t* x, struct token_t* t, struct result_t* r)
{
    mo_byte* pc = x->pos;
    while (1) {
        //  跳过空白和空行并确保当前位置为有效识别字符
        pc = mo_lex_locate(x, r, pc);

        switch (*pc) {
            case '\n':
                if (pc == x->end) { return MO_TOKEN_EOF; }
                return mo_result_errorf(r, 112, "");

        }
    }
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

