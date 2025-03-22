# ProjectInit

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/project-config.txt")
	message(FATAL_ERROR "Missing project-config.txt file!")
endif()

if (NOT DEFINED TARGET_TRIPLE OR "${TARGET_TRIPLE}" STREQUAL "")
	message(FATAL_ERROR "TARGET_TRIPLE is not set!")
endif()

message(STATUS "Build to ${TARGET_TRIPLE}")

include (CheckCPUVendor)

include(CheckOS)

include (ApplyProjectConfig)

find_package(Asio REQUIRED)
if (ASIO_FOUND)
	message(STATUS "Found Asio, version ${ASIO_VERSION}")
else()
	message(FATAL_ERROR "Asio not found!")
endif()

find_package(PFR REQUIRED)
if (PFR_FOUND)
	message(STATUS "Found PFR, version ${PFR_VERSION}")
else()
	message(FATAL_ERROR "PFR not found!")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")

	# Use LibreHardwareMonitor to detect sensors on Windows
	find_package(LHWM-Wrapper REQUIRED)
	if (LHWM-Wrapper_FOUND)
		message(STATUS "Found LHWM-Wrapper, version ${LHWM-Wrapper_VERSION}")
	else()
		message(FATAL_ERROR "LHWM-Wrapper not found!")
	endif()
endif()

find_package(SQLite3 REQUIRED)
if (SQLite3_FOUND)
	message(STATUS "Found SQLite3, version ${SQLite3_VERSION}")
else()
	message(FATAL_ERROR "SQLite3 not found!")
endif()