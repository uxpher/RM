#include <opencv2/opencv.hpp>
#include "simulate.h"
using namespace cv;

KalmanFilter kf(4, 2, 0, CV_64F);
simulator sim(0.01, 0.5, 0.1);
// 状态转移矩阵 
int main() {
    kf.transitionMatrix = (Mat_<double>(4, 4) <<
        1, 0, 0.01, 0,
        0, 1, 0, 0.01,
        0, 0, 1, 0,
        0, 0, 0, 1);

    // 测量矩阵 
    kf.measurementMatrix = Mat::zeros(2, 4, CV_64F);
    kf.measurementMatrix.at<double>(0, 0) = 1.0;
    kf.measurementMatrix.at<double>(1, 1) = 1.0;

    // 协方差矩阵
    setIdentity(kf.processNoiseCov, Scalar::all(1e-2));
    setIdentity(kf.measurementNoiseCov, Scalar::all(0.25));
    setIdentity(kf.errorCovPost, Scalar::all(1));

    // 初始状态
    kf.statePost.at<double>(0) = 0;  // x
    kf.statePost.at<double>(1) = 0;  // y
    kf.statePost.at<double>(2) = 2;  // vx
    kf.statePost.at<double>(3) = 3;  // vy

    auto noisy_states = sim.both_noise_simulate(0.50);

    std::cout << std::fixed << std::setprecision(4);  

    std::cout << std::setw(6) << "t"
        << std::setw(12) << "Measured X" << std::setw(12) << "Measured Y"
        << std::setw(12) << "Pred X" << std::setw(12) << "Pred Y"
        << std::setw(12) << "Est X" << std::setw(12) << "Est Y"
        << std::endl;

    for (int i = 0; i < noisy_states.size(); i++) {
        Mat prediction = kf.predict();

        Mat measurement(2, 1, CV_64F);
        measurement.at<double>(0) = noisy_states[i].position.x;
        measurement.at<double>(1) = noisy_states[i].position.y;

        Mat estimated = kf.correct(measurement);

        std::cout << std::setw(6) << i * 0.01
            << std::setw(12) << measurement.at<double>(0)
            << std::setw(12) << measurement.at<double>(1)
            << std::setw(12) << prediction.at<double>(0)
            << std::setw(12) << prediction.at<double>(1)
            << std::setw(12) << estimated.at<double>(0)
            << std::setw(12) << estimated.at<double>(1)
            << std::endl;
    }

    return 0;
}