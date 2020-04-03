# 将base目录下的文件存储到base_SRCS变量中去
aux_source_directory(. base_SRCS)

# add_library命令 base_SRCS文件编译为静态链接库，链接库的名字为第一个参数
add_library(annotate_muduo_base ${base_SRCS})

# target_link_libraries 指明可执行文件 需要连接的链接库名字
target_link_libraries(annotate_muduo_base pthread rt)

# 指定库的安装路径，将annotate_muduo_base 安装到lib中
install(TARGETS annotate_muduo_base DESTINATION lib)

# 指定.h的安装路径
file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/annotate_muduo/base)

if(MUDUO_BUILD_EXAMPLES)
    add_subdirectory(tests)
endif()
