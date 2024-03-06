#include "Bench.h"
#include "pch.h"
#include <io.h>
#include <iostream>
#include <iomanip>
#include <sstream>
void Bench::_Begin()
{
	Bench::begin = std::chrono::high_resolution_clock::now();
}
void Bench::_End()
{
	std::chrono::time_point<struct std::chrono::steady_clock,
		class std::chrono::duration<__int64, struct std::ratio<1, 1000000000>>> end =
			std::chrono::high_resolution_clock::now();
	if (Bench::begin.time_since_epoch().count() > 0)
	{
		Bench::elapse =
		{
			std::chrono::duration_cast<std::chrono::microseconds>	(end - Bench::begin).count(),
			std::chrono::duration_cast<std::chrono::milliseconds>	(end - Bench::begin).count(),
			std::chrono::duration_cast<std::chrono::seconds>			(end - Bench::begin).count()
		};
		Bench::time_logger.push_back(Bench::elapse);
		Bench::microd_logger.push_back(Bench::elapse.Microseconds);
		Bench::millid_logger.push_back(Bench::elapse.Milliseconds);
		Bench::second_logger.push_back(Bench::elapse.Seconds);
	}
}
void Bench::_PrintElapseMessage()
{
	std::cout << formatElapsedMessage() << std::flush;
}
std::string Bench::formatElapsedMessage()
{
	int64_t mcs = Bench::elapse.Microseconds;
	int64_t mls = Bench::elapse.Milliseconds;
	int64_t sec = Bench::elapse.Seconds;
	std::vector<int64_t> microd_logger_v = Bench::microd_logger;
	std::vector<int64_t> millid_logger_v = Bench::millid_logger;
	std::vector<int64_t> second_logger_v = Bench::second_logger;
	int64_t total_micro = [microd_logger_v]()
	{
		int64_t total = 0;
		for (int64_t value : microd_logger_v)
		{
			total += value;
		}
		return total;
	}();
	int64_t total_milli = [millid_logger_v]()
	{
		int64_t total = 0;
		for (int64_t value : millid_logger_v)
		{
			total += value;
		}
		return total;
	}();
	int64_t total_second = [second_logger_v]()
	{
		int64_t total = 0;
		for (int64_t value : second_logger_v)
		{
			total += value;
		}
		return total;
	}();
	std::string total_micro_str = std::to_string(total_micro);
	long double total_micro_average = static_cast<long double>(static_cast<long double>(total_micro) / static_cast<long double>(microd_logger_v.size()));
	std::string total_micro_average_str = std::to_string(total_micro_average);
	std::string total_milli_str = std::to_string(total_milli);
	long double total_milli_average = static_cast<long double>(static_cast<long double>(total_milli) / static_cast<long double>(microd_logger_v.size()));
	std::string total_milli_average_str = std::to_string(total_milli_average);
	std::string total_secon_str = std::to_string(total_second);
	long double total_secon_average = static_cast<long double>(static_cast<long double>(total_second) / static_cast<long double>(microd_logger_v.size()));
	std::string total_secon_average_str = std::to_string(total_secon_average);
	std::stringstream ss;
	ss << std::setw(20) << std::setfill(' ') << mcs;
	std::string mc = ss.str();
	ss.str(std::string());
	ss << std::setw(20) << std::setfill(' ') << mls;
	std::string ml = ss.str();
	ss.str(std::string());
	ss << std::setw(20) << std::setfill(' ') << sec;
	std::string se = ss.str();
	ss.str(std::string());
	ss << std::setw(20) << std::setfill(' ') << std::setprecision(4) << total_micro_average;
	total_micro_average_str = ss.str();
	ss.str(std::string());
	ss << std::setw(20) << std::setfill(' ') << std::setprecision(4) << total_milli_average;
	total_milli_average_str = ss.str();
	ss.str(std::string());
	ss << std::setw(20) << std::setfill(' ') << std::setprecision(4) << total_secon_average;
	total_secon_average_str = ss.str();
	ss.str(std::string());
	ss << Bench::microd_logger.size();
	std::string microd_size = ss.str();
	std::string message = "Bench duration: ";
	message.append(microd_size);
	message.append(" iterations\nMicroseconds:\t[");
	message.append(mc);
	message.append("µs]/[");
	message.append(total_micro_str);
	message.append("]\nAverage:\t[");
	message.append(total_micro_average_str);
	message.append("µs]\nMilliseconds:\t[");
	message.append(ml);
	message.append("ms]/[");
	message.append(total_milli_str);
	message.append("]\nAverage:\t[");
	message.append(total_milli_average_str);
	message.append("ms]\nSeconds:\t[");
	message.append(se);
	message.append("s ]/[");
	message.append(total_secon_str);
	message.append("]\nAverage:\t[");
	message.append(total_secon_average_str);
	message.append("s ]\n");
	return message;
}