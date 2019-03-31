#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mo.h"
#include "mo_utils.h"





static struct token_t*  demo_lex_next(void* ctx, struct lex_t* x, struct token_t* k, struct result_t* r)
{
    mo_byte* pc = x->pos;
    while (1) {
        //  跳过空白和空行并确保当前位置为有效识别字符
        pc = mo_lex_locate(x, r, pc);

        switch (*pc) {
            case '\n':
                if (pc == x->end) {
                    return MO_TOKEN_EOF;
                }
                return mo_token_errorf(k, r, 112, "");
            case '/':
                if (('/' == pc[1]) || ('*' == pc[1])) {
                    mo_lex_skipcomment(x, r, pc);
                    continue;
                }
                return mo_token_errorf(k, r, 111, "不支持的符号");
            case '"':
            case '\'':
                return mo_lex_accept_string(x, k, r);
            case '+':
            case '-':
            case ',':
            case '.':
            case '<':
            case '>':
            case '=':
            case ';':
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
                x->pos = pc + 1;
                return mo_token_as(k, x->pos, NULL, NULL);
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                return mo_lex_accept_number(x, k, r);
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                return mo_accept_name(x, k, r);
            default:    //'!' '#' '$' '%' '&' '*' ':' '?' '@' '~' '|' '\\' '^' '_' '`'
                return mo_token_errorf(k, r, 111, "不支持的符号");
        }
    }
    return MO_TOKEN_UNKNOWN;
}

int main(int argc, char* args[])
{
    struct lex_t*  lex  = mo_lex_new(NULL, demo_lex_next, 1024);
    struct stream_t* stream = mo_stream_file_new("F:\\dev\\github.com\\libbylg\\mo\\test\\test1.proto");
    mo_lex_push_stream(lex, stream);

    struct token_t* k = mo_token_new();
    struct result_t* r = mo_result_new();
    k = mo_lex_next_token(lex, k, r);

    while (mo_result_ok(r)) {
        printf("[%d]\n", k->id);
    }

    return 0;
}

