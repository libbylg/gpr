#ifndef __mo_
#define __mo_

#if defined(__cplusplus)
#define MO_EXTERN  extern  "C"
#else
#define MO_EXTERN  extern
#endif//




typedef int             mo_bool;


#define MO_TRUE    (1)
#define MO_FALSE   (0)


struct lex_t;
struct unit_t;
struct mo_t;
struct result_t;
struct token_t;



MO_EXTERN   struct lex_t*       lex_new (void* ctx, void (*next  )(mo_void* ctx, struct lex_t*  x, struct token_t* t));
MO_EXTERN   struct unit_t*      unit_new(void* ctx, mo_i32  (*accept)(mo_void* ctx, struct unit_t* x, struct token_t* t));

MO_EXTERN   struct mo_t*        mo_new(void* ctx, struct lex_t* x, struct unit_t* u);
MO_EXTERN   void                mo_del(struct mo_t* m);


#endif//__mo_

