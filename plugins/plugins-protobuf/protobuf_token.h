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
#define MO_TOKEN_MESSAGE    301     //  message
#define MO_TOKEN_SERVICE    301     //  service
#define MO_TOKEN_REQUIRED   302     //  required
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


#endif//__protobuf_token_

