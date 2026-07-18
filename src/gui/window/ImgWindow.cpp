#include "ImgWindow.hpp"
#include <opencv2/opencv.hpp>

ImgWindow::ImgWindow(const std::string& name) : windowName(name) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

void ImgWindow::display(const Img& image) {
    if(image.empty()) {
        return;
    }
    cv::imshow(windowName, image.get_mat());
}

void ImgWindow::processEvents(int delayMs) {
    cv::waitKey(delayMs);
}

bool ImgWindow::isOpen() const {
    return cv::getWindowProperty(windowName, cv::WND_PROP_VISIBLE) >= 1;
}

void ImgWindow::close() {
    cv::destroyWindow(windowName);
}