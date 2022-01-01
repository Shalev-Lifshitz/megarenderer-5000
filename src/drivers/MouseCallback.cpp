#include <opencv2/opencv.hpp>

void MouseCallback(int event, int x, int y, int flags, void *userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    } else if (event == cv::EVENT_MBUTTONDOWN) {
        std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    } else if (event == cv::EVENT_MOUSEMOVE) {
        std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;
    }
}

// TODO: Convert this to .h? Its just a func.