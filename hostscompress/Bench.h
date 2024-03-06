#ifndef BENCH_H
#define BENCH_H
#include <chrono>
#include <string>
#include <vector>

struct Time
{
	int64_t	Microseconds;
	int64_t	Milliseconds;
	int64_t	Seconds;
};
class Bench
{
private:
	std::chrono::time_point<struct std::chrono::steady_clock,
		class std::chrono::duration<__int64, struct std::ratio<1, 1000000000>>> begin;
	Time elapse = { 0 };
	std::vector<Time> time_logger;
	std::vector<int64_t> microd_logger;
	std::vector<int64_t> millid_logger;
	std::vector<int64_t> second_logger;
public:
	void _Begin();
	void _End();
	void _PrintElapseMessage();
	std::string formatElapsedMessage();
 };
#endif // !BENCH_H
