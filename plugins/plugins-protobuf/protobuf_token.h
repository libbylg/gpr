#ifndef __protobuf_token_
#define __protobuf_token_


#include "mo.h"


struct protobuf_token_t
{
    struct token_t super;
};




///!    所有的符号定义，如果是单字符符号，那么直接用字符本身的字面量代替
//  下面都是单符号分隔符
//                                      //  {
//                                      //  }
//                                      //  (
//                                      //  )
//                                      //  ;
//                                      //  ,
//                                      //  =
//                                      //  <
//                                      //  >
//  下面都是关键字
#define MO_TOKEN_message        300     //  message
#define MO_TOKEN_service        301     //  service
#define MO_TOKEN_required       302     //  required
#define MO_TOKEN_optional       303     //  optional
#define MO_TOKEN_repeated       304     //  repeated
#define MO_TOKEN_rpc            305     //  rpc
#define MO_TOKEN_init           306     //  init
#define MO_TOKEN_exit           307     //  exit
#define MO_TOKEN_reserved       308     //  reserved
#define MO_TOKEN_enum           309     //  enum
#define MO_TOKEN_import         310     //  import
#define MO_TOKEN_extensions     311     //  extensions
#define MO_TOKEN_extend         312     //  extend
#define MO_TOKEN_oneof          313     //  oneof
#define MO_TOKEN_map            314     //  map
#define MO_TOKEN_package        315     //  package
#define MO_TOKEN_option         316     //  option
#define MO_TOKEN_returns        317     //  returns
#define MO_TOKEN_syntax         318     //  syntax
#define MO_TOKEN_RESERVED_MAX   399     //  -------
//  下面都是字面量或者用户定义的名字
#define MO_TOKEN_STRING         400     //  "xxxx"
#define MO_TOKEN_INTEGER        401     //  12345
#define MO_TOKEN_NAME           402     //  一个名字
#define MO_TOKEN_COMMENT        403     //  一个注释
//      double
//      float
//      int32
//      int64
//      uint32
//      uint64
//      sint32
//      sint64
//      fixed32
//      fixed32
//      sfixed32
//      sfixed64
//      bool
//      string
//      bytes




enum
{
    CM_DEC          =   0x0001,
    CM_HEX          =   0x0002,
    CM_NEWLINE      =   0x0004,
    CM_SPACE        =   0x0008,
    CM_ALPHA        =   0x0010,
    //CM_ALPHAx       =   0x0020,
        CM_STRING_FLAG  =   0x0040,
    CM_COMMENT_FLAG =   0x0080,
};
MO_EXTERN unsigned short cm[256];



#endif//__protobuf_token_

