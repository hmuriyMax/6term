#include <iostream>
#include <omp.h>
#define THRS 3

using std::cin;
using std::cout;
using std::endl;

long long fast_sum_dumb(const int* n, int len) {
	long long sum = 0;

    #pragma omp parallel for num_threads(THRS) default(none) shared(sum, len, n)
    for (int i = 0; i < len; i++) {
        if (n[i]%3 == 1)
            sum += n[i];
    }
	return sum;
}


long long fast_sum(const int* n, int len) {
	long long sum = 0;
	#pragma omp parallel for num_threads(THRS) default(none) shared(sum, len, n)
    for (int i = 0; i < len; i++) {
        if (n[i]%3 == 1)
            #pragma omp critical
            sum += n[i];
    }
	return sum;
}


long long slow_sum(const int* n, int len) {
	long long sum = 0;
    #pragma omp critical
    {
        for (int i = 0; i < len; i++) {
            len = len;
            if (n[i]%3 == 1)
                sum += n[i];
        }
    };
	return sum;
}


int main(int argc, char *argv[])
{
	int n;
	long long result;
	double start;
	cout << "Input n: ";
	cin >> n;
	int* m = new int[n];
	for (int i = 0; i < n; i++) {
		m[i] = i;
	}

	cout << "Slow counter o(n): " << endl;;
	start = omp_get_wtime();
	result = slow_sum(m, n);
	cout << "   Result: " << result << endl;
	printf_s("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;

    omp_set_num_threads(THRS);
	cout << "Parallel counter with race condition: " << endl;;
	start = omp_get_wtime();
	result = fast_sum_dumb(m, n);
	cout << "   Result: " << result << endl;
	printf_s("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;


    omp_set_num_threads(THRS);
	cout << "Parallel counter without race condition: " << endl;;
	start = omp_get_wtime();
	result = fast_sum(m, n);
	cout << "   Result: " << result << endl;
	printf_s("   Took time: %.16g\n", omp_get_wtime() - start);
	cout << endl;
}