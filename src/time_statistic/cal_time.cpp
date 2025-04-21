#include <iostream>
#include <chrono>
#include "cal_time.h"

TimeLabel::TimeLabel()
{
	start_time = std::chrono::high_resolution_clock::now();
}

void TimeLabel::reInit()
{
	start_time = std::chrono::high_resolution_clock::now();
}

void TimeLabel::compute()
{
	end_time = std::chrono::high_resolution_clock::now();
	elapsed_time = end_time - start_time;
	//return elapsed_time.count();
}

void TimeLabel::print_time(std::string print_str){
	std::cout << print_str << elapsed_time.count() <<" s"<< std::endl;
}
