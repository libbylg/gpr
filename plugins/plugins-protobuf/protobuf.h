#ifndef __protobuf_
#define __protobuf_

#include "mo.h"


MO_EXTERN struct unit_t* protobuf_unit_app_new();
MO_EXTERN struct unit_t* protobuf_unit_enum_new();
MO_EXTERN struct unit_t* protobuf_unit_enum_item_new();
MO_EXTERN struct unit_t* protobuf_unit_enum_item_list_new();
MO_EXTERN struct unit_t* protobuf_unit_import_new();
MO_EXTERN struct unit_t* protobuf_unit_message_new();
MO_EXTERN struct unit_t* protobuf_unit_message_field_new();
MO_EXTERN struct unit_t* protobuf_unit_message_field_list_new();
MO_EXTERN struct unit_t* protobuf_unit_option_new();
MO_EXTERN struct unit_t* protobuf_unit_package_new();
MO_EXTERN struct unit_t* protobuf_unit_param_list_new();
MO_EXTERN struct unit_t* protobuf_unit_ref_new();
MO_EXTERN struct unit_t* protobuf_unit_reserved_new();
MO_EXTERN struct unit_t* protobuf_unit_reserved_id_new();
MO_EXTERN struct unit_t* protobuf_unit_reserved_name_new();
MO_EXTERN struct unit_t* protobuf_unit_service_new();
MO_EXTERN struct unit_t* protobuf_unit_service_field_list_new();
MO_EXTERN struct unit_t* protobuf_unit_service_method_new();
MO_EXTERN struct unit_t* protobuf_unit_syntax_new();


#endif//__protobuf_
