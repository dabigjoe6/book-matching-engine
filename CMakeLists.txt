cmake_minimum_required(VERSION 3.29.0)

set(PROJECT_NAME LimitOrderBook)

project(LimitOrderBook VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(Headers
	./src/Order/order.h
	./src/Limit/limit.h
	./src/OrderBook/order_book.h
	./src/OrderGenerator/order_generator.h
)

set(Sources
	./src/Order/order.cpp
	./src/Limit/limit.cpp
	./src/OrderBook/order_book.cpp
	./src/OrderGenerator/order_generator.cpp
)

# Define the library target
add_library(${PROJECT_NAME}_lib STATIC ${Sources} ${Headers})

# Define the executable target
add_executable(${PROJECT_NAME} ./src/main.cpp)

# Link the executable with the library
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
