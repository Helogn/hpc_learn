#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
class TimeLabel
{

public:
	TimeLabel();
	void compute();
	void print_time(std::string print_str="print time is: ");
	void reInit();
	//std::time_t now_c = std::chrono::system_clock::to_time_t(now);

private:
	std::chrono::high_resolution_clock::time_point start_time;
	std::chrono::high_resolution_clock::time_point end_time;
	std::chrono::duration<double> elapsed_time;
};