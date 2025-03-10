
file(GLOB_RECURSE SOURCES ${TCSL_ROOT_DIR}/src/*.c)

idf_component_register(SRCS ${SOURCES} INCLUDE_DIRS ${TCSL_ROOT_DIR} )
