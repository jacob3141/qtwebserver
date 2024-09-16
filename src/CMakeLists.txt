set(PACKAGE qtwebserver-qt6)

set(SOURCES
    http/httprequest.cpp
    http/httpstatuscodes.cpp
    http/httpwebengine.cpp
    tcp/tcpmultithreadedserver.cpp
    tcp/tcpserverthread.cpp
    misc/log.cpp
    misc/logger.cpp
    http/httpresource.cpp
    http/httpiodeviceresource.cpp
    http/bytearrayresource.cpp
    sql/sqlconnectionpool.cpp
    html/htmldocument.cpp
    util/utilassetsresource.cpp
    http/httpresponse.cpp
    http/httpheaders.cpp
    util/utildataurlcodec.cpp
    util/utilformurlcodec.cpp
    css/cssdocument.cpp
    css/cssruleset.cpp
    weblayout.cpp)

set(HEADERS
    http/httprequest.h
    http/httpstatuscodes.h
    http/httpwebengine.h
    tcp/tcpserverthread.h
    tcp/tcpmultithreadedserver.h
    tcp/tcpresponder.h
    misc/threadsafety.h
    misc/logger.h
    misc/log.h
    http/httpresource.h
    http/bytearrayresource.h
    http/httpiodeviceresource.h
    sql/sqlconnectionpool.h
    html/htmldocument.h
    util/utilassetsresource.h
    http/httpresponse.h
    http/httpheaders.h
    util/utildataurlcodec.h
    util/utilformurlcodec.h
    css/cssdocument.h
    css/cssruleset.h
    webwidget.h
    weblayout.h)

add_library(${PACKAGE} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PACKAGE}
    PROPERTIES VERSION ${CMAKE_PROJECT_VERSION}
    SOVERSION ${CMAKE_PROJECT_VERSION})

target_link_libraries(${PACKAGE} PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::Xml)

configure_file(${PACKAGE}.pc.in ${PACKAGE}.pc @ONLY)

install(TARGETS ${PACKAGE} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PACKAGE})
