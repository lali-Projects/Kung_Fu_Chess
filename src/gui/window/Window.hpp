#pragma once

#include "Img.hpp"
#include <string>


class Window
{

private:

    std::string title;
    bool open;

public:

    Window(
        const std::string& title);


    void show(
        const Img& image);


    bool isOpen() const;

};