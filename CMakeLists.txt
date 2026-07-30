cmake_minimum_required(VERSION 3.16)
project(KungFuChess LANGUAGES CXX C)

option(KFC_SERVER_ONLY "Build only the authoritative server" OFF)

set(CMAKE_TLS_VERIFY OFF CACHE BOOL "" FORCE)
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

# Keep third-party tests disabled while registering this project's tests.
enable_testing()
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
if(WIN32 AND NOT KFC_SERVER_ONLY)
    list(APPEND CMAKE_PREFIX_PATH
        "C:/msys64/ucrt64"
        "C:/msys64/ucrt64/lib/cmake"
        "C:/msys64/ucrt64/lib/cmake/Qt6"
    )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

if(NOT KFC_SERVER_ONLY)
    # מציאת ספריית OpenCV שהותקנה דרך MSYS2
    find_package(OpenCV REQUIRED)

    # Find Qt6
    find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# ---------------------------------------------------------
# איסוף אוטומטי של נתיבי ה-Headers
# ---------------------------------------------------------
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.hpp" "src/*.h")
set(SRC_INCLUDE_DIRS "")
foreach(HEADER_FILE ${HEADERS})
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    list(APPEND SRC_INCLUDE_DIRS ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES SRC_INCLUDE_DIRS)

set(COMMON_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${SRC_INCLUDE_DIRS}
    ${sqlite3_SOURCE_DIR}
)

if(NOT KFC_SERVER_ONLY)
    list(APPEND COMMON_INCLUDES ${OpenCV_INCLUDE_DIRS})
endif()

# ---------------------------------------------------------
# חלוקת קובצי המקור לפי התיקיות
# ---------------------------------------------------------

# 1. קוד משותף אמיתי (ללא application/ ששייך לשרת)
file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS 
    "src/common/*.cpp"
    "src/logic/*.cpp"
)

# 2. קוד ייעודי לשרת (כולל application)
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS 
    "src/server/*.cpp"
    "src/application/*.cpp"
)

# 3. קוד ייעודי ללקוח (כולל gui)
file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS 
    "src/client/*.cpp"
    "src/gui/*.cpp"
)

# ---------------------------------------------------------
# הגדרת יעד השרת (Server Target)
# ---------------------------------------------------------
add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES} ${sqlite3_SOURCE_DIR}/sqlite3.c)

target_include_directories(server PRIVATE ${COMMON_INCLUDES})

target_link_libraries(server PRIVATE
    ixwebsocket
    Threads::Threads
)

if(NOT KFC_SERVER_ONLY)
    target_link_libraries(server PRIVATE
        ${OpenCV_LIBS}
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
    )
endif()

# ---------------------------------------------------------
# הגדרת יעד הלקוח (Client Target)
# ---------------------------------------------------------
if(NOT KFC_SERVER_ONLY)
add_executable(client ${CLIENT_SOURCES} ${COMMON_SOURCES})

target_include_directories(client PRIVATE ${COMMON_INCLUDES})

target_link_libraries(client PRIVATE 
    ${OpenCV_LIBS}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ixwebsocket
)

# Mirror runtime assets beside the client executable. Removing the destination
# first prevents deleted or renamed source assets from lingering in build output.
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
        "$<TARGET_FILE_DIR:client>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:client>/assets"
    COMMENT "Deploying client assets"
    VERBATIM
)

# Focused protocol/snapshot round-trip coverage without either application main.
add_executable(protocol_codec_tests
    src/client/snapshot/tests/ProtocolCodecRoundTripTests.cc
    src/client/snapshot/SnapshotDeserializer.cpp
    src/common/CommandResponseCodec.cpp
    src/common/NetworkMessage.cpp
    src/logic/snapshot/GameSnapshot.cpp
    src/server/network/SnapshotSerializer.cpp
)

target_include_directories(protocol_codec_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/snapshot"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/model"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/snapshot"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/network"
)

add_test(NAME protocol_codec_tests COMMAND protocol_codec_tests)

# Real localhost transport coverage. Reuse the production server sources
# without its executable entry point and only the headless client services
# needed to drive independent client stacks.
set(INTEGRATION_SERVER_SOURCES ${SERVER_SOURCES})
list(FILTER INTEGRATION_SERVER_SOURCES
    EXCLUDE REGEX "[/\\\\]main\\.cpp$")

file(GLOB_RECURSE INTEGRATION_CLIENT_SOURCES CONFIGURE_DEPENDS
    "src/client/auth/*.cpp"
    "src/client/commands/*.cpp"
    "src/client/network/*.cpp"
    "src/client/room/*.cpp"
    "src/client/session/*.cpp"
    "src/client/snapshot/*.cpp"
    "src/client/state/*.cpp"
)

add_executable(two_client_websocket_integration_tests
    src/integration/tests/TwoClientWebSocketIntegrationTests.cc
    src/client/game/RemoteBoardClickSink.cpp
    src/client/game/RemoteGameFrameSource.cpp
    src/gui/animation/AnimationPlayer.cpp
    src/gui/config/GuiConfig.cpp
    src/gui/config/Layout.cpp
    src/gui/graphics/Img.cpp
    src/gui/rendering/BoardRenderer.cpp
    src/gui/rendering/GameRenderer.cpp
    src/gui/rendering/PieceRenderer.cpp
    src/gui/rendering/SelectionRenderer.cpp
    src/gui/resources/Animation.cpp
    src/gui/resources/Sprite.cpp
    src/gui/resources/SpriteKey.cpp
    src/gui/resources/TextureManager.cpp
    ${INTEGRATION_SERVER_SOURCES}
    ${INTEGRATION_CLIENT_SOURCES}
    ${COMMON_SOURCES}
    ${sqlite3_SOURCE_DIR}/sqlite3.c
)

target_include_directories(two_client_websocket_integration_tests PRIVATE
    ${COMMON_INCLUDES}
)

target_link_libraries(two_client_websocket_integration_tests PRIVATE
    ${OpenCV_LIBS}
    ixwebsocket
)

target_compile_definitions(two_client_websocket_integration_tests PRIVATE
    KFC_TEST_ASSET_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

add_test(
    NAME two_client_websocket_integration_tests
    COMMAND two_client_websocket_integration_tests
)
set_tests_properties(two_client_websocket_integration_tests PROPERTIES
    TIMEOUT 60
)
endif()

# ---------------------------------------------------------
# העתקת DLLs של OpenCV ו-Qt6 עבור שני הקבצים
# ---------------------------------------------------------
if(WIN32 AND NOT KFC_SERVER_ONLY)
    set(ALL_TARGETS server client)

    foreach(TARGET_NAME ${ALL_TARGETS})
        # Copy OpenCV DLLs
        foreach(OpenCV_LIB ${OpenCV_LIBS})
            get_filename_component(OpenCV_LIB_DIR ${OpenCV_LIB} DIRECTORY)
            file(GLOB OpenCV_DLLS "${OpenCV_LIB_DIR}/../bin/*.dll")
            foreach(DLL ${OpenCV_DLLS})
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    VERBATIM
                )
            endforeach()
        endforeach()
        
        # Copy Qt6 DLLs
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
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/"
                    VERBATIM
                )
            endif()
        endforeach()
        
        # Copy Qt6 platform plugin
        file(GLOB Qt6_PLUGINS 
            "${Qt6_BIN_DIR}/../plugins/platforms/qwindows.dll"
            "${Qt6_BIN_DIR}/../share/qt6/plugins/platforms/qwindows.dll"
        )
        
        foreach(PLUGIN ${Qt6_PLUGINS})
            if(EXISTS ${PLUGIN})
                add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${TARGET_NAME}>/platforms"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${PLUGIN}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/platforms/"
                    VERBATIM
                )
            endif()
        endforeach()
    endforeach()
endif()

if(MSVC)
    target_compile_options(server PRIVATE /EHsc /MD)
    if(NOT KFC_SERVER_ONLY)
        target_compile_options(client PRIVATE /EHsc /MD)
        target_compile_options(protocol_codec_tests PRIVATE /EHsc /MD)
        target_compile_options(two_client_websocket_integration_tests PRIVATE /EHsc /MD)
    endif()
endif()
