cmake_minimum_required(VERSION 3.20)
project("cache")

enable_testing()

option(WITH_SET_HIRS_SIZE "Enable setting the size of the Q list (resident HIRS list)" OFF)
option(WITH_DEBUG_OUTPUT  "Enable debug output (cache dumps on each request)" OFF)

add_library(cachelib INTERFACE)
target_include_directories(cachelib INTERFACE "cachelib")

add_executable(lirs lirs.cpp)
add_executable(ideal ideal.cpp)

if (WITH_SET_HIRS_SIZE)
    target_compile_definitions(lirs PUBLIC SET_HIRS_SIZE)
endif()

if (WITH_DEBUG_OUTPUT)
    target_compile_definitions(lirs PUBLIC DEBUG_OUTPUT)
    target_compile_definitions(ideal PUBLIC DEBUG_OUTPUT)
endif()

target_link_libraries(lirs cachelib)
target_link_libraries(ideal cachelib)

file(GLOB testfiles "tests/*.dat")

foreach(file ${testfiles})
    get_filename_component(testname ${file} NAME_WE)
    add_test(
        NAME lirs.${testname}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/run_test.py ${file} lirs ./lirs
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(lirs.${testname} PROPERTIES DEPENDS lirs LABELS lirs)
endforeach()

foreach(file ${testfiles})
    get_filename_component(testname ${file} NAME_WE)
    add_test(
        NAME ideal.${testname}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/run_test.py ${file} ideal ./ideal
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(ideal.${testname} PROPERTIES DEPENDS ideal LABELS ideal)
endforeach()
