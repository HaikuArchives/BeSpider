if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

set(EXECUTABLE_OUTPUT_PATH ${BESPIDER_BINARY_DIR})

set(BESPIDER_SRCS
	SpiderApplication.cpp
	SpiderWindow.cpp
	SpiderView.cpp
)

add_executable(BeSpider ${BESPIDER_SRCS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-multichar -Woverloaded-virtual -Wno-unknown-pragmas -std=c++0x -lroot -lbe -ltranslation")

target_link_libraries(BeSpider "be" "tracker")
