cmake_minimum_required(VERSION 3.30)
project(FGameplayTagParser)

set(CMAKE_CXX_STANDARD 20)

add_executable(FGameplayTagParser main.cpp
        Components/gameplay_tag_tree_parser.cpp
        Components/gameplay_tag_tree_parser.h
        Components/TagNode.h
        Components/tag_node_tree_visitor.cpp
        Components/tag_node_tree_visitor.h
        Components/visitors/gameplay_tag_visitor.cpp
        Components/visitors/gameplay_tag_visitor.h
        Components/helpers.h
        Components/helpers.cpp
        Components/visitors/gameplay_tag_display_data.cpp
        Components/visitors/gameplay_tag_display_data.h
        Components/visitors/gameplay_tag_supplier_processor.cpp
        Components/visitors/gameplay_tag_supplier_processor.h
        Components/visitors/flags.h
        Components/visitors/SupplierAttributeFormatTemplates.h
        Components/visitors/gameplay_tag_scalar_field_accumulator.cpp
        Components/visitors/gameplay_tag_scalar_field_accumulator.h
        Components/visitors/ScalarFieldFormatTemplates.h
        Components/visitors/gameplay_tag_to_fragment_visitor.cpp
        Components/visitors/gameplay_tag_to_fragment_visitor.h
        Components/visitors/FieldAttributeProcessorTemplates.h
        Components/visitors/gameplay_tag_apply_field_to_attribute.cpp
        Components/visitors/gameplay_tag_apply_field_to_attribute.h
)

