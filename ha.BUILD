package(default_visibility = ["//visibility:public"])

# yum install libaio-devel snappy libzstd-devel lz4-devel zlib-devel 

# /usr/include/lz4hc.h
#define LZ4HC_MIN_CLEVEL        3
#define LZ4HC_DEFAULT_CLEVEL    9
#define LZ4HC_MAX_CLEVEL        16

cc_library(
    name = "alib",
    srcs = [
            "alib/autil/libautil.a",
            "alib/anet/libanet.a",
            "alib/alog/libalog.a",
            "alib/fslib/libfslib-framework.a",
            "alib/fslib/libfslib_plugin_hdfs.a",
            "alib/fslib/libfslib_plugin_http.a",
            "alib/kmonitor/libkmonitor_client.a",
            "alib/kmonitor_adapter/libkmonitor_adapter.a",
            "alib/future_lite/libfuture_lite.a",
            "alib/beeper/libbeeper-framework.a",
            "alib/beeper/libbeeper_plugin_sqi.a",
        ],
    hdrs = glob([
            "alib/autil/*.h",
            "alib/autil/**/*.h",
            "alib/anet/*.h",
            "alib/alog/*.h",
            "alib/fslib/*.h",
            "alib/fslib/**/*.h",
            "alib/kmonitor/**/*.h",
            "alib/kmonitor_adapter/*.h",
            "alib/future_lite/*.h",
            "alib/future_lite/**/*.h",
            "alib/beeper/*.h",
            "alib/beeper/**/*.h",
        ]),
    alwayslink = 1,
    copts = ["-std=c++14"],
    linkopts = [
        "-pthread",
        "-lrt",
        "-lstdc++",
    ],
    defines = [],
    strip_include_prefix = "alib",
    visibility = ["//visibility:public"],
    deps = [
    ],
)

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
        ":alib"
    ],
)