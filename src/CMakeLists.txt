file(GLOB_RECURSE SRC_FILES *.cc)

add_executable(app main.cc parsearg.cc ${SRC_FILES})


find_package(Qt6 COMPONENTS Widgets REQUIRED)
target_link_libraries(app Qt6::Widgets)

