cmake_minimum_required(VERSION 3.1.0)


PROJECT(xcs_stock_op_project)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()


include_directories(${PROJECT_SOURCE_DIR})
include_directories(/usr/local/include)
include_directories(/usr/include)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(xcs_stock_op_project_train account.cpp binary_action.cpp condition.cpp  train_main.cpp util.cpp xcs_classifier_system.cpp 
binary_input.cpp config_magr2.cpp generic.cpp stock_env.cpp xcs_classifier.cpp xcs_random.cpp)

add_executable(xcs_stock_op_project_test account.cpp binary_action.cpp condition.cpp  test_main.cpp util.cpp xcs_classifier_system.cpp 
binary_input.cpp config_magr2.cpp generic.cpp stock_env.cpp xcs_classifier.cpp xcs_random.cpp)


add_executable(account_test account.cpp account_test.cpp)
target_link_libraries(account_test gtest)

