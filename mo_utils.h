#ifndef __mo_utils_
#define __mo_utils_

#include "mo.h"

//  跳过空白
MO_EXTERN   mo_byte*            mo_lex_skipspace    (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  加载更多数据（pc为加载数据前pos应该处于什么位置，返回值为加载数据之后的poc的位置）
MO_EXTERN   mo_byte*            mo_lex_load_more    (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  从pc的位置开始重新定位识别数据的起始位置
MO_EXTERN   mo_byte*            mo_lex_locate       (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  遇到新的一行(pc应该总是指向\n字符所在的位置)
MO_EXTERN   mo_byte*            mo_lex_newline      (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  单行注释
MO_EXTERN   mo_byte*            mo_lex_singleline_comment   (struct lex_t* x, struct result_t* r, mo_byte* pc, int pervsize, mo_byte escape_newline);

//  从本地文件,字符串创建流对象
MO_EXTERN   struct stream_t*    mo_stream_file_new  (const char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new(char* str, int size, int auto_free);


#endif//__mo_utils_




