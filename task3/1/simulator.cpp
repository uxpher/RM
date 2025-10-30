#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

struct coord {
	double x;
	double y;
	coord(double x_input = 0.0, double y_input = 0.0) :x(x_input), y(y_input) {}
};

struct motion_state {
	coord position;
	coord velocity;
	motion_state(coord pos = coord(), coord vel = coord()) :position(pos), velocity(vel) {}
};

class simulator {
private:
	double delta_t;
	double measure_noise_std_d;
	double process_noise_std_d;
	std::default_random_engine generator;
	std::normal_distribution<double> measure_noise_dist;
	std::normal_distribution<double> process_noise_dist;
	motion_state initial_state;
public:
	simulator(double dt, double m_nsd, double p_nsd)
		:delta_t(dt), measure_noise_std_d(m_nsd), process_noise_std_d(p_nsd)
		, measure_noise_dist(0.0, m_nsd), process_noise_dist(0.0, p_nsd), initial_state(coord(0, 0), coord(2, 3)) {}
	//无噪模拟：
	std::vector<motion_state> no_noise_simulate(double time) {
		int steps = static_cast<int> (time / delta_t) + 1;
		std::vector<motion_state> states;
		states.reserve(steps);
		motion_state current = initial_state;
		for (int i = 0; i < steps; ++i) {
			states.push_back(current);
			current.position.x += current.velocity.x * delta_t;
			current.position.y += current.velocity.y * delta_t;
		}
		return states;
	}
	//观测噪声模拟：
	std::vector<motion_state> measure_noise_simulate(double time) {
		std::vector<motion_state> true_states = no_noise_simulate(time);
		std::vector<motion_state> states;
		states.reserve(true_states.size());
		motion_state current = initial_state;
		for (auto& true_state: true_states) {
			current.position.x = true_state.position.x + measure_noise_dist(generator);
			current.position.y = true_state.position.y + measure_noise_dist(generator);
			states.push_back(current);
		}
		return states;
	}
	//观测噪声+过程噪声模拟
	std::vector<motion_state> both_noise_simulate(double time) {
		int steps = static_cast<int> (time / delta_t) + 1;
		std::vector<motion_state> states;
		states.reserve(steps);
		motion_state current = initial_state;
		motion_state true_state = initial_state;
		for (int i = 0; i < steps; ++i) {
			current.position.x = true_state.position.x + measure_noise_dist(generator);
			current.position.y = true_state.position.y + measure_noise_dist(generator);
			current.velocity.x += process_noise_dist(generator);
			current.velocity.y += process_noise_dist(generator);
			states.push_back(current);
			true_state.position.x += current.velocity.x * delta_t;
			true_state.position.y += current.velocity.y * delta_t;
		}
		return states;
	}
	//打印位置-速度-时间
	void print_states(const std::vector<motion_state>& states, const std::string& title) {
		std::cout << "\n=== " << title << " ===\n";
		int steps = states.size();
		std::cout << std::setw(6) << "Time" << std::setw(10) << "Pos X"
			<< std::setw(10) << "Pos Y" << std::setw(10) << "Vel X"
			<< std::setw(10) << "Vel Y" << std::endl;
		for (int i = 0; i < steps; ++i) {
			std::cout << std::setw(6) << i * delta_t << std::setw(10) << states[i].position.x
				<< std::setw(10) << states[i].position.y << std::setw(10) << states[i].velocity.x
				<< std::setw(10) << states[i].velocity.y << std::endl;
		}
	}
};

int main() {
	simulator sim(0.01, 0.5, 0.1);
	double time = 0.50;
	std::cout << "装甲板运动模拟 (帧率: 100fps)" << std::endl;
	std::cout << "初始位置: (0, 0), 初始速度: (2, 3)" << std::endl;
	std::cout << "观测噪声标准差: 0.5, 过程噪声标准差: 0.1" << std::endl;
	//无噪声模拟：
	auto no_noise = sim.no_noise_simulate(time);
	sim.print_states(no_noise, "无噪声模拟");
	//观测噪声模拟：
	auto me_noise = sim.measure_noise_simulate(time);
	sim.print_states(me_noise, "观测噪声模拟");
	//观测噪声+过程噪声模拟：
	auto pr_noise = sim.both_noise_simulate(time);
	sim.print_states(pr_noise, "观测噪声+过程噪声模拟");
}
