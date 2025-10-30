#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
//图像处理异常类
class ImageProcessorException : public std::runtime_error {
public:
	explicit ImageProcessorException(const std::string& message)
		: std::runtime_error(message) {}
};
//图像处理工具类
class ImageProcessor {
private:
	cv::Mat image;
	std::string image_path;
public:
	//构造函数：加载图像
	ImageProcessor(const std::string& path) : image_path(path) {
		image = cv::imread(image_path, cv::IMREAD_COLOR);
		if (image.empty()) {
			throw ImageProcessorException("无法加载图像: " + image_path +
				" - 文件不存在或格式不支持");
		}
	}
	//获取图像尺寸
	cv::Size GetSize() const {
		return cv::Size(image.cols, image.rows);
	}
	//获取通道数
	int GetChannels() const {
		return image.channels();
	}
	//获取像素数据
	cv::Vec3b GetPixels(int x, int y) const {
		if (x < 0 || x >= image.cols || y < 0 || y >= image.rows) {
			throw ImageProcessorException("像素坐标越界");
		}
		return image.at<cv::Vec3b>(y, x);
	}
	//保存图像：
	void SaveImage(const std::string& outpath, const cv::Mat& img) {
		if (!cv::imwrite(outpath, img)) {
			throw ImageProcessorException("图像保存失败：" + outpath);
		}
	}
	//显示图像：
	void ShowImage(const std::string& WinName, const cv::Mat& img) {
		cv::imshow(WinName, img);
	}
	// 灯条阈值分割 - 基于亮度（灰度阈值）
	cv::Mat LightBarSegmentation(int threshold_value = 200) const {
		cv::Mat gray, binary;
		// 转换为灰度图
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		// 二值化处理 - 灯条区域为前景（白色），其余为背景（黑色）
		cv::threshold(gray, binary, threshold_value, 255, cv::THRESH_BINARY);
		// 形态学操作：去除噪声和连接断点
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel);   // 开运算去噪
		cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);  // 闭运算填充
		return binary;
	}
	// 灯条阈值分割 - 基于颜色（HSV 空间）
	cv::Mat ColorSegmentation_HSV(int h_low = 0, int h_high = 20, int s_low = 100, int s_high = 255, int v_low = 100, int v_high = 255) const {
		cv::Mat hsv, mask, binary;
		// 1. 转换到 HSV 颜色空间
		cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
		//定义灯条颜色的 HSV 范围
		cv::Scalar lower_bound(h_low, s_low, v_low);
		cv::Scalar upper_bound(h_high, s_high, v_high);
		//应用阈值分割，生成mask
		cv::inRange(hsv, lower_bound, upper_bound, mask);
		//形态学操作：去除噪声和连接断点
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::morphologyEx(mask, binary, cv::MORPH_OPEN, kernel);   // 开运算去噪
		cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);  // 闭运算填充
		return binary;
	}
	//预处理:
	//RGB转灰度图
	cv::Mat ToGray() const {
		cv::Mat gray;
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
		return gray;
	}
	//均值模糊去噪
	cv::Mat MeanBlur(int ksize = 5) const {
		cv::Mat blurred;
		cv::blur(image, blurred, cv::Size(ksize, ksize));
		return blurred;
	}
}; 
