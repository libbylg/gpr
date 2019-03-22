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


///!    词法识别出来的符号
struct token_t
{
    struct token_t*     prev;
    int                 size;       //  整个token所占的存储空间
    mo_token            type;       //  符号的类型编号
    int                 opts;       //  选项
    union 
    {
        struct 
        {
            char        text[0];    //  符号的文本范围
        };
        struct 
        {
            char*       ref[2];     //  符号的文本范围
        };
    };
};


///!    词法和语法解析结果
struct result_t
{
    int                 error;
    char                text[500];
};

//  词法识别时，用到的cache
struct cache_t
{
    int                 rsrv;       //  预留大小
    int                 cap;        //  数据缓冲区的总容量（总是等于：limit - buf）
    char*               buf;        //  数据缓冲区
    char*               limit;      //  缓冲区结尾
    char*               pos;        //  当前识别位置指针
    char*               end;        //  有效数据结束位置，*end永远是\n
    void*               ctx;        //  词法识别的cache上下文
};

//  上下文
struct context_t
{
    int                 size;
    struct context_t*   prev;
    int                 state;
};

struct compile_t;
struct lex_t;
struct sytx_t;
struct unit_t;
struct stream_t;

struct compile_t
{
    struct lex_t*       lex;
    struct sytx_t*      sytx;
    struct token_t*     token;
    struct result_t*    result;
};

#define MO_READ_OK      (0)
#define MO_READ_EOF     (1)
#define MO_READ_ERROR   (2)

typedef     int       (*MO_READ_CALLBACK  )(void* ctx, char** pos, char* end);
typedef     int       (*MO_CLOSE_CALLBACK )(void* ctx);

typedef     mo_token  (*MO_NEXT_CALLBACK  )(void* ctx, struct lex_t*  x, struct token_t* t, struct result_t* r);
typedef     mo_action (*MO_ACCEPT_CALLBACK)(void* ctx, struct unit_t* u, struct token_t* t, struct result_t* r);

MO_EXTERN   struct cache_t*     mo_cache_new    (void* ctx, int cap, int rsrv);
MO_EXTERN   struct lex_t*       mo_lex_new      (void* ctx, MO_NEXT_CALLBACK next);
MO_EXTERN   struct sytx_t*      mo_sytx_new     (void* ctx);
MO_EXTERN   struct lex_t*       mo_unit_new     (void* ctx, MO_ACCEPT_CALLBACK accept);
MO_EXTERN   struct stream_t*    mo_stream_new   (void* ctx, MO_READ_CALLBACK   accept, MO_READ_CALLBACK close);


MO_EXTERN   struct compile_t*   mo_compile_new  (void* ctx, struct lex_t* x, struct sytx_t* y, struct cache_t* c);
MO_EXTERN   void                mo_compile_del  (struct compile_t* p);

MO_EXTERN   void                mo_compile_push_stream  (struct compile_t* p, struct stream_t* m);
MO_EXTERN   struct stream_t*    mo_compile_pop__stream  (struct compile_t* p);

MO_EXTERN   void                mo_compile_push_unit    (struct compile_t* p, struct unit_t*   u);
MO_EXTERN   struct unit_t*      mo_compile_top_unit     (struct compile_t* p);

MO_EXTERN   mo_errno            mo_compile_walk         (struct compile_t* p);

MO_EXTERN   struct result_t*    mo_result_clear         (struct result_t* r);
MO_EXTERN   struct token_t*     mo_token_clear          (struct token_t*  k);


MO_EXTERN   struct stream_t*    mo_stream_file_new      (const char* filename);
MO_EXTERN   struct stream_t*    mo_stream_string_new    (char* str, int size, int auto_free);


#endif//__mo_


