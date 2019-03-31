#ifndef __mo_utils_
#define __mo_utils_

#include "mo.h"

//  �ӱ����ļ�,�ַ�������������
MO_EXTERN   struct stream_t*    mo_stream_file_new  (const char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new(char* str, int size, int auto_free);

//  �����հ�
MO_EXTERN   mo_byte*            mo_lex_skipspace(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ���ظ������ݣ�pcΪ��������ǰposӦ�ô���ʲôλ�ã�����ֵΪ��������֮���poc��λ�ã�
MO_EXTERN   mo_byte*            mo_lex_loadmore(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ��pc��λ�ÿ�ʼ���¶�λʶ�����ݵ���ʼλ��
MO_EXTERN   mo_byte*            mo_lex_locate(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  �����µ�һ��,��lex�Զ������ڲ�������ص�����(pcӦ������ָ��\n�ַ����ڵ�λ��)
MO_EXTERN   mo_byte*            mo_lex_newline(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ��pc��λ�ö�λ�����еĿ�ʼ����ͨ��ʹ�ø���������ע��
MO_EXTERN   mo_byte*            mo_lex_skipline(struct lex_t* x, struct result_t* r, mo_byte* pc);

//  ��ȡһ��
MO_EXTERN   struct token_t*     mo_lex_accept_number(struct lex_t* x, struct token_t* k, struct result_t* r);

MO_EXTERN   struct token_t*     mo_lex_accept_string(struct lex_t* x, struct token_t* k, struct result_t* r);

#endif//__mo_utils_




