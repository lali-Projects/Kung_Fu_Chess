#include "SelectionRenderer.hpp"

SelectionRenderer::SelectionRenderer(
    const Layout& layout)
:
layout(layout)
{
}

void SelectionRenderer::draw( Img& canvas, const GameSnapshot& snapshot) const
{
    if(!snapshot.getSelectedPosition().has_value())
    {
        return;
    }

    cv::Rect rect =
        layout.getCellRect(snapshot.getSelectedPosition().value());

    canvas.draw_rectangle(
        rect.x,
        rect.y,
        rect.width,
        rect.height,
        cv::Scalar(0,255,0,255),
        4);
}