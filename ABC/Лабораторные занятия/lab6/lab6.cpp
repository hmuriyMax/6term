#define MAIN
#ifdef MAIN
#include <iostream>
#include <omp.h>
#define THRS 8

using namespace std;

long long FastSumWrong(int n) {
	unsigned long long sum = 0;
#pragma omp parallel for num_threads(THRS) default(none) shared(sum, n)
	for (int i = 1; i <= n; i++) {
		sum += i;
	}
	return sum;
}


long long FastSum(int n) {
	unsigned long long sum = 0;
#pragma omp parallel for num_threads(THRS) default(none) shared(n) reduction (+: sum)	
	for (int i = 1; i <= n; i++) {
		sum += i;
	}
	return sum;
}


long long SlowSum(int n) {
	unsigned long long sum = 0;
	{
		for (int i = 1; i <= n; i++) {
			sum += i;
		}
	}
	return sum;
}


int main(int argc, char* argv[])
{
	const bool autoTest = true;
	const int n_start = 10, multiplier = 10;
	bool stop = false;
	double timeLimit = 0.1;
	uint32_t n;
	if (!autoTest)
	{
		cout << "Input N: ";
		cin >> n;
	}
	else
		n = n_start;

	unsigned long long sum, correct_sum;
	double start, delta1, delta2, delta3, speedup;
	int count = 0;
	do
	{
		if (autoTest)
		{
			count++;
			cout.setf(std::ios::scientific);
			cout << "N = " << n << " (" << (double)n << ")" << endl;
			cout.unsetf(std::ios::scientific);
		}
		cout << "1 thread (non-parallel): " << endl;
		start = omp_get_wtime();
		correct_sum = sum = SlowSum(n);
		delta1 = omp_get_wtime() - start;
		if (delta1 > timeLimit)
			stop = true;
		cout << "Result: " << sum << endl;
		cout.setf(std::ios::scientific);
		cout << "Time spent: " << delta1 << " seconds" << endl;
		cout.unsetf(std::ios::scientific);

		omp_set_num_threads(THRS);

		cout << endl << THRS << " threads (parallel, race condition):" << endl;
		start = omp_get_wtime();
		sum = FastSumWrong(n);
		delta2 = omp_get_wtime() - start;
		if (delta2 > timeLimit)
			stop = true;
		cout << "Result: " << sum;
		if (sum == correct_sum)
			cout << "\t(sum is correct)" << endl;
		else
			cout << "\t(sum is incorrect!)" << endl;
		cout.setf(std::ios::scientific);
		cout << "Time spent: " << delta2 << " seconds";
		speedup = 1 - delta2 / delta1;
		cout.unsetf(std::ios::scientific);
		if (speedup >= 0)
			cout << " (boost is " << delta1 / delta2 << ", solution is " << speedup * 100 << " % faster than non - parallel)" << endl;
		else
			cout << " (boost is " << delta1 / delta2 << ", solution is " << -speedup * 100 << "% slower than non-parallel)" << endl;

		cout << endl << THRS << " threads (parallel, no race condition):" << endl;
		start = omp_get_wtime();
		sum = FastSum(n);
		delta3 = omp_get_wtime() - start;
		if (delta3 > timeLimit)
			stop = true;
		cout << "Result: " << sum;
		if (sum == correct_sum)
			cout << "\t(sum is correct)" << endl;
		else
			cout << "\t(sum is incorrect!)" << endl;
		cout.setf(std::ios::scientific);
		cout << "Time spent: " << delta3 << " seconds";
		speedup = 1 - delta3 / delta1;
		cout.unsetf(std::ios::scientific);
		if (speedup >= 0)
			cout << " (boost is " << delta1 / delta3 << ", solution is " << speedup * 100 << "% faster than non-parallel)" << endl;
		else
			cout << " (boost is " << delta1 / delta3 << ", solution is " << -speedup * 100 << "% slower than non-parallel)" << endl;
		cout << "\n==============================================================\n\n";

		n *= multiplier;
	} while (autoTest && !stop);
}
#endif