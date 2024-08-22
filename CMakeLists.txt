cmake_minimum_required(VERSION 3.5.0)
project(WebServer VERSION 0.1.0 LANGUAGES C CXX)


add_executable(WebServer
 main.cpp
 src/TcpServer.cpp
 )

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(
  htest
  test/test.cpp
)
target_link_libraries(
  htest
  GTest::gtest_main
)

add_test(htest htest)

include(GoogleTest)
gtest_discover_tests(htest)