cmake_minimum_required(VERSION 3.6)
project(SystemInfoDetector C)

set(CMAKE_C_STANDARD 99)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
        src/main.c
        src/system_info.c
        src/hardware_info.c
        src/utils.c
        )

# 主可执行文件
add_executable(SystemInfoDetector ${SOURCES})

# Windows特定设置
if(WIN32)
    target_link_libraries(SystemInfoDetector
            kernel32
            user32
            advapi32
            netapi32
            ws2_32
            iphlpapi
            pdh
            )
endif()

# 测试可执行文件
add_executable(SystemInfoTest
        tests/test_main.c
        src/system_info.c
        src/hardware_info.c
        src/utils.c
        )

if(WIN32)
    target_link_libraries(SystemInfoTest
            kernel32
            user32
            advapi32
            netapi32
            ws2_32
            iphlpapi
            pdh
            )
endif()