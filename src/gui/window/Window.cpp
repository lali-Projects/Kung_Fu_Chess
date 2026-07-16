#include "Window.hpp"

#include <opencv2/highgui.hpp>


Window::Window(
    const std::string& title)
    :
    title(title),open(true)
{
}


void Window::show(
    const Img& image)
{
    cv::imshow(
        title,
        image.get_mat());
int key = cv::waitKey(1);

if (key == 27)
{
    open = false;
}

double visible = cv::getWindowProperty(title,cv::WND_PROP_VISIBLE);

if (visible < 1)
{
    open = false;
}
}


bool Window::isOpen() const
{
    return open;
}