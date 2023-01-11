package(default_visibility = ["//visibility:public"])

# yum install libaio-devel snappy libzstd-devel lz4-devel zlib-devel 

# /usr/include/lz4hc.h
#define LZ4HC_MIN_CLEVEL        3
#define LZ4HC_DEFAULT_CLEVEL    9
#define LZ4HC_MAX_CLEVEL        16

cc_library(
    name = "indexlib",
    srcs = glob([
            "aios/indexlib/indexlib/**/*.h",
            "aios/indexlib/indexlib/**/*.cpp",
        ], exclude = [
        "aios/indexlib/**/test/**",
        "aios/indexlib/indexlib/**/perf_test/**",
        "aios/indexlib/indexlib/**/simple_Polygon.cpp",
        "aios/indexlib/indexlib/testlib/**",
        "aios/indexlib/indexlib/**/demo/**",
        "aios/indexlib/indexlib/**/signature.cpp",
        "aios/indexlib/indexlib/**/*_mock.*",
        "aios/indexlib/indexlib/**/mockstorage/**",
        ]),
    hdrs = glob([
            "aios/indexlib/indexlib/*.h",
            "aios/indexlib/indexlib/**/*.h",
        ], exclude = [
        "aios/indexlib/indexlib/**/test/**",
        "aios/indexlib/indexlib/**/perf_test/**",
        "aios/indexlib/indexlib/testlib/**",
        "aios/indexlib/indexlib/**/*_mock.*",
        "aios/indexlib/indexlib/**/mockstorage/**",
        ]),
    copts = [
        "-std=c++14",
        "-msse4.2",
    ],
    defines = [
        "_GLIBCXX_USE_CXX11_ABI=0"
    ],
    alwayslink = 1,
    linkopts = [
        "-pthread",
        "-lrt",
        "-lstdc++",
    ],
    strip_include_prefix = "aios/indexlib",
    visibility = ["//visibility:public"],
    deps = [
        "@hello_havenask:alib"
    ],
)