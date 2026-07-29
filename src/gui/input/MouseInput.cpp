#include "MouseInput.hpp"


MouseInput::MouseInput(
    IBoardClickSink& clickSink,
    const Layout& layout)
    : m_clickSink(clickSink),
      m_layout(layout)
{
}


void MouseInput::click(int x, int y)
{
    const Position position = m_layout.pixelToBoard(x, y);
    m_clickSink.click(position);
}
