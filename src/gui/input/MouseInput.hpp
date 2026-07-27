#pragma once

#include "IBoardClickSink.hpp"
#include "Layout.hpp"


/** Maps window coordinates to board coordinates and forwards the click. */
class MouseInput
{
public:
    MouseInput(
        IBoardClickSink& clickSink,
        const Layout& layout);

    void click(int x, int y);

private:
    IBoardClickSink& m_clickSink;
    const Layout& m_layout;
};
