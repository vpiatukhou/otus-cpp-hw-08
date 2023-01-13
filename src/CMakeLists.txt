set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

add_executable(bayan_cli main.cpp)

add_library(bayan
    DirectoryScanner.cpp
    File.cpp
    FileComparer.cpp
    FileImpl.cpp 
    HasherFactory.cpp
    Util.cpp)

#rename the executable to "bayan"
set_target_properties(bayan_cli PROPERTIES OUTPUT_NAME bayan)

target_link_libraries(bayan Boost::system Boost::filesystem Boost::program_options)
target_link_libraries(bayan_cli PRIVATE bayan)

#BEGIN release
install(TARGETS bayan_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
#END
