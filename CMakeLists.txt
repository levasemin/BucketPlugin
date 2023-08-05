project(TOOLPAINT)
cmake_minimum_required(VERSION 3.25)

include(FetchContent)

FetchContent_Declare(
  PluginStandart
  GIT_REPOSITORY https://github.com/levasemin/PluginStandart.git
  GIT_TAG        master
)

FetchContent_GetProperties(PluginStandart)
FetchContent_Populate(PluginStandart)

include_directories(build/_deps/pluginstandart-src)

file(GLOB brush_SRC "src/*.cpp")
file(GLOB brush_LIB "include/*.hpp")

add_library(bucket_tool SHARED ${brush_SRC} ${brush_LIB})
add_compile_options(-fPIC)
target_include_directories(bucket_tool PUBLIC include)