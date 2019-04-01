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


typedef int                 mo_bool;
typedef int                 mo_errno;
typedef unsigned char       mo_byte;
typedef unsigned int        mo_cm;
#ifdef _MSC_VER
typedef __int64             mo_int64;
typedef unsigned __int64    mo_uint64;
#else
typedef long long           mo_int64;
typedef unsigned long long  mo_uint64;
#endif


#define MO_FALSE        (!!0)
#define MO_TRUE         (!0)


struct token_t;
struct lex_t;
struct stream_t;




//  �ͷŶ���Ľӿ�
typedef     void        (*MO_DEL_CALLBACK   )(void* obj);


//  ��ȡ���ݵķ���ֵ����
#define MO_READ_OK      (0)
#define MO_READ_EOF     (1)
#define MO_READ_ERROR   (2)
typedef     int         (*MO_READ_CALLBACK  )(void* ctx, char** pos, char* end);
typedef     void        (*MO_CLOSE_CALLBACK )(void* ctx);


//  �����ڶ��Ĵʷ����
#define     MO_TOKEN_UNKNOWN    (-1)
#define     MO_TOKEN_EOF        (-2)
#define     MO_TOKEN_ERROR      (-3)
typedef     struct token_t*     (*MO_NEXT_CALLBACK)(void* ctx, struct lex_t* x, struct token_t* t, struct result_t* r);


//  ����
struct object_t
{
    void*               prev;
    int                 typeid;
};


//  �ࣨע�⣬��Ҳ��һ������
struct class_t
{
    void*               prev;
    int                 typeid;
    char                name[64];
    MO_DEL_CALLBACK     del;
};


//  ���Ͷ����
#define MO_DEFINE(cls_name, body)  \
    cls_name{                   \
        void*   prev;           \
        int     typeid;         \
        struct  body;           \
    }


//  ���
#define MO_OK               (0)
MO_DEFINE(struct result_t,
{
    int         error;      //  ������
    char        desc[500];  //  ��������
});


//  �ʷ�ʶ������ķ���

#define MO_TOKEN_SIZE_MAX   (32)
MO_DEFINE(struct token_t,
{
    int                 size;       //  ����token��ռ�Ĵ洢�ռ�
    int                 id;         //  ���ŵ����ͱ��
    int                 opts;       //  ѡ��
    union 
    {
        struct
        {
            char*       val_str[2];                 //  ���ŵ��ı�ָ��
            char        text[MO_TOKEN_SIZE_MAX];    //  ���ŵ��ı�����
        };
        struct
        {
            mo_int64    val_int64;
        };
        struct
        {
            mo_uint64   val_uint64;
        };
        struct
        {
            double      val_double;
        };
    };
});


//  �ʷ���λ��Ϣ
struct anchor_t
{
    char*               name;   //  ����
    int                 lino;   //  �к�
    char*               line;   //  ����ʼλ�ã����ڼ��㵱ǰ���ڵ�ǰ�е���һ�У�
};


//  ����������
MO_DEFINE(struct stream_t,
{
    void*               prev;   //  ǰһ��������
    int                 typeid;
    struct anchor_t     anchor; //  �ʷ�ʶ���ê����Ϣ
    void*               ctx;    //  �������������
    MO_CLOSE_CALLBACK   close;  //  ���رպ���
    MO_READ_CALLBACK    read;   //  ����ȡ����
});


//  �ʷ���������
MO_DEFINE(struct lex_t,
{
    void*               prev;       //  ǰһ���ʷ�����
    int                 typeid;     //  �������ͱ��
    //
    void*               ctx;        //  �ʷ�ʶ���������
    MO_NEXT_CALLBACK    next;       //  �ʷ�ʶ��ĺ���
    //
    struct result_t*    result;     //  ��ǰ�Ľ��
    struct token_t*     token;      //  ���ʶ�������һ������,mo_lex_push_back�����Ὣ���˵�token�ݴ浽����
    struct stream_t*    stream;     //  ������
    struct anchor_t*    anchor;     //  �ʷ���λ��Ϣ(ֱ�������� stream ����)
    //
    int                 cap;        //  ���ݻ������������������ǵ��ڣ�limit - buf��
    char*               pos;        //  ��ǰʶ��λ��ָ��
    char*               end;        //  ��Ч���ݽ���λ�ã�*end��Զ��\n
    char*               limit;      //  ��������β
    mo_byte*            buf;        //  ���ݻ�����
});


//  Ĭ�ϵ��ַ�ӳ�����
#define MO_CM_DEC                  (0x00100000)
#define MO_CM_HEX                  (0x00200000)
#define MO_CM_OCT                  (0x00400000)
#define MO_CM_NEWLINE              (0x00800000)
#define MO_CM_SPACE                (0x01000000)
#define MO_CM_ALPHA                (0x02000000)
#define MO_CM_STRING_FLAG          (0x04000000)
#define MO_CM_COMMENT_FLAG         (0x08000000)
#define MO_CM_UDRLINE              (0x10000000)
MO_EXTERN   mo_cm*              mo_cms_default();


//  ע����
MO_EXTERN   int                 mo_define_class         (char* name, MO_DEL_CALLBACK del);
MO_EXTERN   struct class_t*     mo_class_of             (int typeid);
MO_EXTERN   int                 mo_typeid_of            (char* name);
MO_EXTERN   void                mo_object_del           (void* obj);


//  �������
MO_EXTERN   struct result_t*    mo_result_new           ();
MO_EXTERN   mo_bool             mo_result_ok            (struct result_t* r);
MO_EXTERN   struct result_t*    mo_result_errorf        (struct result_t* r, int error, char* format, ...);
MO_EXTERN   struct result_t*    mo_result_verrorf       (struct result_t* r, int error, char* format, va_list args);
MO_EXTERN   struct result_t*    mo_result_clear         (struct result_t* r);


//  ���Ų���
#define MO_TOKEN_UNKNOEN        (-1)
#define MO_TOKEN_ERROR          (-2)
#define MO_TOKEN_EOF            (-3)
#define MO_TOKEN_STRING         (-300)
#define MO_TOKEN_NAME           (-301)
#define MO_TOKEN_INTEGER        (-302)
#define MO_TOKEN_FLOAT          (-303)
MO_EXTERN   struct token_t*     mo_token_new            ();
MO_EXTERN   mo_bool             mo_token_ok             (struct token_t* k);
MO_EXTERN   struct token_t*     mo_token_clear          (struct token_t* k);
MO_EXTERN   struct token_t*     mo_token_errorf         (struct token_t* k, struct result_t* r, int error, char* format, ...);
MO_EXTERN   struct token_t*     mo_token_as             (struct token_t* k, int id, int opts, mo_byte* bgn, mo_byte* end);

//  �ʷ�ʶ��Ľӿ�
MO_EXTERN   struct stream_t*    mo_stream_new           (void* ctx, MO_READ_CALLBACK   read, MO_CLOSE_CALLBACK close);


//  �ʷ�ʶ��Ľӿ�
MO_EXTERN   struct lex_t*       mo_lex_new              (void* ctx, MO_NEXT_CALLBACK   next, int cap);
MO_EXTERN   void                mo_lex_push_stream      (struct lex_t* x, struct stream_t* m);
MO_EXTERN   struct stream_t*    mo_lex_pop_stream       (struct lex_t* x);
MO_EXTERN   struct token_t*     mo_lex_next_token       (struct lex_t* x, struct token_t* k, struct result_t* r);
MO_EXTERN   void                mo_lex_push_back        (struct lex_t* x, struct token_t* k);


#define MO_ACCEPT(lex,pc,funccall)  \
    (lex)->pos = pc;                \
    (funccall);                     \
    pc = (lex)->pos;


#endif//__mo_


