#ifndef __mo_
#define __mo_




#if defined(__cplusplus)
#define MO_EXTERN  extern  "C"
#else
#define MO_EXTERN  extern
#endif//




#ifndef NULL
#if defined(__cplusplus)
#define NULL    (void*)(0)
#else
#define NULL    (0)
#endif
#endif




typedef int             mo_errno;
typedef unsigned int    mo_token;
typedef int             mo_action;




///!    语法分析后返回的对当前 token 的处理建议
#define     MO_ACTION_ERROR     (0)
#define     MO_ACTION_NEEDMORE  (1)
#define     MO_ACTION_TRYAGAIN  (2)




///!    几个内定的词法编号
#define     MO_TOKEN_ERROR      (0xFFFFFFFE)
#define     MO_TOKEN_EOF        (0xFFFFFFFF)




struct mo_t;
struct lex_t;
struct sytx_t;
struct unit_t;
struct feed_t;
struct result_t;
struct token_t;




typedef     void      (*MO_DEL_CALLBACK   )(void* obj);




///!    词法识别出来的符号
struct token_t
{
    int                 size;
    mo_token            token;      ///<    符号的编号
    char*               text[2];    ///<    符号的文本范围
};




///!    词法和语法解析结果
struct result_t
{
    int                 size;
    MO_DEL_CALLBACK     del;
    char                text[0];
};




///!    词法分析对象
typedef     mo_token  (*MO_NEXT_CALLBACK  )(struct lex_t*    x, struct token_t* t);
struct lex_t
{
    int                 size;
    struct stream_t*    stream_top;
    MO_DEL_CALLBACK     del;
    MO_NEXT_CALLBACK    next;
};




///!    输入流对象
typedef     int       (*MO_READ_CALLBACK  )(struct stream_t* m, char* buf, int* len);
struct stream_t
{
    int                 size;
    struct stream_t*    prev;
    MO_DEL_CALLBACK     del;
    MO_READ_CALLBACK    read;
};




///!    语法分析单元
typedef     mo_action (*MO_ACCEPT_CALLBACK)(struct unit_t*   u, struct token_t* t);
struct unit_t
{
    int                 size;
    struct unit_t*      prev;
    MO_DEL_CALLBACK     del;
    MO_ACCEPT_CALLBACK  accept;
};




MO_EXTERN   struct mo_t*        mo_new          ();
MO_EXTERN   void                mo_del          (struct mo_t* mo);
MO_EXTERN   void                mo_reg_lex      (struct mo_t* mo, struct lex_t*    x);
MO_EXTERN   void                mo_reg_result   (struct mo_t* mo, struct result_t* r);
MO_EXTERN   void                mo_push_stream  (struct mo_t* mo, struct stream_t* m);
MO_EXTERN   void                mo_push_unit    (struct mo_t* mo, struct unit_t*   u);
MO_EXTERN   struct unit_t*      mo_pop_unit     (struct mo_t* mo);
MO_EXTERN   struct unit_t*      mo_top_unit     (struct mo_t* mo);
MO_EXTERN   mo_errno            mo_walk         (struct mo_t* mo);




#endif//__mo_


