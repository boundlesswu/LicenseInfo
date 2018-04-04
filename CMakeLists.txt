cmake_minimum_required(VERSION 3.7)
project(LicenseInfo)

set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

LINK_DIRECTORIES(".")

set(SOURCE_FILES_LIB AES.cpp AES.h LicenseInfo.cpp LicenseInfo.h)
set(SOURCE_FILES_GENERATE  ${SOURCE_FILES_LIB} generateLicenseFile.cpp)
add_library(LicenseInfo SHARED ${SOURCE_FILES_LIB})
add_executable(libTest main.cpp)
target_link_libraries(libTest LicenseInfo)
add_executable(generateLicense ${SOURCE_FILES_GENERATE})
target_link_libraries(generateLicense LicenseInfo)
add_executable(parseLicense parseLicense.cpp)
target_link_libraries(parseLicense LicenseInfo)