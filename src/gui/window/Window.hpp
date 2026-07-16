#pragma once

#include "Img.hpp"
#include <string>
#include <functional>


class Window
{

private:

    std::string title;

    bool open;


    std::function<void(int,int)> mouseCallback;


public:

    Window(
        const std::string& title);


    void show(
        const Img& image);


    bool isOpen() const;


    void setMouseCallback(
        std::function<void(int,int)> callback);


private:

    static void mouseHandler(
        int event,
        int x,
        int y,
        int flags,
        void* userdata);

};