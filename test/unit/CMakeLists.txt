# All unit test executables will be in test/unit/bin
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

INCLUDE_DIRECTORIES(.) # for test.hpp
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/karazeh)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/deps)

MACRO(ADD_TEST TEST)
  SET(TEST ${TEST}) # expose it for main.cpp.in expansion
  SET(TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}/${TEST}.hpp ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}/${TEST}.cpp )
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/main.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}/main.cpp)
  LIST(APPEND TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/${TEST}/main.cpp)
  ADD_EXECUTABLE(${TEST} ${TEST_SRCS})
  TARGET_LINK_LIBRARIES(${TEST} kzh)
ENDMACRO()

# ADD_TEST("md5_hasher_test")
# ADD_TEST("resources_test")
# ADD_TEST("file_operations_test")
# ADD_TEST("manifests_test")
# ADD_TEST("permissions_test")
# ADD_TEST("rdiff_patch_test")
FILE(GLOB_RECURSE SPECS "./*_test.cpp")

ADD_EXECUTABLE(${PROJECT_NAME}_unit_tests ${SPECS} main.cpp)
# TARGET_LINK_LIBRARIES(${PROJECT_NAME}_unit_tests ${GTEST_BOTH_LIBRARIES} kzh)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}_unit_tests ${GTEST_LIBRARIES} kzh)