cmake_minimum_required(VERSION 3.16)
project(KungFuChess LANGUAGES CXX C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(FetchContent)

# ---------------------------------------------------------
# הגדרה והורדה אוטומטית של ixWebSocket
# ---------------------------------------------------------
FetchContent_Declare(
    ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG        v11.4.5
)

# ביטול TLS/OpenSSL וטסטים לחיסכון בזמן קומפילציה
set(USE_TLS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(ixwebsocket)
# ---------------------------------------------------------

# ---------------------------------------------------------
# הגדרה והורדה אוטומטית של SQLite3 (Amalgamation)
# ---------------------------------------------------------
FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2024/sqlite-amalgamation-3450200.zip
)
FetchContent_MakeAvailable(sqlite3)
# ---------------------------------------------------------

# הגדרת נתיבי חיפוש מפורשים עבור Qt6 ב-MSYS2 UCRT64
list(APPEND CMAKE_PREFIX_PATH 
    "C:/msys64/ucrt64"
    "C:/msys64/ucrt64/lib/cmake"
    "C:/msys64/ucrt64/lib/cmake/Qt6"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# מציאת ספריית OpenCV שהותקנה דרך MSYS2
find_package(OpenCV REQUIRED)

# Find Qt6
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# 1. איסוף אוטומטי של כל קובצי ה-cpp בתוך src וכל תת-התיקיות
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# יצירת ה-Executable עם main.cpp, כל קובצי המקור שנמצאו, וקובץ ה-C של SQLite שהורד אוטומטית
add_executable(app main.cpp ${SOURCES} ${sqlite3_SOURCE_DIR}/sqlite3.c)

# 2. איסוף אוטומטי של כל תת-התיקיות בתוך src המכילות קובצי hpp / h
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp" "src/*.h")
set(SRC_INCLUDE_DIRS "")
foreach(HEADER_FILE ${HEADERS})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND SRC_INCLUDE_DIRS ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES SRC_INCLUDE_DIRS)

target_include_directories(app PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${SRC_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${sqlite3_SOURCE_DIR}
)

# קישור הספריות של OpenCV, Qt6 ו-ixwebsocket
target_link_libraries(app PRIVATE 
    ${OpenCV_LIBS}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ixwebsocket
)

# Copy OpenCV DLLs to build directory
if(WIN32)
    foreach(OpenCV_LIB ${OpenCV_LIBS})
        get_filename_component(OpenCV_LIB_DIR ${OpenCV_LIB} DIRECTORY)
        file(GLOB OpenCV_DLLS "${OpenCV_LIB_DIR}/../bin/*.dll")
        foreach(DLL ${OpenCV_DLLS})
            add_custom_command(TARGET app POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL}" "$<TARGET_FILE_DIR:app>/"
                VERBATIM
            )
        endforeach()
    endforeach()
    
    # Copy Qt6 DLLs to build directory
    get_target_property(Qt6_Core_Location Qt6::Core IMPORTED_LOCATION)
    get_filename_component(Qt6_BIN_DIR ${Qt6_Core_Location} DIRECTORY)
    
    file(GLOB Qt6_DLLS 
        "${Qt6_BIN_DIR}/Qt6Core.dll"
        "${Qt6_BIN_DIR}/Qt6Gui.dll"
        "${Qt6_BIN_DIR}/Qt6Widgets.dll"
        "${Qt6_BIN_DIR}/Qt6Network.dll"
        "${Qt6_BIN_DIR}/Qt6Sql.dll"
        "${Qt6_BIN_DIR}/Qt6Concurrent.dll"
        "${Qt6_BIN_DIR}/Qt6DBus.dll"
    )
    
    foreach(DLL ${Qt6_DLLS})
        if(EXISTS ${DLL})
            add_custom_command(TARGET app POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL}" "$<TARGET_FILE_DIR:app>/"
                VERBATIM
            )
        endif()
    endforeach()
    
    # Copy Qt6 platform plugin
    file(GLOB Qt6_PLUGINS 
        "${Qt6_BIN_DIR}/../plugins/platforms/qwindows.dll"
    )
    
    foreach(PLUGIN ${Qt6_PLUGINS})
        if(EXISTS ${PLUGIN})
            add_custom_command(TARGET app POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:app>/platforms"
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PLUGIN}" "$<TARGET_FILE_DIR:app>/platforms/"
                VERBATIM
            )
        endif()
    endforeach()
endif()

if(MSVC)
    target_compile_options(app PRIVATE /EHsc /MD)
endif()