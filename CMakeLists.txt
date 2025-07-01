cmake_minimum_required(VERSION 3.16)
project(FinanceTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Try Qt6 first, fallback to Qt5
find_package(Qt6 COMPONENTS Core Gui Qml Quick QuickControls2 Sql REQUIRED QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Gui Qml Quick QuickControls2 Sql REQUIRED)
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/core/transaction.cpp
    ${SRC_DIR}/data/databasemanager.cpp
    ${SRC_DIR}/data/transactiondao.cpp
    ${SRC_DIR}/ui/viewmodels/transactionmodel.cpp
    finance_resources.qrc
)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

if(Qt6_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::QuickControls2 Qt6::Sql)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Sql)
endif() 