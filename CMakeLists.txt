cmake_minimum_required(VERSION 3.5)	

project(magic_tree LANGUAGES CXX)	

find_package(Qt5 REQUIRED COMPONENTS Widgets)	

set(CMAKE_CXX_STANDARD 11)	
set(CMAKE_CXX_STANDARD_REQUIRED ON)	

add_library(magic_tree SHARED	
    tree/cacheobject.cpp	
    tree/cacheobject.h	
    tree/storecacheobject.cpp	
    tree/storecacheobject.h	
    tree/tree.cpp	
    tree/tree.h	
    tree/treeitem.cpp	
    tree/treeitem.h	

    itemmodel.cpp	
    itemmodel.h)	

target_link_libraries(magic_tree Qt5::Widgets)