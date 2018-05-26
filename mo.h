#ifndef __mo_
#define __mo_

#if defined(__cplusplus)
#define MO_EXTERN  extern  "C"
#else
#define MO_EXTERN  extern
#endif//




typedef unsigned int    mo_u32;
typedef int             mo_errno;
typedef int             mo_bool;
#define MO_TRUE    (1)
#define MO_FALSE   (0)


struct lex_t;
struct unit_t;
struct mo_t;
struct result_t;
struct token_t;


struct token_t
{
    mo_u32  type;
    char*   text[2];
};

struct result_t
{
    mo_errno    id;
    char        error[256];
};


#define     MO_UNIT_ERROR   (0)
#define     MO_UNIT_MORE    (1)
#define     MO_UNIT_AGAIN   (2)


MO_EXTERN   struct mo_t*        mo_new(void* ctx, struct lex_t* x, struct unit_t* u, struct result_t* r);
MO_EXTERN   struct lex_t*       mo_lex_new (void* ctx, mo_u32 (*next  )(void* ctx, struct lex_t*  x, struct token_t* t), struct result_t* r);
MO_EXTERN   struct unit_t*      mo_unit_new(void* ctx, mo_u32 (*accept)(void* ctx, struct unit_t* x, struct token_t* t), struct result_t* r);



MO_EXTERN   struct unit_t*      mo_compile(struct mo_t* m, void* ctx, int (*read)(void* ctx, char* data, int* len));
MO_EXTERN   void                mo_del(struct mo_t* m);




MO_EXTERN   struct mo_t*        mo_push(struct mo_t* m, struct unit_t* u);
MO_EXTERN   struct unit_t*      mo_pop(struct mo_t* m);



#endif//__mo_


