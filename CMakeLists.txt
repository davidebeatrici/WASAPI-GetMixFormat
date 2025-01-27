cmake_minimum_required(VERSION 3.15)

project(WASAPI-GetMixFormat)

add_executable(WASAPI-GetMixFormat
	"main.cpp"
	"COM.cpp"
	"Endpoint.cpp"
	"Enumerator.cpp"
)

if(MSVC)
  target_compile_options(WASAPI-GetMixFormat PRIVATE "/W4" "/WX")
else()
  target_compile_options(WASAPI-GetMixFormat PRIVATE "-Wall" "-Wextra" "-Wpedantic" "-Werror")
endif()
