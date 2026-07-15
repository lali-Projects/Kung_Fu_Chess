#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "TextureManager.hpp"


class BoardRenderer
{
private:

    const Layout& layout;
    TextureManager& textureManager;


public:

    BoardRenderer(
        const Layout& layout,
        TextureManager& textureManager);


    void draw(Img& canvas);
};