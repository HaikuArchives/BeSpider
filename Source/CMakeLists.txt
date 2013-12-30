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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-multichar -Woverloaded-virtual -Wno-unknown-pragmas -std=c++98 -pedantic-errors -lroot -lbe -ltranslation -lgame")

target_link_libraries(BeSpider "be" "tracker")

add_custom_command(
	TARGET BeSpider
	POST_BUILD
	COMMAND rc -o "${BESPIDER_BINARY_DIR}/BeSpider.rsrc" "${CMAKE_SOURCE_DIR}/BeSpider.rdef"
	COMMAND xres -o "${BESPIDER_BINARY_DIR}/BeSpider" "${BESPIDER_BINARY_DIR}/BeSpider.rsrc"
	COMMAND mimeset -f "${BESPIDER_BINARY_DIR}/BeSpider"
)
