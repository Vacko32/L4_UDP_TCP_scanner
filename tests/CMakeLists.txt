add_executable(tests
    test.cc
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Catch2 REQUIRED)
find_package(PkgConfig REQUIRED)
# Link Qt libraries if using Qt
target_link_libraries(tests Qt6::Widgets)
target_link_libraries(tests Catch2::Catch2)
target_link_libraries(tests Catch2::Catch2WithMain)
target_link_libraries(app ${PCAP_LIBRARIES})
catch_discover_tests(tests)