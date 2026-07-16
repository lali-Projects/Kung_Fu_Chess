#include "Window.hpp"

#include <opencv2/highgui.hpp>


Window::Window(
    const std::string& title)
:
title(title),
open(true)
{
}



void Window::show(
    const Img& image)
{

    cv::imshow(
        title,
        image.get_mat());


    if(mouseCallback)
    {
        cv::setMouseCallback(
            title,
            mouseHandler,
            this);
    }


    int key =
        cv::waitKey(1);



    if(key == 27)
    {
        open = false;
    }


    if(cv::getWindowProperty(
        title,
        cv::WND_PROP_VISIBLE) < 1)
    {
        open=false;
    }
}



bool Window::isOpen() const
{
    return open;
}



void Window::setMouseCallback(
    std::function<void(int,int)> callback)
{
    mouseCallback = callback;
}



void Window::mouseHandler(
    int event,
    int x,
    int y,
    int flags,
    void* userdata)
{

    Window* window =
        static_cast<Window*>(userdata);


    if(event == cv::EVENT_LBUTTONDOWN)
    {
        if(window->mouseCallback)
        {
            window->mouseCallback(
                x,
                y);
        }
    }
}