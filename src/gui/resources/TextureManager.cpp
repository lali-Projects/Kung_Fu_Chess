#include "TextureManager.hpp"

#include <filesystem>
#include <vector>
#include <algorithm>
#include <stdexcept>


namespace fs = std::filesystem;



TextureManager::TextureManager(
    const std::string& path
)
    :
    basePath(path)
{
}




Sprite TextureManager::loadSpriteFromFolder(
    const std::string& folderPath
)
{
    Sprite sprite;


    if(!fs::exists(folderPath))
    {
        throw std::runtime_error(
            "Sprite folder does not exist: "
            + folderPath
        );
    }



    std::vector<fs::path> images;



    for(const auto& entry :
        fs::directory_iterator(folderPath))
    {
        if(entry.is_regular_file())
        {
            if(entry.path().extension() == ".png")
            {
                images.push_back(
                    entry.path()
                );
            }
        }
    }



    /*
       התמונות נקראות:
       
       1.png
       2.png
       3.png

       ולכן נמיין לפי שם.
    */
    std::sort(
        images.begin(),
        images.end(),
        [](const fs::path& a,
           const fs::path& b)
        {
            return
                std::stoi(
                    a.stem().string()
                )
                <
                std::stoi(
                    b.stem().string()
                );
        }
    );



    for(const auto& imagePath :
        images)
    {
        Img img;

        img.read(
            imagePath.string()
        );


        sprite.addFrame(
            img
        );
    }


    return sprite;
}





std::string TextureManager::buildPieceFolder(
    Side side,
    PieceType type
) const
{
    std::string folder;


    /*
        לדוגמה:

        WHITE PAWN
        -> PW

        BLACK KING
        -> KB
    */


    switch(type)
    {
        case PieceType::KING:
            folder += "K";
            break;

        case PieceType::QUEEN:
            folder += "Q";
            break;

        case PieceType::ROOK:
            folder += "R";
            break;

        case PieceType::BISHOP:
            folder += "B";
            break;

        case PieceType::KNIGHT:
            folder += "N";
            break;

        case PieceType::PAWN:
            folder += "P";
            break;
    }



    if(side == Side::WHITE)
        folder += "W";
    else
        folder += "B";



    return folder;
}





std::string TextureManager::buildStateFolder(
    PieceState state
) const
{
    switch(state)
    {
        case PieceState::IDLE:
            return "idle";


        case PieceState::MOVING:
            return "moving";


        case PieceState::CAPTURED:
            return "captured";


        case PieceState::AIRBORNE:
            return "jump";
    }


    return "";
}





void TextureManager::loadAll()
{

    std::vector<Side> sides =
    {
        Side::WHITE,
        Side::BLACK
    };


    std::vector<PieceType> types =
    {
        PieceType::KING,
        PieceType::QUEEN,
        PieceType::ROOK,
        PieceType::BISHOP,
        PieceType::KNIGHT,
        PieceType::PAWN
    };


    std::vector<PieceState> states =
    {
        PieceState::IDLE,
        PieceState::MOVING,
        PieceState::CAPTURED,
        PieceState::AIRBORNE
    };



    for(auto side : sides)
    {
        for(auto type : types)
        {
            for(auto state : states)
            {

                SpriteKey key(
                    side,
                    type,
                    state
                );


                std::string folder =
                    basePath
                    + "/"
                    + buildPieceFolder(
                        side,
                        type
                    )
                    + "/states/"
                    + buildStateFolder(
                        state
                    )
                    + "/sprites";



                if(fs::exists(folder))
                {
                    textures[key] =
                        loadSpriteFromFolder(
                            folder
                        );
                }
            }
        }
    }
}






const Sprite& TextureManager::getSprite(
    const SpriteKey& key
) const
{
    auto it =
        textures.find(key);



    if(it == textures.end())
    {
        throw std::runtime_error(
            "Sprite not found"
        );
    }


    return it->second;
}






bool TextureManager::contains(
    const SpriteKey& key
) const
{
    return
        textures.find(key)
        !=
        textures.end();
}






void TextureManager::clear()
{
    textures.clear();
}