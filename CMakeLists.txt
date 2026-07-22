cmake_minimum_required(VERSION 3.16)
project(KungFuChess LANGUAGES CXX)

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

add_executable(app
    main.cpp
    
    # קובץ מתיקיית application:
    src/application/Application.cpp

    # GUI ו-Logic:
    src/gui/graphics/Img.cpp
    src/gui/window/ImgWindow.cpp
    src/gui/window/Window.cpp
    src/gui/resources/Sprite.cpp
    src/gui/resources/SpriteKey.cpp
    src/gui/resources/TextureManager.cpp
    src/gui/config/GuiConfig.cpp
    src/gui/config/Layout.cpp
    src/gui/rendering/BoardRenderer.cpp
    src/gui/rendering/PieceRenderer.cpp
    src/gui/rendering/GameRenderer.cpp
    src/logic/snapshot/GameSnapshot.cpp
    src/logic/config/GameConfig.cpp
    src/logic/engine/GameEngine.cpp
    src/logic/engine/PieceEngine.cpp
    src/logic/engine/RuleEngine.cpp
    src/logic/model/Board.cpp
    src/logic/realtime/RealTimeArbiter.cpp
    src/logic/realtime/Jump.cpp
    src/logic/rules/RuleKing.cpp
    src/logic/rules/RuleQueen.cpp
    src/logic/rules/RuleRook.cpp
    src/logic/rules/RuleBishop.cpp
    src/logic/rules/RuleKnight.cpp
    src/logic/rules/RulePawn.cpp
    src/logic/rules/RuleUtils.cpp
    src/gui/loop/GameLoop.cpp
    src/logic/setup/BoardInitializer.cpp
    src/logic/input/GameController.cpp
    src/gui/Input/MouseInput.cpp
    src/logic/snapshot/GameSnapshotBuilder.cpp
    src/gui/rendering/SelectionRenderer.cpp
    src/gui/resources/Animation.cpp
    src/gui/animation/AnimationPlayer.cpp
    src/logic/realtime/Rest.cpp
    src/logic/engine/MoveExecutor.cpp
    src/logic/engine/JumpManager.cpp
    src/logic/engine/RestManager.cpp
    src/logic/engine/MotionManager.cpp
    src/logic/engine/GameOverHandler.cpp
    src/logic/engine/PromotionHandler.cpp
    
    # קובץ חדש - Server:
    src/server/Server.cpp

    # קבצים מתיקיית server/Logging:
    src/server/Logging/AsyncLogger.cpp
    src/server/Logging/LogFormatter.cpp
    
    # קבצים מתיקיית server/core:
    src/server/core/GameSession.cpp
    src/server/core/PlayerSession.cpp
    src/server/core/commands/ClickCommand.cpp
    src/server/core/commands/CommandHandler.cpp
    src/server/core/commands/CommandParser.cpp
    
    # קבצים מתיקיית server/infrastructure:
    src/server/infrastructure/config/Config.cpp
    src/server/infrastructure/config/ConfigLoader.cpp
    src/server/infrastructure/events/EventBus.cpp
    src/server/infrastructure/taskScheduler/TaskScheduler.cpp

    # קובץ מתיקיית server/network:
    src/server/network/ConnectionHandler.cpp
)

target_include_directories(app PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application"

    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/graphics"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/window"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/resources"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/rendering"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/common"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/model"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/rules"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/realtime"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/snapshot"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/setup"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/io"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/loop"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/input"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/logic/input"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/animation"
    
    # תיקיות שרת:
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Logging"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/core/commands"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/infrastructure/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/infrastructure/events"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/infrastructure/taskScheduler"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/network"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/protocol"
    
    ${OpenCV_INCLUDE_DIRS}
)

# קישור הספריות האוטומטי של OpenCV ו-Qt6
target_link_libraries(app PRIVATE 
    ${OpenCV_LIBS}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
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