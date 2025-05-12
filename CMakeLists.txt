cmake_minimum_required(VERSION 3.30)
project(FGameplayTagParser)

set(CMAKE_CXX_STANDARD 20)

add_executable(FGameplayTagParser main.cpp
        Components/gameplay_tag_generator.cpp
        Components/gameplay_tag_generator.h
        Components/TagNode.h
        Components/tag_node_tree_visitor.cpp
        Components/tag_node_tree_visitor.h
)

