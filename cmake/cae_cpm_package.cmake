CPMAddPackage (
    NAME eigen
    GIT_REPOSITORY  "https://gl.poissonsoft.com/yf22004/ps-eigen3"
    SOURCE_DIR ${PS_WORKSPACE_PATH}/yf22004/ps-eigen3
    BINARY_DIR  ${PS_BUILD_PATH}/yf22004/ps-eigen3
    GIT_TAG "main"
)

CPMAddPackage (
    NAME Libevent
    GIT_REPOSITORY  "https://github.com/libevent/libevent"
    SOURCE_DIR ${PS_WORKSPACE_PATH}/yf22004/ps-libevent
    BINARY_DIR  ${PS_BUILD_PATH}/yf22004/ps-libevent
    GIT_TAG "release-2.1.12-stable"
)

CPMAddPackage (
    NAME Hdf5
    GIT_REPOSITORY  "https://gl.poissonsoft.com/yf22004/ps-hdf5.git"
    SOURCE_DIR ${PS_WORKSPACE_PATH}/yf22004/ps-hdf5
    BINARY_DIR  ${PS_BUILD_PATH}/yf22004/ps-hdf5
    GIT_TAG "main"
)


get_target_property(hdf5_include_dirs Hdf5 INCLUDE_DIRECTORIES)
message(STATUS "Libevent include directories: ${hdf5_include_dirs}")



#set(hdf5_DIR ${hdf5_dir_SOURCE_DIR}/lib/linux/release/cmake)
#find_package(hdf5 REQUIRED)
message(STATUS "HDF5 " ${hdf5_INCLUDE_DIRS})
message(STATUS "HDF5 " ${HDF5_INCLUDE_DIRS})
message(STATUS "HDF5 " ${Hdf5_DIR})


message(STATUS "XXXX" ${hdf5_ADDED})
message(STATUS "XXXX" ${hdf5_BINARY_DIR})
message(STATUS "XXXX" ${hdf5_SOURCE_DIR})

message(STATUS "XXXX" ${Hdf5_FOUND})

message(STATUS "XXXX" ${HDF5_FOUND})
message(STATUS "XXXX" ${hdf5_FOUND})
message(STATUS "XXXX" ${hdf5_DIR})