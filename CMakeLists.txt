cmake_minimum_required(VERSION 3.16)

project(megaCppMap LANGUAGES C)

add_executable(megaCppMap main.c)

include(GNUInstallDirs)
install(TARGETS megaCppMap
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
