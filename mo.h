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
typedef unsigned char   mo_byte;


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


//  ����result
#define MO_OK               (0)



//  �ͷŶ���Ľӿ�
typedef     void        (*MO_DEL_CALLBACK   )(void* obj);


//  ��ȡ���ݵķ���ֵ����
#define MO_READ_OK      (0)
#define MO_READ_EOF     (1)
#define MO_READ_ERROR   (2)
typedef     int         (*MO_READ_CALLBACK  )(void* ctx, char** pos, char* end);
typedef     void        (*MO_CLOSE_CALLBACK )(void* ctx);


//  �����ڶ��Ĵʷ����
#define     MO_TOKEN_UNKNOWN    (0x00000000)
#define     MO_TOKEN_EOF        (0xFFFFFFFE)
#define     MO_TOKEN_ERROR      (0xFFFFFFFF)
typedef     mo_token    (*MO_NEXT_CALLBACK  )(void* ctx, struct lex_t*  p, struct token_t* t, struct result_t* r);


//  �﷨�����󷵻صĶԵ�ǰ token �Ĵ�����
#define     MO_ACTION_ERROR     (0)     ///<    ��������
#define     MO_ACTION_NEEDMORE  (1)     ///<    ��ȡ�������
#define     MO_ACTION_RETRY     (2)     ///<    ����
#define     MO_ACTION_COMPLETE  (3)     ///<    ʶ�����
typedef     mo_action   (*MO_ACCEPT_CALLBACK)(void* ctx, struct sytx_t* y, struct token_t* t, struct result_t* r);


//  �﷨������
typedef     mo_errno    (*MO_DRIVE_CALLBACK )(void* ctx, struct compile_t* p);

//#define MO_CLASS(t)             \
//    struct token_t*     prev;   \
//    int                 type;
//
//#define MO_DEFINE(new_name, parent, xxx)
//struct new_name
//{
//    struct token_t*     prev;
//    int                 type;
//    yyy
//};

//  ��
struct class_t
{
    void*               prev;
    int                 typeid;
    char                name[64];
    MO_DEL_CALLBACK     del;
};

//  �ʷ�ʶ������ķ���
struct token_t
{
    void*               prev;
    int                 typeid;
    int                 size;       //  ����token��ռ�Ĵ洢�ռ�
    mo_token            id;         //  ���ŵ����ͱ��
    int                 opts;       //  ѡ��
    union 
    {
        struct 
        {
            char        text[64];    //  ���ŵ��ı���Χ
        };
        struct 
        {
            char*       ref[2];     //  ���ŵ��ı���Χ
        };
    };
};


//  �ʷ����﷨�������
struct result_t
{
    void*               prev;
    int                 typeid;
    int                 error;
    char                text[500];
};


//  �ʷ���λ��Ϣ
struct anchor_t
{
    void*               prev;   //  
    int                 typeid; //  
    char*               name;   //  ����
    int                 lino;   //  �к�
    char*               line;   //  ����ʼλ�ã����ڼ��㵱ǰ���ڵ�ǰ�е���һ�У�
};


//  ״̬
struct stats_t
{
    void*               prev;       //  ǰһ��״̬
    int                 typeid;
    int                 state;      //  ��ǰ��״ֵ̬
};


//  ����������
struct stream_t
{
    void*               prev;   //  ǰһ��������
    int                 typeid;
    struct anchor_t     anchor; //  �ʷ�ʶ���ê����Ϣ
    void*               ctx;    //  �������������
    MO_CLOSE_CALLBACK   close;  //  ���رպ���
    MO_READ_CALLBACK    read;   //  ����ȡ����
};


//  �﷨������Ԫ
struct unit_t
{
    void*               prev;   //  ǰһ���﷨��Ԫ
    int                 typeid;
    void*               ctx;    //  �ʷ�������
    MO_ACCEPT_CALLBACK  accept; //  �Ƿ���յ�ǰ�ʷ���Ԫ
};


//  �ʷ���������
struct lex_t
{
    void*               prev;       //  ǰһ���ʷ�����
    int                 typeid;     //  �������ͱ��
    struct stream_t*    stream;     //  ������
    struct anchor_t*    anchor;     //  �ʷ���λ��Ϣ(ֱ�������� stream ����)
    void*               ctx;        //  �ʷ�������
    MO_NEXT_CALLBACK    next;       //  �ʷ�ʶ��ĺ���
    int                 rsrv;       //  Ԥ����С
    int                 cap;        //  ���ݻ������������������ǵ��ڣ�limit - buf��
    char*               pos;        //  ��ǰʶ��λ��ָ��
    char*               end;        //  ��Ч���ݽ���λ�ã�*end��Զ��\n
    char*               limit;      //  ��������β
    mo_byte             buf[0];     //  ���ݻ�����

};


//  �﷨��������
struct sytx_t
{
    void*               prev;       //  ǰһ���ʷ�����
    int                 typeid;       //  �������ͱ��
    struct unit_t*      unit;       //  �﷨ʶ��Ԫ
};

//  ������
struct compile_t
{
    void*               prev;
    int                 typeid;
    struct lex_t*       lex;
    struct sytx_t*      sytx;
    struct token_t*     token;
    struct result_t*    result;
};


//  ע����
MO_EXTERN   int                 mo_define_class         (char* name, MO_DEL_CALLBACK del);
MO_EXTERN   struct class_t*     mo_class_of             (int typeid);
MO_EXTERN   int                 mo_typeid_of            (char* name);



//  ��������Ľӿ�
MO_EXTERN   struct result_t*    mo_result_new           ();
MO_EXTERN   struct result_t*    mo_result_clear         (struct result_t* r);
MO_EXTERN   mo_bool             mo_result_ok            (struct result_t* r);
MO_EXTERN   struct result_t*    mo_result_errorf        (struct result_t* r, int error, char* format, ...);

MO_EXTERN   struct token_t*     mo_token_new            ();
MO_EXTERN   struct token_t*     mo_token_clear          (struct token_t*  k);

//  �ʷ�ʶ��Ľӿ�
MO_EXTERN   struct stream_t*    mo_stream_new           (void* ctx, MO_READ_CALLBACK   read, MO_CLOSE_CALLBACK close);
MO_EXTERN   struct lex_t*       mo_lex_init             (struct lex_t* x, void* ctx, MO_NEXT_CALLBACK   next, int cap, int rsrv);
MO_EXTERN   void                mo_lex_push_stream      (struct lex_t* x, struct stream_t* m);
MO_EXTERN   struct stream_t*    mo_lex_pop_stream       (struct lex_t* x);
MO_EXTERN   mo_token            mo_lex_next_token       (struct lex_t* x, struct token_t* t, struct result_t* r);

//  �﷨ʶ��Ľӿ�
MO_EXTERN   struct unit_t*      mo_unit_new             (struct unit_t* u, void* ctx, MO_ACCEPT_CALLBACK accept);
MO_EXTERN   struct sytx_t*      mo_sytx_init            (struct sytx_t* y, struct lex_t* x);
MO_EXTERN   void                mo_sytx_push_unit       (struct sytx_t* y, struct unit_t* u);
MO_EXTERN   struct unit_t*      mo_sytx_top_unit        (struct sytx_t* y);
MO_EXTERN   void                mo_sytx_drive           (struct sytx_t* y);





#endif//__mo_


