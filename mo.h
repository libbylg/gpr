#ifndef __mo_
#define __mo_

#if defined(__cplusplus)
#define MO_EXTERN  extern  "C"
#else
#define MO_EXTERN  extern
#endif//





typedef int             mo_errno;
typedef unsigned int    mo_token;
typedef int             mo_action;
// typedef int             mo_bool;
// #define MO_TRUE    (1)
// #define MO_FALSE   (0)


struct lex_t;
struct sytx_t;
struct unit_t;
struct feed_t;
struct result_t;
struct token_t;


struct token_t
{
    mo_token    token;      ///<    符号的编号
    char*       text[2];    ///<    符号的文本范围
};


#define     MO_ACTION_ERROR     (0)
#define     MO_ACTION_NEEDMORE  (1)
#define     MO_ACTION_TRYAGAIN  (2)

#define     MO_TOKEN_ERROR      (0)
#define     MO_TOKEN_EOF        (0xFFFFFFFF)

typedef     int       (*MO_READ_CALLBACK  )(void* ctx, char* buf, int* len);
typedef     mo_token  (*MO_NEXT_CALLBACK  )(void* ctx, struct lex_t*  x, struct token_t* t);
typedef     mo_action (*MO_ACCEPT_CALLBACK)(void* ctx, struct sytx_t* y, struct token_t* t);

MO_EXTERN   struct unit_t*      mo_unit_new     (void* ctx, MO_ACCEPT_CALLBACK accept);
MO_EXTERN   void                mo_unit_del     (struct unit_t* u);

MO_EXTERN   struct stream_t*    mo_stream_new   (void* ctx, MO_READ_CALLBACK   read);
MO_EXTERN   void                mo_stream_del   (struct stream_t* u);

MO_EXTERN   struct lex_t*       mo_lex_new      (void* ctx, MO_NEXT_CALLBACK   next);
MO_EXTERN   void                mo_lex_del      (struct lex_t* x);
MO_EXTERN   void                mo_lex_put      (struct lex_t* x, struct stream_t* m);

MO_EXTERN   struct sytx_t*      mo_sytx_new     ();
MO_EXTERN   void                mo_sytx_del     (struct sytx_t* y);
MO_EXTERN   void                mo_sytx_push    (struct sytx_t* y, struct unit_t* u);
MO_EXTERN   struct unit_t*      mo_sytx_pop     (struct sytx_t* y);
MO_EXTERN   struct unit_t*      mo_sytx_top     (struct sytx_t* y);

MO_EXTERN   int                 mo_walk(struct sytx_t* y, struct lex_t* x);







#endif//__mo_


