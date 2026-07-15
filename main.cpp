#include <iostream>
#include <vector>

#include "Img.hpp"

#include "GuiConfig.hpp"
#include "Layout.hpp"

#include "TextureManager.hpp"

#include "BoardRenderer.hpp"
#include "PieceRenderer.hpp"

#include "GameSnapshot.hpp"
#include "PieceSnapshot.hpp"
#include "GameRenderer.hpp"
#include "Window.hpp"


int main()
{
    std::cout
        << "==============================\n"
        << " LAYOUT + PIECE RENDER TEST\n"
        << "==============================\n\n";


    //---------------------------------
    // Create Canvas
    //---------------------------------

    Img canvas(
        GuiConfig::WINDOW_WIDTH,
        GuiConfig::WINDOW_HEIGHT
    );


    //---------------------------------
    // Create Layout
    //---------------------------------

    Layout layout(
        GuiConfig::WINDOW_WIDTH,
        GuiConfig::WINDOW_HEIGHT,
        8,
        8
    );


    //---------------------------------
    // Texture Manager
    //---------------------------------

    TextureManager textureManager;


    try
    {
        //---------------------------------
        // Load board first
        //---------------------------------

        textureManager.loadTexture(
            "board",
            GuiConfig::BOARD_TEXTURE_PATH
        );


        Img& board =
            textureManager.getTexture("board");


        //---------------------------------
        // Update layout from board size
        //---------------------------------

        layout.setBoardSize(
    GuiConfig::WINDOW_WIDTH - 40,
    GuiConfig::WINDOW_HEIGHT - 40);


        std::cout
            << "Board size : "
            << board.width()
            << " x "
            << board.height()
            << std::endl;


        std::cout
            << "Cell size  : "
            << layout.getCellSize()
            << std::endl;


        std::cout
            << "Offset X   : "
            << layout.getBoardOffsetX()
            << std::endl;


        std::cout
            << "Offset Y   : "
            << layout.getBoardOffsetY()
            << std::endl;



        //---------------------------------
        // Load piece AFTER layout update
        //---------------------------------

        textureManager.loadTexture(
            "WK_idle",
            "assets/pieces/WK/states/idle/sprites/1.png",
            layout.getCellSize(),
            layout.getCellSize()
        );


        std::cout
            << "[OK] Textures loaded\n";


    }
    catch(const std::exception& e)
    {
        std::cout
            << "Texture error:\n"
            << e.what()
            << std::endl;

        return 1;
    }



    //---------------------------------
    // Create Renderers
    //---------------------------------
GameRenderer renderer(
    layout,
    textureManager);



    //---------------------------------
    // Create Snapshot
    //---------------------------------

    std::vector<PieceSnapshot> pieces;


    PieceSnapshot king
    {
        1,
        Side::WHITE,
        PieceType::KING,
        Position(7,4),
        PieceState::IDLE
    };


    pieces.push_back(king);



    GameSnapshot snapshot(
        pieces,
        0,
        false
    );



    //---------------------------------
    // Render
    //---------------------------------

    try
    {
       renderer.render(
    canvas,
    snapshot);


        cv::imwrite(
            "render_test.png",
            canvas.get_mat()
        );


       Window window(
    GuiConfig::WINDOW_TITLE);


window.show(canvas);

    }
    catch(const std::exception& e)
    {
        std::cout
            << "Render error:\n"
            << e.what()
            << std::endl;
    }



    std::cout
        << "\n==============================\n"
        << " TEST FINISHED\n"
        << "==============================\n";


    return 0;
}