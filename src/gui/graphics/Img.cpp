#include "Img.hpp"
#include <iostream>
#include <stdexcept>

Img::Img() {
    // img initialized empty
}

Img::Img(int width, int height, const cv::Scalar& background) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Invalid image dimensions");
    }
    img = cv::Mat(height, width, CV_8UC4, background);
}

Img& Img::read(const std::string& path, const std::pair<int,int>& size, bool keep_aspect, int interpolation) {
    img = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (img.empty()) {
        throw std::runtime_error("Cannot load image: " + path);
    }

    if(size.first != 0 && size.second != 0) {
        int targetWidth = size.first;
        int targetHeight = size.second;

        if(keep_aspect) {
            int currentWidth = img.cols;
            int currentHeight = img.rows;

            double scale = std::min(
                static_cast<double>(targetWidth) / currentWidth,
                static_cast<double>(targetHeight) / currentHeight
            );

            int newWidth = static_cast<int>(currentWidth * scale);
            int newHeight = static_cast<int>(currentHeight * scale);

            cv::resize(img, img, cv::Size(newWidth, newHeight), 0, 0, interpolation);
        } else {
            cv::resize(img, img, cv::Size(targetWidth, targetHeight), 0, 0, interpolation);
        }
    }
    return *this;
}

void Img::draw_on(Img& other_img, int x, int y) {
    if(img.empty() || other_img.img.empty()) {
        throw std::runtime_error("Both images must contain data");
    }

    if(x < 0 || y < 0 || x + img.cols > other_img.img.cols || y + img.rows > other_img.img.rows) {
        throw std::runtime_error("Image does not fit destination");
    }

    cv::Mat source = img;

    if(source.channels() == 4 && other_img.img.channels() == 4) {
        cv::Mat roi = other_img.img(cv::Rect(x, y, source.cols, source.rows));
        std::vector<cv::Mat> channels;
        cv::split(source, channels);
        cv::Mat alpha = channels[3];

        for(int row = 0; row < source.rows; row++) {
            for(int col = 0; col < source.cols; col++) {
                uchar a = alpha.at<uchar>(row, col);
                if(a > 0) {
                    roi.at<cv::Vec4b>(row, col) = source.at<cv::Vec4b>(row, col);
                }
            }
        }
    } else {
        source.copyTo(other_img.img(cv::Rect(x, y, source.cols, source.rows)));
    }
}

void Img::put_text(const std::string& txt, int x, int y, double font_size, const cv::Scalar& color, int thickness) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::putText(img, txt, cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, font_size, color, thickness, cv::LINE_AA);
}

void Img::show() {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::imshow("Image", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void Img::clear(const cv::Scalar& color) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    img.setTo(color);
}

void Img::draw_rectangle(int x, int y, int width, int height, const cv::Scalar& color, int thickness) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::rectangle(img, cv::Rect(x, y, width, height), color, thickness);
}

void Img::fill_rectangle(int x, int y, int width, int height, const cv::Scalar& color) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::rectangle(img, cv::Rect(x, y, width, height), color, cv::FILLED);
}

void Img::draw_line(int x1, int y1, int x2, int y2, const cv::Scalar& color, int thickness) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::line(img, cv::Point(x1,y1), cv::Point(x2,y2), color, thickness);
}

int Img::width() const {
    return img.cols;
}

int Img::height() const {
    return img.rows;
}

bool Img::empty() const {
    return img.empty();
}

void Img::resize(int width, int height) {
    if(img.empty()) {
        throw std::runtime_error("Image is empty");
    }
    cv::resize(img, img, cv::Size(width, height), 0, 0, cv::INTER_AREA);
}