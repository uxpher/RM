#include "Matrix.h"

std::vector<double> translation(std::vector<double> input, std::vector<double> trans) {
	if (trans.size() != input.size()) {
		throw std::invalid_argument("ԭ������ƽ���������Ȳ�ƥ��");
	}
	std::vector<double> result(input.size());
	for (int i = 0; i < input.size(); i++) {
		result[i] = input[i] + trans[i];
	}
	return result;
}

std::vector<double> rotation(std::vector<double> input, Matrix& rot) {
	if (input.size() != 2) {
		throw std::invalid_argument("ԭ�����Ƕ�ά����");
	}
	else if (rot.get_cols() != rot.get_rows() || rot.get_cols() != 2 || !(rot * rot.transpose()).is_identity()) {
		throw std::invalid_argument("�����Ƕ�ά��ת����");
	}
	std::vector<double> result = rot * input;
	return result;
}

std::vector<double> rot_and_trans(std::vector<double> input, Matrix& rot, std::vector<double> trans) {
	if (input.size() != 2) {
		throw std::invalid_argument("ԭ�����Ƕ�ά����");
	}
	else if (trans.size() != input.size()) {
		throw std::invalid_argument("ԭ������ƽ���������Ȳ�ƥ��");
	}
	else if (rot.get_cols() != rot.get_rows() || rot.get_cols() != 2 || !(rot * rot.transpose()).is_identity()) {
		throw std::invalid_argument("�����Ƕ�ά��ת����");
	}
	input.push_back(1);
	trans.push_back(1);
	Matrix together({
	{rot(0,0), rot(0,1), trans[0]},
	{rot(1,0), rot(1,1), trans[1]},
	{0, 0, 1}
		});
	std::vector<double> result = together * input;
	result.pop_back();
	return result;
}

int main() {
	Matrix rotate({ {0.5, -sqrt(3) / 2}, {sqrt(3)/2, 0.5} });
	std::vector<double> pos{ 5, 0 };
	std::vector<double> trans{ 0, 1 };
	std::vector<double> res(2);
	std::cout << "������ɣ�" << std::endl;
	std::cout << "��һ����" << std::endl;
	res = rotation(pos, rotate);
	std::cout << res[0] <<"," << res[1] << std::endl;
	std::cout << "�ڶ�����" << std::endl;
	res = translation(res, trans);
	std::cout << res[0] << "," << res[1] << std::endl;
	std::cout << "һ����ɣ�" << std::endl;
	res = rot_and_trans(pos, rotate, trans);
	std::cout << res[0] << "," << res[1] << std::endl;
	return 0;
}