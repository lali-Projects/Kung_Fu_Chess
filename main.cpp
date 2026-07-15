#include <iostream>
#include <filesystem>

#include "TextureManager.hpp"
#include "GuiConfig.hpp"


int main()
{
    std::cout
        << "==============================\n"
        << " TEXTURE PATH TEST\n"
        << "==============================\n\n";


    //----------------------------------
    // Current working directory
    //----------------------------------

    std::cout
        << "Current directory:\n"
        << std::filesystem::current_path()
        << "\n\n";



    //----------------------------------
    // Check configured path
    //----------------------------------

    std::cout
        << "Board texture path:\n"
        << GuiConfig::BOARD_TEXTURE_PATH
        << "\n\n";


    bool exists =
        std::filesystem::exists(
            GuiConfig::BOARD_TEXTURE_PATH);


    std::cout
        << "File exists: "
        << exists
        << "\n\n";



    if(!exists)
    {
        std::cout
            << "ERROR: Board image was not found.\n";

        return 1;
    }



    //----------------------------------
    // TextureManager test
    //----------------------------------

    TextureManager textureManager;


    try
    {
        textureManager.loadTexture(
            "board",
            GuiConfig::BOARD_TEXTURE_PATH);


        std::cout
            << "[OK] Board texture loaded\n";
    }
    catch(const std::exception& e)
    {
        std::cout
            << "[ERROR] Loading failed:\n"
            << e.what()
            << "\n";

        return 1;
    }



    //----------------------------------
    // Verify stored texture
    //----------------------------------

    std::cout
        << "\nVerification:\n";


    std::cout
        << "Contains board: "
        << textureManager.contains("board")
        << "\n";


    try
    {
        Img& board =
            textureManager.getTexture("board");


        std::cout
            << "Image empty: "
            << board.empty()
            << "\n";


        std::cout
            << "Image width: "
            << board.width()
            << "\n";


        std::cout
            << "Image height: "
            << board.height()
            << "\n";
    }
    catch(const std::exception& e)
    {
        std::cout
            << "Verification failed:\n"
            << e.what()
            << "\n";

        return 1;
    }



    std::cout
        << "\n==============================\n"
        << " TEST PASSED\n"
        << "==============================\n";


    return 0;
}