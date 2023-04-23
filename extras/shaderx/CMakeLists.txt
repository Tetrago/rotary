cmake_minimum_required(VERSION 3.8)
project(shaderx)

add_executable(shaderx
	"src/main.cpp"
)

target_link_libraries(shaderx PRIVATE argparse::argparse shcc)
