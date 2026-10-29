cmake_minimum_required(VERSION 3.16)

project(PatientCareManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(patient_care_manager main.cpp)
add_executable(second_year_patient_system second_year_patient_system.cpp)

if (MSVC)
    target_compile_options(patient_care_manager PRIVATE /W4 /permissive-)
    target_compile_options(second_year_patient_system PRIVATE /W4 /permissive-)
else()
    target_compile_options(patient_care_manager PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(second_year_patient_system PRIVATE -Wall -Wextra -Wpedantic)
endif()
