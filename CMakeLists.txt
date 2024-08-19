cmake_minimum_required (VERSION 2.8.11)

set(PS_WORKSPACE_PATH "${CMAKE_SOURCE_DIR}/../../")
set(PS_BUILD_PATH "${PS_WORKSPACE_PATH}/ps_build")


set(PS_BUILD_PATH "${PS_WORKSPACE_PATH}/ps_build")

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
#CPMUsePackageLock(package-lock.cmake)

#set(CPM_Toys_SOURCE ${PS_WORKSPACE_PATH}/yfh/toys)
CPMAddPackage (
    NAME Toys
    GIT_REPOSITORY  "https://github.com/nereus-yfh/toys"
    SOURCE_DIR ${PS_WORKSPACE_PATH}/yfh/toys
    BINARY_DIR  ${PS_BUILD_PATH}/yfh/toys
    GIT_TAG "dev"
)

#set(CPM_yfh_Justfortest_SOURCE ${PS_WORKSPACE_PATH}/yfh/justfortest)
CPMAddPackage (
    NAME yfh_Justfortest 
    GIT_REPOSITORY  "https://github.com/nereus-yfh/justfortest"
    SOURCE_DIR ${PS_WORKSPACE_PATH}/yfh/justfortest
    BINARY_DIR  ${PS_BUILD_PATH}/yfh/justfortest
    GIT_TAG "dev"
)

PSAddPackage (
    PSMOUDLE "nereus-yfh/justfortest"
    GIT_TAG "dev"
)

PSAddPackage_dowload (
    PSMOUDLE "nereus-yfh/justfortest"
    GIT_TAG "dev"
)

find_package(

)
add_library()

