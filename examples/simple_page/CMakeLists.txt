set(SRC main.cpp)

set(PACKAGE simple_page)

add_executable(${PACKAGE} ${SRC} ${RESOURCES})

include_directories("../../src")

target_link_libraries(${PACKAGE} PUBLIC
       Qt6::Core
       Qt6::Network
       Qt6::Sql
       Qt6::Xml
       -L ../../src -lqtwebserver-qt6)

install(TARGETS ${PACKAGE} RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR})
