CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(plugins-protobuf)



#   All headers
SET(HRDS
    protobuf.h
    protobuf_lex.h
    protobuf_token.h
    )
SET_SOURCE_FILES_PROPERTIES(${HRDS} PROPERTIES HEADER_FILE_ONLY YES)



#   All sources
SET(SRCS
    protobuf_lex.c
    protobuf_token.c
    protobuf_unit_app.c
    protobuf_unit_enum.c
    protobuf_unit_enum_item.c
    protobuf_unit_enum_item_list.c
    protobuf_unit_import.c
    protobuf_unit_message.c
    protobuf_unit_message_field.c
    protobuf_unit_message_field_list.c
    protobuf_unit_option.c
    protobuf_unit_package.c
    protobuf_unit_param_list.c
    protobuf_unit_ref.c
    protobuf_unit_reserved.c
    protobuf_unit_reserved_id.c
    protobuf_unit_service.c
    protobuf_unit_service_field_list.c
    protobuf_unit_service_method.c
    protobuf_unit_syntax.c
    ${HRDS}
    )


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")


#   Change the properties of files for Visual Stdio C++
IF   (MSVC)
    SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES LANGUAGE CXX)
ENDIF(MSVC)



#   Where to find the libs and included files 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../out/)


#   Use the debug mode
ADD_DEFINITIONS(-DDEBUG)


#   Define the targets
ADD_EXECUTABLE(plugins-protobuf ${SRCS})


#   Which libs to link
TARGET_LINK_LIBRARIES(plugins-protobuf mo)



#   Set the output dirs
SET_TARGET_PROPERTIES(plugins-protobuf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../../out/)

