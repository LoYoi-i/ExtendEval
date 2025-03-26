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
set(RC_R ${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}.r)
set(RC_RR ${BIN_DIR}/${PROJECT_NAME}.rr)
set(RC_RRC ${BIN_DIR}/${PROJECT_NAME}.rrc)
set(RC_RC ${BIN_DIR}/${PROJECT_NAME}.rc)
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
