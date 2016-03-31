cmake_minimum_required(VERSION 3.4)

project(tiny_obj_loader)

set(SOURCE_FILES tiny_obj_loader.cc)

add_library(tiny_obj_loader_lib STATIC ${SOURCE_FILES})
