cmake_minimum_required(VERSION 3.28)

add_executable(Client)

target_sources(Client
    PRIVATE
        Client.cpp
        GetCpuInfo.cpp
        GetGpuInfo.cpp
        GetMachModel.cpp
        GetCpuSensorID.cpp
        GetGpuSensorID.cpp
        Sender.cpp
        DataHandler.cpp
)

set_target_properties(Client PROPERTIES CXX_STANDARD 23)
target_include_directories(Client PRIVATE "${CMAKE_SOURCE_DIR}/include" "${CONFIG_DIR}")

target_link_libraries(Client PRIVATE
        BOOST::PFR
        BOOST::ASIO
)

if (USE_LLVM EQUAL ON)
    configure_for_llvm(Client)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(Client PRIVATE "/D_WIN32_WINNT=0x0601" "/utf-8" "/MP" "/Zm80")

    target_link_libraries(Client
            PRIVATE
            LHWM::LHWM-WRAPPER
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        get_target_property(LHWM_DLL_PATH LHWM::LHWM-WRAPPER IMPORTED_LOCATION_DEBUG)
    else()
        get_target_property(LHWM_DLL_PATH LHWM::LHWM-WRAPPER IMPORTED_LOCATION_RELEASE)
    endif()

    add_custom_command(TARGET Client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_PROPERTY:LHWM::LHWM-WRAPPER,IMPORTED_LOCATION_$<UPPER_CASE:$<CONFIG>>>
            $<TARGET_FILE_DIR:Client>
            COMMENT "Copying LHWM-WRAPPER DLL to output directory"
    )

endif ()

install(TARGETS Client
        RUNTIME DESTINATION "${HEARTBEAT_INSTALL_PREFIX}/bin"
        LIBRARY DESTINATION "${HEARTBEAT_INSTALL_PREFIX}/lib"
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(
            FILES $<TARGET_PROPERTY:LHWM::LHWM-WRAPPER,IMPORTED_LOCATION_$<UPPER_CASE:$<CONFIG>>>
            DESTINATION "${HEARTBEAT_INSTALL_PREFIX}/bin"
    )
endif ()