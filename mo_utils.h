#ifndef __mo_utils_
#define __mo_utils_

#include "mo.h"

//  从本地文件,字符串创建流对象
MO_EXTERN   struct stream_t*    mo_stream_file_new  (const char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new(char* str, int size, int auto_free);

//  跳过空白
MO_EXTERN   mo_byte*            mo_lex_skipspace(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  加载更多数据（pc为加载数据前pos应该处于什么位置，返回值为加载数据之后的poc的位置）
MO_EXTERN   mo_byte*            mo_lex_loadmore(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  从pc的位置开始重新定位识别数据的起始位置
MO_EXTERN   mo_byte*            mo_lex_locate(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  遇到新的一行,让lex自动处理内部与行相关的数据(pc应该总是指向\n字符所在的位置)
MO_EXTERN   mo_byte*            mo_lex_newline(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  从pc的位置定位到新行的开始处，通常使用该能力处理注释
MO_EXTERN   mo_byte*            mo_lex_skipline(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  收取一个
MO_EXTERN   struct token_t*     mo_lex_accept_number(struct lex_t* x, struct token_t* k, struct result_t* r);

MO_EXTERN   struct token_t*     mo_lex_accept_string(struct lex_t* x, struct token_t* k, struct result_t* r);

#endif//__mo_utils_




