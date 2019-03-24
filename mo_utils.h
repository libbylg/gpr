#ifndef __mo_utils_
#define __mo_utils_

#include "mo.h"

//  �����հ�
MO_EXTERN   mo_byte*            mo_lex_skipspace    (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ���ظ������ݣ�pcΪ��������ǰposӦ�ô���ʲôλ�ã�����ֵΪ��������֮���poc��λ�ã�
MO_EXTERN   mo_byte*            mo_lex_load_more    (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ��pc��λ�ÿ�ʼ���¶�λʶ�����ݵ���ʼλ��
MO_EXTERN   mo_byte*            mo_lex_locate       (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  �����µ�һ��(pcӦ������ָ��\n�ַ����ڵ�λ��)
MO_EXTERN   mo_byte*            mo_lex_newline      (struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ����ע��
MO_EXTERN   mo_byte*            mo_lex_singleline_comment   (struct lex_t* x, struct result_t* r, mo_byte* pc, int pervsize, mo_byte escape_newline);

//  �ӱ����ļ�,�ַ�������������
MO_EXTERN   struct stream_t*    mo_stream_file_new  (const char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new(char* str, int size, int auto_free);


#endif//__mo_utils_




