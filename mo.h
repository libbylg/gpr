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
#define     MO_ACTION_ERROR     (0)     ///<    遇到错误
#define     MO_ACTION_NEEDMORE  (1)     ///<    读取更多符号
#define     MO_ACTION_RETRY     (2)     ///<    重试
#define     MO_ACTION_COMPLETE  (3)     ///<    识别完成



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



///!    核心参数
struct params_t
{
    int cache_size;
};


///!    词法识别出来的符号
struct token_t
{
    int                 size;       ///<    整个token所占的存储空间
    mo_token            token;      ///<    符号的编号
    char*               text[2];    ///<    符号的文本范围
};




///!    词法和语法解析结果
struct result_t
{
    struct result_t*    prev;
    int                 value;
    char                module[10];
    char                text[500];
};




///!    cache 管理
#define MO_TOKEN_LIMIT  (32)
struct cache_t
{
    int                 size;
    MO_DEL_CALLBACK     del;
    struct cache_t*     prev;       ///<   前一个cache
    struct stream_t*    stream;     ///<    每个cache都对应一个输入流
    struct mo_t*        mo;
    int                 buf_size;   ///<    数据缓冲区的总大小
    char*               buf;        ///<    数据缓冲区
    char*               buf_limit;  ///<    cache中可用来缓存数据的位置
    char*               pc;         ///<    当前识别位置指针
    char*               pe;         ///<    有效数据结束位置，*pe永远是\n
    char*               line;       ///<    行起始位置
    int                 lino;       ///<    行号
};




///!    词法分析对象
typedef     mo_token  (*MO_NEXT_CALLBACK  )(struct lex_t*    x, struct token_t* t);
struct lex_t
{
    int                 size;
    struct cache_t*     cache_top;
    struct mo_t*        mo;
    MO_DEL_CALLBACK     del;
    MO_NEXT_CALLBACK    next;
};




///!    输入流对象
#define MO_READ_OK      (0)
#define MO_READ_EOF     (1)
#define MO_READ_ERROR   (2)
typedef     int       (*MO_READ_CALLBACK  )(struct stream_t* m, char** pos, char* end);
struct stream_t
{
    int                 size;
    struct mo_t*        mo;
    MO_DEL_CALLBACK     del;
    MO_READ_CALLBACK    read;
};




///!    语法分析单元
typedef     mo_action (*MO_ACCEPT_CALLBACK)(struct unit_t*   u, struct token_t* t);
struct unit_t
{
    int                 size;
    struct unit_t*      prev;
    struct mo_t*        mo;
    MO_DEL_CALLBACK     del;
    MO_ACCEPT_CALLBACK  accept;
};





MO_EXTERN   struct mo_t*        mo_new          ();
MO_EXTERN   void                mo_del          (struct mo_t* mo);
MO_EXTERN   struct params_t*    mo_get_params   (struct mo_t* mo);
MO_EXTERN   void                mo_reg_lex      (struct mo_t* mo, struct lex_t*    x);
MO_EXTERN   void                mo_push_stream  (struct mo_t* mo, struct stream_t* m);
MO_EXTERN   void                mo_push_unit    (struct mo_t* mo, struct unit_t*   u);
MO_EXTERN   struct unit_t*      mo_pop_unit     (struct mo_t* mo);
MO_EXTERN   struct unit_t*      mo_top_unit     (struct mo_t* mo);
MO_EXTERN   mo_errno            mo_walk         (struct mo_t* mo);


MO_EXTERN   struct result_t*    mo_result_new   (char* module, int value, char* format, ...);
MO_EXTERN   void                mo_push_result  (struct mo_t* mo, struct result_t* r);
MO_EXTERN   void                mo_clear_result (struct mo_t* mo);


MO_EXTERN   struct stream_t*    mo_stream_file_new  (char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new(char* str, int size, int auto_free);


MO_EXTERN   int                 mo_cache_load(struct cache_t* c); ///<    从流中加载数据


#endif//__mo_


