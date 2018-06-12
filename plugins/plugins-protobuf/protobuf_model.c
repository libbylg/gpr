#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "mo.h"
#include "protobuf_lex.h"



int main(int argc, char* args[])
{
    struct mo_t* mo = mo_new();
    mo_reg_lex(mo, protobuf_lex_new(32* 1024));



    return 0;
}

