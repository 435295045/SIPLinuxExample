cmake_minimum_required(VERSION 3.10.2)
project(sip_example)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_STANDARD 11)
## --- C++11 build flags ---
add_definitions(-D_GLIBCXX_USE_C99=1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug) #Release Debug
set(CM_BUILD_OPTION "arm_ez") #x86_64 arm arm_ez mips mips_gcc540

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -s")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall -s")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -s")   
endif ()

if (${CM_BUILD_OPTION} STREQUAL "x86_64")
        
elseif (${CM_BUILD_OPTION} STREQUAL "arm")
        #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR arm)
        set(tools /home/ubuntu/project/sip_linux_sdk/resources/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf)
        set(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                /home/ubuntu/project/sip_linux_sdk/libs/linux/arm/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/include
                /home/ubuntu/project/lvgl/linux_outdoor/main/libs/alsa/arm/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/arm/lib/libcurl.so
                /home/ubuntu/project/sip_linux_sdk/libs/linux/arm/lib/libsipsdk.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm/lib/libssl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm/lib/libcurl_license.so
                /home/ubuntu/project/lvgl/linux_outdoor/main/libs/alsa/arm/lib/libasound.so
        )

        set(lib_l "-lpthread -lm -ldl -lstdc++ -lrt")
elseif (${CM_BUILD_OPTION} STREQUAL "arm_ez")
        set(CMAKE_BUILD_RPATH "/data/libs")
        #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR arm)
        set(tools /home/ubuntu/project/sip_linux_sdk/resources/gcc-7.4.0-x86_64_arm-ez-linux-uclibcgnueabihf)
        set(CMAKE_C_COMPILER ${tools}/bin/arm-ez-linux-uclibcgnueabihf-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/arm-ez-linux-uclibcgnueabihf-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm_ez/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/arm_ez/lib/libcurl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/arm_ez/lib/libsipsdk.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/arm_ez/lib/libssl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/arm_ez/lib/libcurl_license.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/arm_ez/lib/libasound.so
        )
elseif (${CM_BUILD_OPTION} STREQUAL "mips_gcc540")
         #配置编译工具链
        set(CMAKE_SYSTEM_PROCESSOR mips)
        set(tools /home/ubuntu/project/sip_linux_sdk/resources/mips-gcc540-uclibc0.9.33.2-64bit-r3.3.0.smaller)
        set(CMAKE_C_COMPILER ${tools}/bin/mips-linux-uclibc-gcc)
        set(CMAKE_CXX_COMPILER ${tools}/bin/mips-linux-uclibc-g++)

        
        include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/main/src
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips_gcc540/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/mips_gcc540/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips_gcc540/include
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/mips_gcc540/include
        )

        set(LIB_FILES 
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl/mips_gcc540/lib/libcurl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/sip/mips_gcc540/lib/libsipsdk.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips_gcc540/lib/libcrypto.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/openssl/mips_gcc540/lib/libssl.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/curl_license/mips_gcc540/lib/libcurl_license.so
                ${CMAKE_CURRENT_SOURCE_DIR}/main/libs/alsa/mips_gcc540/lib/libasound.so
        )
endif ()

file(GLOB_RECURSE SOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/main/src/*.cpp
)

add_executable(sip_example
        ${SOURCES}
)

# 链接第三方静态库
target_link_libraries(sip_example 
        -Wl,--start-group ${LIB_FILES} -Wl,--end-group
)
