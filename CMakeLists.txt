cmake_minimum_required(VERSION 3.5.0)
project(VitalSenderProcessor VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB SOURCES "Sender/*.cpp")
add_executable(
    sender
    ${SOURCES}
)

file(GLOB SOURCES "Processor/src/*.cpp")
add_executable(
    processor
    ${SOURCES}
)


