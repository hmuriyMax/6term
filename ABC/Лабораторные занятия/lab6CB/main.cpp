#include <iostream>
#include <stdio.h>
#include <omp.h>

using std::cin;
using std::cout;
using std::endl;

long long fast_sum_dumb(int n) {
	long long sum = 0;
	#pragma omp parallel shared(sum)
	{
		#pragma omp for
		for (int i = 0; i < n; i++) {
			sum += i;
		}
	}
	return sum;
}


long long fast_sum(int n) {
	long long sum = 0;
	#pragma omp parallel shared(sum)
	{
	#pragma omp for
		for (int i = 0; i < n; i++) {
			#pragma omp atomic
			sum += i;
		}
	}
	return sum;
}


long long slow_sum(int n) {
	long long sum = 0;
	for (int i = 0; i < n; i++) {
		sum += i;
	}
	return sum;
}


int main(int argc, char *argv[])
{
	int n;
	long long result;
	double start;
	cout << "Input n: ";
	cin >> n;
	omp_set_num_threads(n);

	cout << "Slow counter o(n): " << endl;;
	start = omp_get_wtime();
	result = slow_sum(n);
	cout << "   Result: " << result << endl;
	printf("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;

	cout << "Parallel counter with race condition: " << endl;;
	start = omp_get_wtime();
	result = fast_sum_dumb(n);
	cout << "   Result: " << result << endl;
	printf("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;

	cout << "Parallel counter without race condition: " << endl;;
	start = omp_get_wtime();
	result = fast_sum(n);
	cout << "   Result: " << result << endl;
	printf("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;
}
