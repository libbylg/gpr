#ifndef __mo_hash_
#define __mo_hash_


#if defined(__cplusplus)
#define MO_EXTERN  extern  "C"
#else
#define MO_EXTERN  extern
#endif//


MO_EXTERN   size_t BKDRHash(const char *str)    ;
MO_EXTERN   size_t SDBMHash(const char *str)    ;
MO_EXTERN   size_t RSHash(const char *str)      ;
MO_EXTERN   size_t APHash(const char *str)      ;
MO_EXTERN   size_t JSHash(const char *str)      ;
MO_EXTERN   size_t DEKHash(const char* str)     ;
MO_EXTERN   size_t FNVHash(const char* str)     ;
MO_EXTERN   size_t DJBHash(const char *str)     ;
MO_EXTERN   size_t PJWHash(const char *str)     ;
MO_EXTERN   size_t ELFHash(const char *str)     ;

#endif//__mo_hash_

