#ifndef __protobuf_model_
#define __protobuf_model_

#define PROTOBUF_TYPE_OF(t) (t##ype)
struct protobuf_node_t
{
    int                     type;
    struct protobuf_node_t* parent;
    struct mo_map_t*        childs;
}




#define protobuf_app_type  (0)
struct  protobuf_app_t
{
    struct protobuf_node_t  node;
};




#define protobuf_option_type  (0)
struct  protobuf_option_t
{
    struct protobuf_node_t  node;
    char*                   key;
    char*                   value;
};




#define protobuf_service_type  (0)
struct  protobuf_service_t
{
    struct protobuf_node_t  node;
};




#define protobuf_message_type  (0)
struct  protobuf_message_t
{
    struct protobuf_node_t  node;
};




#define protobuf_enum_type  (0)
struct  protobuf_enum_t
{
    struct protobuf_node_t  node;
};

#endif//__protobuf_model_

