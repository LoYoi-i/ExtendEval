cmake_minimum_required(VERSION 3.20.0)
project(ExtendEval)

#init
set(CMAKE_CXX_STANDARD 20)
set(AE_SDK_PATH "E:/C_C++/After-Effects-Plug-ins/include")
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(DIST_DIR ${BIN_DIR}/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${DIST_DIR})
file(
    GLOB_RECURSE
    SOURCE_FILES
    ${AE_SDK_PATH}/Util/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(
    ${PROJECT_NAME}
    MODULE
    ${SOURCE_FILES}
)

# to aex plugin
target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    UNICODE
    _UNICODE
    WIN32
    WINDOWS
    _WINDOWS
)
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${AE_SDK_PATH}/External
    ${AE_SDK_PATH}/Headers
    ${AE_SDK_PATH}/Headers/SP
    ${AE_SDK_PATH}/Resources
    ${AE_SDK_PATH}/Util
    ${PROJECT_SOURCE_DIR}/inc/func
    ${PROJECT_SOURCE_DIR}/inc
)
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/res)
set(RC_R ${RESOURCE_DIR}/${PROJECT_NAME}.r)
set(RC_RR ${RESOURCE_DIR}/${PROJECT_NAME}.rr)
set(RC_RRC ${RESOURCE_DIR}/${PROJECT_NAME}.rrc)
set(RC_RC ${RESOURCE_DIR}/${PROJECT_NAME}.rc)
add_custom_command(
    OUTPUT ${RC_RR}
    COMMAND cl /I "${AE_SDK_PATH}/Headers" /EP \"${RC_R}\" > ${RC_RR}
)
add_custom_command(
    DEPENDS ${RC_RR}
    OUTPUT ${RC_RRC}
    COMMAND "${AE_SDK_PATH}/Resources/PiPLtool.exe" "${RC_RR}" "${RC_RRC}"
)
add_custom_command(
    DEPENDS ${RC_RRC}
    OUTPUT ${RC_RC}
    COMMAND cl /EP ${RC_RRC} > ${RC_RC}
)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".aex")
target_sources(
    ${PROJECT_NAME}
    PRIVATE
    ${RC_RC}
)

#打包dll
# set(MyDLL "./pipe_server.dll")
# set(DLL_RC ${BIN_DIR}/pipe_server.rc)
# set(DLL_RES ${BIN_DIR}/pipe_server.res)
# configure_file(
#     ${MyDLL}
#     ${DIST_DIR}/pipe_server.dll
#     COPYONLY)
# find_program(RC_COMPILER NAMES windres rc)
# if(NOT RC_COMPILER)
#     message(FATAL_ERROR "Resource compiler not found")
# endif()
# file(WRITE ${DLL_RC} "IDR_EMBEDDED_DLL1 RCDATA \"pipe_server.dll\"\n")
# add_custom_command(
#     DEPENDS ${DLL_RC}
#     OUTPUT ${DLL_RES}
#     COMMAND ${RC_COMPILER} -i ${DLL_RC} -o ${DLL_RES}
# )
# target_link_libraries(${PROJECT_NAME} PRIVATE ${DLL_RES})
