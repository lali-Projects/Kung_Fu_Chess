#include "MouseInput.hpp"



MouseInput::MouseInput(GameController& controller, Layout& layout)
:
controller(controller), layout(layout)
{
}



void MouseInput::click(int x, int y)
{
    Position position = layout.pixelToBoard(x, y);
    MoveResult result = controller.click(position);
    std::cout << "MouseInput::click: " << x << ", " << y << " -> Board Position: (" << position.getRow() << ", " << position.getCol() << ") Result: " << (result.success ? "Success" : "Failure") << ", Reason: " << result.reason << std::endl;    
}