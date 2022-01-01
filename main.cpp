#include <opencv2/highgui.hpp>
#include <iostream>

// Include all GLM core / GLSL features
#include <glm/glm.hpp> // vec2, vec3, mat4, radians

// Include all GLM extensions
#include <glm/ext.hpp> // perspective, translate, rotate

glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    return Proj * View * Model;
}

int main(int argc, char** argv){
    cv::Mat imageBackground;
    imageBackground = cv::imread("../gradient.jpeg", cv::IMREAD_UNCHANGED);

    if(!imageBackground.data){
        std::cout <<  "Could not open or find the background image" << std::endl ;
        return -1;
    }

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    int key;
    int i = 0;
    while (key != 27) {
        cv::imshow("Display window", imageBackground);

        key = cv::waitKey(100);
        std::cout << "Key pressed: " << key << std::endl;

        i++;
    }

    return 0;
}