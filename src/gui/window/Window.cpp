#include "Window.hpp"

#include <opencv2/highgui.hpp>


Window::Window(
    const std::string& title)
    :
    title(title)
{
}


void Window::show(
    const Img& image)
{
    cv::imshow(
        title,
        image.get_mat());

    cv::waitKey(0);
}


bool Window::isOpen() const
{
    return true;
}