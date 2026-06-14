cmake_minimum_required(VERSION 3.16)

project(loxsort VERSION 0.1.0 LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(LOXSORT_ENABLE_SHELL "Enable Shell sort" ON)
option(LOXSORT_ENABLE_INTRO "Enable introsort" ON)
option(LOXSORT_ENABLE_MERGE "Enable merge sort" ON)
option(LOXSORT_ENABLE_CYCLE "Enable cycle sort" OFF)
option(LOXSORT_ENABLE_VERIFY "Enable post-sort verification" OFF)
option(LOXSORT_BUILD_TESTS "Build tests" ON)
option(LOXSORT_BUILD_BENCH "Build benchmark tools" OFF)
option(LOXSORT_BUILD_FUZZ "Build fuzz targets" OFF)
option(LOXSORT_WARNINGS_AS_ERRORS "Treat warnings as errors for this project" ON)
option(LOXSORT_ENABLE_SANITIZERS "Enable ASan/UBSan for supported Clang toolchains" OFF)

set(LOXSORT_CLANG_RESOURCE_DIR "")
if(CMAKE_C_COMPILER_ID MATCHES "Clang")
    execute_process(
        COMMAND "${CMAKE_C_COMPILER}" --print-resource-dir
        OUTPUT_VARIABLE LOXSORT_CLANG_RESOURCE_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

add_library(loxsort
    src/lox_api.c
    src/lox_dispatch.c
    src/lox_features.c
    src/lox_profile.c
    src/lox_registry.c
    src/lox_swap.c
    src/algorithms/lox_cycle.c
    src/algorithms/lox_insertion.c
    src/algorithms/lox_intro.c
    src/algorithms/lox_merge.c
    src/algorithms/lox_shell.c
)

target_include_directories(loxsort
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

target_compile_features(loxsort PUBLIC c_std_99)
target_compile_definitions(loxsort PRIVATE
    LOXSORT_ENABLE_SHELL=$<BOOL:${LOXSORT_ENABLE_SHELL}>
    LOXSORT_ENABLE_INTRO=$<BOOL:${LOXSORT_ENABLE_INTRO}>
    LOXSORT_ENABLE_MERGE=$<BOOL:${LOXSORT_ENABLE_MERGE}>
    LOXSORT_ENABLE_CYCLE=$<BOOL:${LOXSORT_ENABLE_CYCLE}>
    LOXSORT_ENABLE_VERIFY=$<BOOL:${LOXSORT_ENABLE_VERIFY}>
)

if(LOXSORT_ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_options(loxsort PRIVATE -fsanitize=address,undefined)
        target_link_libraries(loxsort INTERFACE
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.ubsan_standalone-x86_64.lib"
        )
    endif()
endif()

function(loxsort_apply_clang_sanitizers target_name)
    if(LOXSORT_ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined)
        target_link_libraries(${target_name} PRIVATE
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.ubsan_standalone-x86_64.lib"
        )
    endif()
endfunction()

if(MSVC)
    target_compile_options(loxsort PRIVATE /W4)
    if(LOXSORT_WARNINGS_AS_ERRORS)
        target_compile_options(loxsort PRIVATE /WX)
    endif()
else()
    target_compile_options(loxsort PRIVATE -Wall -Wextra -Wpedantic)
    if(LOXSORT_WARNINGS_AS_ERRORS)
        target_compile_options(loxsort PRIVATE -Werror)
    endif()
endif()

add_library(loxsort::loxsort ALIAS loxsort)

if(LOXSORT_BUILD_TESTS)
    enable_testing()
    add_executable(loxsort_tests
        tests/test_main.c
        tests/test_support.c
        tests/test_api.c
        tests/test_algorithms.c
        tests/test_dispatch.c
        tests/test_features.c
        tests/test_profile.c
        tests/test_stability.c
        tests/test_generators.c
        bench/lox_generators.c
    )
    target_link_libraries(loxsort_tests PRIVATE loxsort)
    target_include_directories(loxsort_tests PRIVATE src generated bench)
    target_compile_features(loxsort_tests PRIVATE c_std_99)
    if(LOXSORT_ENABLE_SANITIZERS AND CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_compile_options(loxsort_tests PRIVATE -fsanitize=address,undefined)
    endif()
    if(MSVC)
        target_compile_options(loxsort_tests PRIVATE /W4)
        if(LOXSORT_WARNINGS_AS_ERRORS)
            target_compile_options(loxsort_tests PRIVATE /WX)
        endif()
    else()
        target_compile_options(loxsort_tests PRIVATE -Wall -Wextra -Wpedantic)
        if(LOXSORT_WARNINGS_AS_ERRORS)
            target_compile_options(loxsort_tests PRIVATE -Werror)
        endif()
    endif()
    add_test(NAME loxsort_tests COMMAND loxsort_tests)
    loxsort_apply_clang_sanitizers(loxsort_tests)
endif()

if(LOXSORT_BUILD_BENCH)
    add_executable(loxsort_bench
        bench/lox_bench_main.c
        bench/lox_campaign.c
        bench/lox_bench.c
        bench/lox_generators.c
    )
    if(WIN32)
        target_sources(loxsort_bench PRIVATE bench/lox_clock_windows.c)
    else()
        target_sources(loxsort_bench PRIVATE bench/lox_clock_posix.c)
    endif()
    target_link_libraries(loxsort_bench PRIVATE loxsort)
    target_include_directories(loxsort_bench PRIVATE src generated)
    target_compile_features(loxsort_bench PRIVATE c_std_99)
    loxsort_apply_clang_sanitizers(loxsort_bench)
endif()

if(LOXSORT_BUILD_FUZZ)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "LOXSORT_BUILD_FUZZ requires Clang or clang-cl")
    endif()
    add_executable(loxsort_fuzz_public_api
        fuzz/fuzz_main.c
        fuzz/fuzz_public_api.c
    )
    add_executable(loxsort_fuzz_algorithms
        fuzz/fuzz_main.c
        fuzz/fuzz_algorithms.c
    )
    foreach(fuzz_target IN ITEMS loxsort_fuzz_public_api loxsort_fuzz_algorithms)
        target_link_libraries(${fuzz_target} PRIVATE loxsort)
        target_include_directories(${fuzz_target} PRIVATE src generated)
        target_compile_features(${fuzz_target} PRIVATE c_std_99)
        target_compile_options(${fuzz_target} PRIVATE -fsanitize=address,undefined -fsanitize=fuzzer-no-link)
        target_link_libraries(${fuzz_target} PRIVATE
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.asan_dynamic_runtime_thunk-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.ubsan_standalone-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.fuzzer_no_main-x86_64.lib"
            "${LOXSORT_CLANG_RESOURCE_DIR}/lib/windows/clang_rt.fuzzer_interceptors-x86_64.lib"
        )
        if(MSVC)
            target_compile_options(${fuzz_target} PRIVATE /W4)
            if(LOXSORT_WARNINGS_AS_ERRORS)
                target_compile_options(${fuzz_target} PRIVATE /WX)
            endif()
        else()
            target_compile_options(${fuzz_target} PRIVATE -Wall -Wextra -Wpedantic)
            if(LOXSORT_WARNINGS_AS_ERRORS)
                target_compile_options(${fuzz_target} PRIVATE -Werror)
            endif()
        endif()
    endforeach()
    enable_testing()
    add_test(NAME fuzz_public_api
        COMMAND loxsort_fuzz_public_api -runs=1000 ${CMAKE_CURRENT_SOURCE_DIR}/fuzz/corpus/public_api
    )
    add_test(NAME fuzz_algorithms
        COMMAND loxsort_fuzz_algorithms -runs=1000 ${CMAKE_CURRENT_SOURCE_DIR}/fuzz/corpus/algorithms
    )
endif()

install(TARGETS loxsort EXPORT loxsortTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT loxsortTargets
    NAMESPACE loxsort::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loxsort
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/loxsortConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/loxsortConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/loxsortConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loxsort
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/loxsortConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/loxsortConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/loxsort
)
