# Set CMAKE_INSTALL_* if not defined

# NEEDED VARIABLES: 
#   PROJECT_NAME
#   PROJECT_VERSION_STRING
#   PROJECT_TARGET
#   PROJECT_INCLUDE_PREFIX

include(GNUInstallDirs)

set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_TARGET}")

add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION_STRING} COMPATIBILITY AnyNewerVersion)

string(REPLACE "." ";" VERSION_LIST ${PROJECT_VERSION_STRING})
list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)

configure_package_config_file("cmake/packageConfig.cmake.in" "${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/bin"
        PATH_VARS CMAKE_INSTALL_INCLUDEDIR
        )

configure_file("cmake/cmakeUninstall.cmake" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
configure_file("cmake/package.pc.in" "${PROJECT_NAME}.pc" @ONLY)

install(TARGETS ${PROJECT_TARGET}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_INCLUDE_PREFIX}"
        )

# install include files
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_INCLUDE_PREFIX}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
if (USE_JSON)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/json/${PROJECT_INCLUDE_PREFIX}
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
endif ()


install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_TARGET}::
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        )

# Set library pkgconfig file for facilitating 3rd party integration
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        )

install(CODE "execute_process(COMMAND ldconfig)")
