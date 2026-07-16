#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "TextureManager.hpp"
#include "GameSnapshot.hpp"



class PieceRenderer
{

private:

    const Layout& layout;

    TextureManager& textureManager;


public:


    PieceRenderer(
        const Layout& layout,
        TextureManager& textureManager);



    void draw(
        Img& canvas,
        const GameSnapshot& snapshot);
};