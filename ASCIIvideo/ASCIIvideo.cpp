#include <iostream>
#include <opencv2/opencv.hpp>
#include <Windows.h> // for Windows API functions

const std::string asciiChars = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@"; // Define your ASCII characters

std::string intensityToAsciiChar(uchar intensity)
{
    // Map intensity to ASCII character
    //int numChars = sizeof(asciiChars) / sizeof(char);
    int numChars = asciiChars.size();
    int index = static_cast<int>(intensity) * numChars / 256;
    return asciiChars.substr(index,1);
}

int main()
{
    // Open camera
    cv::VideoCapture cap(0); // 0 represents the default camera index
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open camera" << std::endl;
        return 1;
    }
    
    cv::namedWindow("ASCII Art", cv::WINDOW_NORMAL);

    // Loop over camera frames
    cv::Mat frame;
    while (cap.read(frame))
    {
        //system("CLS");
        cv::flip(frame, frame, 1);
        // Convert frame to grayscale
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        // Resize frame to smaller resolution if needed
        cv::resize(frame, frame, cv::Size(160, 90));

        // Process frame to ASCII characters
        std::string asciiFrame;
        for (int i = 0; i < frame.rows; ++i) {
            for (int j = 0; j < frame.cols; ++j) {
                asciiFrame += intensityToAsciiChar(frame.at<uchar>(i, j));
            }
            asciiFrame += '\n';
        }
        
        // Display ASCII art string as image in window
        cv::Mat asciiImage = cv::Mat::zeros(frame.rows * 16, frame.cols * 9, CV_8UC3);
        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        double fontScale = 0.4;
        int thickness = 1;
        cv::Point textOrg(0, 16);
        int lineHeight = 16;
        int charWidth = 9;
        for (int i = 0; i < asciiFrame.length(); ++i)
        {
            char c = asciiFrame[i];
            if (c == '\n')
            {
                textOrg.y += lineHeight;
                textOrg.x = 0;
            }
            else
            {
                cv::putText(asciiImage, std::string(1, c), textOrg, fontFace, fontScale, cv::Scalar(255, 255, 255), thickness, cv::LINE_AA);
                textOrg.x += charWidth;
            }
        }
        cv::imshow("ASCII Art", asciiImage);

        // Wait for a key press
        if (cv::waitKey(1) == 27) // 27 is the ASCII code for ESC key
            break;
    }

    return 0;
}
