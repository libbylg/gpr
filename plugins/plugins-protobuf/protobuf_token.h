#ifndef __protobuf_token_
#define __protobuf_token_


#include "mo.h"


struct protobuf_token_t
{
    struct token_t super;
};






///!    所有的符号定义，如果是单字符符号，那么直接用字符本身的字面量代替
//                                  //  {
//                                  //  }
//                                  //  (
//                                  //  )
//                                  //  ;
//                                  //  ,
//                                  //  =
//                                  //  <
//                                  //  >
#define MO_TOKEN_STRING     200     //  "xxxx"
#define MO_TOKEN_INTEGER    201     //  12345
#define MO_TOKEN_message    301     //  message
#define MO_TOKEN_service    301     //  service
#define MO_TOKEN_required   302     //  required
#define MO_TOKEN_optional   303     //  optional
#define MO_TOKEN_repeated   304     //  repeated
#define MO_TOKEN_rpc        305     //  rpc
#define MO_TOKEN_init       306     //  init
#define MO_TOKEN_exit       307     //  exit
#define MO_TOKEN_reserved   308     //  reserved
#define MO_TOKEN_enum       309     //  enum
#define MO_TOKEN_import     310     //  import
#define MO_TOKEN_extensions 311     //  extensions
#define MO_TOKEN_extend     312     //  extend
#define MO_TOKEN_oneof      313     //  oneof
#define MO_TOKEN_map        314     //  map
#define MO_TOKEN_package    315     //  package
#define MO_TOKEN_option     316     //  option
#define MO_TOKEN_returns    317     //  returns
#define MO_TOKEN_ERROR      999     //  遇到错误

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
    CM_DEC      =   0x0001,
    CM_HEX      =   0x0002,
    CM_NEWLINE  =   0x0004,
    CM_SPACE    =   0x0008,

    CM_ALPHA    =   0x0010,
    CM_ALPHAx   =   0x0020,
};
MO_EXTERN short cm[256];

#endif//__protobuf_token_

