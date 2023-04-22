cmake_minimum_required(VERSION 3.8)
project(player)

add_executable(player
	"src/main.cpp"
)

target_link_libraries(player plat rotary vtk)
