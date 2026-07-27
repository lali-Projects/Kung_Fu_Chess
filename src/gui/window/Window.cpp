#include "Window.hpp"
#include <opencv2/highgui.hpp>
#include <utility>
#include "GuiConfig.hpp"

Window::Window(const std::string& title)
: title(title), open(true) {
}

Window::~Window() {
    unregisterMouseCallback();
}

void Window::show(const Img& image) {
    cv::imshow(title, image.get_mat());

    if(mouseCallback && !nativeMouseCallbackRegistered) {
        cv::setMouseCallback(title, mouseHandler, this);
        nativeMouseCallbackRegistered = true;
    }

    int key = cv::waitKey(1);

    if(key ==  GuiConfig::ESCAPE_KEY) {
        open = false;
    }

    if(cv::getWindowProperty(title, cv::WND_PROP_VISIBLE) < 1) {
        open = false;
    }
}

bool Window::isOpen() const {
    return open;
}

void Window::setMouseCallback(std::function<void(int,int)> callback) {
    mouseCallback = std::move(callback);

    if(!mouseCallback) {
        unregisterMouseCallback();
    }
}

void Window::unregisterMouseCallback() noexcept {
    if(!nativeMouseCallbackRegistered) {
        return;
    }

    try {
        cv::setMouseCallback(title, nullptr, nullptr);
    }
    catch(const cv::Exception&) {
        // A window already destroyed by the native UI has no live callback.
    }

    nativeMouseCallbackRegistered = false;
}

void Window::mouseHandler(int event, int x, int y, int flags, void* userdata) {
    Window* window = static_cast<Window*>(userdata);

    if(event == cv::EVENT_LBUTTONDOWN) {
        if(window->mouseCallback) {
            window->mouseCallback(x, y);
        }
    }
}
