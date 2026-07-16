#pragma once

#include "Img.hpp"
#include "Layout.hpp"
#include "GameSnapshot.hpp"

class SelectionRenderer
{
private:

    const Layout& layout;

public:

    SelectionRenderer(
        const Layout& layout);

    void draw(
        Img& canvas,
        const GameSnapshot& snapshot) const;
};