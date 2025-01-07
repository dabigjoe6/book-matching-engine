cmake_minimum_required(VERSION 3.29.0)

set(TEST_PROJECT_NAME LimitOrderBookTest)

set(Sources
		./LimitTest.cpp
)

add_executable(${TEST_PROJECT_NAME} ${Sources})
target_link_libraries(${TEST_PROJECT_NAME} PUBLIC gtest_main LimitOrderBook_lib)

add_test(
	NAME ${TEST_PROJECT_NAME}
	COMMAND ${TEST_PROJECT_NAME}
)
