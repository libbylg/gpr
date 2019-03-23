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


#define MO_ASSERT(expr,msg) assert()


typedef int             mo_bool;
typedef int             mo_errno;
typedef unsigned int    mo_token;
typedef int             mo_action;


#define MO_FALSE        (!!0)
#define MO_TRUE         (!0)


struct result_t;
struct token_t;
struct unit_t;
struct stream_t;
struct cache_t;
struct lex_t;
struct sytx_t;
struct compile_t;


//  用于result
#define MO_OK               (0)



//  释放对象的接口
typedef     void        (*MO_DEL_CALLBACK   )(void* obj);


//  读取数据的返回值定义
#define MO_READ_OK      (0)
#define MO_READ_EOF     (1)
#define MO_READ_ERROR   (2)
typedef     int         (*MO_READ_CALLBACK  )(void* ctx, char** pos, char* end);
typedef     void        (*MO_CLOSE_CALLBACK )(void* ctx);


//  几个内定的词法编号
#define     MO_TOKEN_UNKNOWN    (0x00000000)
#define     MO_TOKEN_EOF        (0xFFFFFFFE)
#define     MO_TOKEN_ERROR      (0xFFFFFFFF)
typedef     mo_token    (*MO_NEXT_CALLBACK  )(void* ctx, struct lex_t*  p, struct token_t* t, struct result_t* r);


//  语法分析后返回的对当前 token 的处理建议
#define     MO_ACTION_ERROR     (0)     ///<    遇到错误
#define     MO_ACTION_NEEDMORE  (1)     ///<    读取更多符号
#define     MO_ACTION_RETRY     (2)     ///<    重试
#define     MO_ACTION_COMPLETE  (3)     ///<    识别完成
typedef     mo_action   (*MO_ACCEPT_CALLBACK)(void* ctx, struct sytx_t* y, struct token_t* t, struct result_t* r);


//  语法驱动器
typedef     mo_errno    (*MO_DRIVE_CALLBACK )(void* ctx, struct compile_t* p);

//  对象
struct object_t
{
    int                 type;
    char                name[64];
    MO_DEL_CALLBACK     del;
};

//  词法识别出来的符号
struct token_t
{
    struct token_t*     prev;
    int                 type;
    int                 size;       //  整个token所占的存储空间
    mo_token            id;         //  符号的类型编号
    int                 opts;       //  选项
    union 
    {
        struct 
        {
            char        text[64];    //  符号的文本范围
        };
        struct 
        {
            char*       ref[2];     //  符号的文本范围
        };
    };
};


//  词法和语法解析结果
struct result_t
{
    struct result_t*    prev;
    int                 type;
    int                 error;
    char                text[500];
};


//  词法识别时，用到的cache
struct cache_t
{
    void*               prev;       //  前一个cache对象
    int                 type;
    struct anchor_t*    anchor;     //  词法定位信息(直接引用自 stream 对象)
    int                 rsrv;       //  预留大小
    int                 cap;        //  数据缓冲区的总容量（总是等于：limit - buf）
    char*               pos;        //  当前识别位置指针
    char*               end;        //  有效数据结束位置，*end永远是\n
    char*               limit;      //  缓冲区结尾
    char                buf[0];     //  数据缓冲区
};


//  状态
struct stats_t
{
    void*               prev;       //  前一个状态
    int                 type;
    int                 state;      //  当前的状态值
};


//  输入流对象
struct stream_t
{
    void*               prev;   //  前一个流对象
    int                 type;
    struct anchor_t     anchor; //  词法识别的锚点信息
    void*               ctx;    //  流对象的上下文
    MO_CLOSE_CALLBACK   close;  //  流关闭函数
    MO_READ_CALLBACK    read;   //  流读取函数
};


//  语法分析单元
struct unit_t
{
    void*               prev;   //  前一个语法单元
    int                 type;
    void*               ctx;    //  词法上下文
    MO_ACCEPT_CALLBACK  accept; //  是否接收当前词法单元
};


//  词法分析对象
struct lex_t
{
    void*               prev;       //  前一个词法对象
    int                 type;       //  对象类型编号
    void*               ctx;        //  词法上下文
    MO_NEXT_CALLBACK    next;       //  词法识别的函数

    struct cache_t*     cache;
    struct stream_t*    stream;
};


//  语法分析对象
struct sytx_t
{
    void*               prev;       //  前一个词法对象
    int                 type;       //  对象类型编号
    struct unit_t*      unit;       //  语法识别单元
};

//  编译器
struct compile_t
{
    void*               prev;
    int                 type;
    struct lex_t*       lex;
    struct sytx_t*      sytx;
    struct token_t*     token;
    struct result_t*    result;
};


//  词法定位信息
struct anchor_t
{
    char*               name;   //  名称
    int                 lino;   //  行号
    char*               line;   //  行起始位置（用于计算当前处于当前行的那一列）
};



//  注册类
MO_EXTERN   int                 mo_register_object       (char* name, MO_DEL_CALLBACK del);


//  基础对象的接口
MO_EXTERN   struct result_t*    mo_result_new           ();
MO_EXTERN   struct result_t*    mo_result_clear         (struct result_t* r);
MO_EXTERN   mo_bool             mo_result_ok            (struct result_t* r);

MO_EXTERN   struct token_t*     mo_token_new            ();
MO_EXTERN   struct token_t*     mo_token_clear          (struct token_t*  k);

//  词法识别的接口
MO_EXTERN   struct stream_t*    mo_stream_new           (void* ctx, MO_READ_CALLBACK   read, MO_READ_CALLBACK close);
MO_EXTERN   struct lex_t*       mo_lex_new              (void* ctx, MO_NEXT_CALLBACK   next, int cap, int rsrv);
MO_EXTERN   void                mo_lex_push_stream      (struct lex_t* x, struct stream_t* m);
MO_EXTERN   struct stream_t*    mo_lex_pop_stream       (struct lex_t* x);
MO_EXTERN   struct result_t*    mo_lex_next_token       (struct lex_t* x, struct token_t* t, struct result_t* r);

//  语法识别的接口
MO_EXTERN   struct unix_t*      mo_unit_new             (void* ctx, MO_ACCEPT_CALLBACK accept);
MO_EXTERN   struct sytx_t*      mo_sytx_new             ();
MO_EXTERN   void                mo_sytx_push_unit       (struct sytx_t* y, struct unit_t*   u);
MO_EXTERN   struct unit_t*      mo_sytx_top_unit        (struct sytx_t* y);
MO_EXTERN   struct result_t*    mo_sytx_accept_token    (struct sytx_t* y, struct token_t* t, struct result_t* r);

//  编译器接口
MO_EXTERN   struct compile_t*   mo_compile_init         (struct compile_t* p, struct lex_t* x, struct sytx_t* y);
MO_EXTERN   void                mo_compile_clear        (struct compile_t* p);
MO_EXTERN   void                mo_compile_drive        (struct compile_t* p);


MO_EXTERN   struct stream_t*   mo_stream_file_new       (const char* filename);

#endif//__mo_


