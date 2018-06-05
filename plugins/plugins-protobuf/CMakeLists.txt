CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(plugins-protobuf)



#   All headers
SET(HRDS    protobuf.h   
            protobuf_lex.h
            protobuf_token.h
    )
SET_SOURCE_FILES_PROPERTIES(${HRDS} PROPERTIES HEADER_FILE_ONLY YES)



#   All sources
SET(SRCS    protobuf_units.c
            protobuf_lex.c
            protobuf_demo.c
            protobuf_token.c
            ${HRDS}
)



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

