#include <iostream>
#include <cstdint>
#include <ctime>
#include <chrono>

using namespace std;

static __inline__ uint64_t rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (uint64_t)lo)|( ((uint64_t)hi)<<32 );
}

template<typename T>
static T add_numbers(const T *v, const int size)
{
    T sum = 0;
    for(int i = 0; i < size; i++)
    sum += v[i];
    return sum;
}


template<typename T, const int size>
static T add_matrix(const T v[size][size])
{
    T sum[size] = {};
    for(int i = 0; i < size; i++)
    {
    for(int j = 0; j < size; j++)
        sum[i] += v[i][j];
    }
    T tsum=0;
    for(int i = 0; i < size; i++)
    tsum += sum[i];
    return tsum;
}



template<typename T>
static T add_mul_numbers(const T *v, const T mul, const int size)
{
    T sum = 0;
    for(int i = 0; i < size; i++)
    sum += v[i] * mul;
    return sum;
}

template<typename T>
static T add_div_numbers(const T *v, const T mul, const int size)
{
    T sum = 0;
    for(int i = 0; i < size; i++)
    sum += v[i] / mul;
    return sum;
}


template<typename T> 
void fill_array(T *v, const int size)
{
    for(int i = 0; i < size; i++)
    v[i] = i;
}

template<typename T, const int size> 
void fill_array(T v[size][size])
{
    for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
        v[i][j] = i + size * j;
}




uint32_t bench_add_numbers(const uint32_t v[], const int size)
{
    uint32_t res = add_numbers(v, size);
    return res;
}

uint64_t bench_add_numbers(const uint64_t v[], const int size)
{
    uint64_t res = add_numbers(v, size);
    return res;
}

uint32_t bench_add_mul_numbers(const uint32_t v[], const int size)
{
    const uint32_t c = 7;
    uint32_t res = add_mul_numbers(v, c, size);
    return res;
}

uint64_t bench_add_mul_numbers(const uint64_t v[], const int size)
{
    const uint64_t c = 7;
    uint64_t res = add_mul_numbers(v, c, size);
    return res;
}

uint32_t bench_add_div_numbers(const uint32_t v[], const int size)
{
    const uint32_t c = 7;
    uint32_t res = add_div_numbers(v, c, size);
    return res;
}

uint64_t bench_add_div_numbers(const uint64_t v[], const int size)
{
    const uint64_t c = 7;
    uint64_t res = add_div_numbers(v, c, size);
    return res;
}


template<const int size>
uint32_t bench_matrix(const uint32_t v[size][size])
{
    uint32_t res = add_matrix(v);
    return res;
}
template<const int size>
uint64_t bench_matrix(const uint64_t v[size][size])
{
    uint64_t res = add_matrix(v);
    return res;
}


template<typename T>
void runbench(T (*func)(const T *v, const int size), const char *name, T *v, const int size)
{
    fill_array(v, size);

    uint64_t t = rdtsc();
	auto start = chrono::steady_clock::now();
    T res = func(v, size);
    t = rdtsc() - t;
	auto duration = chrono::steady_clock::now() - start; 
    cout << "result = " << res << endl;
    cout << name << " time in clocks " << dec << t  << endl;
	cout << name << " time in ms " << duration.count() << endl;
}

template<typename T, const int size>
void runbench2(T (*func)(const T v[size][size]), const char *name, T v[size][size])
{
    fill_array(v);

    uint64_t t = rdtsc();
	auto start = chrono::steady_clock::now();
    T res = func(v);
    t = rdtsc() - t;
	auto duration = chrono::steady_clock::now() - start;
    cout << "result = " << res << endl;
    cout << name << " time in clocks " << dec << t  << endl;
	cout << name << " time in ms " << duration.count() << endl;
}


int main()
{
    // spin up CPU to full speed...
    time_t t = time(NULL);
    while(t == time(NULL)) ;

    const int vsize=10000;

    uint32_t v32[vsize];
    uint64_t v64[vsize];

    uint32_t m32[100][100];
    uint64_t m64[100][100];


    runbench(bench_add_numbers, "Add 32", v32, vsize);
    runbench(bench_add_numbers, "Add 64", v64, vsize);

    runbench(bench_add_mul_numbers, "Add Mul 32", v32, vsize);
    runbench(bench_add_mul_numbers, "Add Mul 64", v64, vsize);

    runbench(bench_add_div_numbers, "Add Div 32", v32, vsize);
    runbench(bench_add_div_numbers, "Add Div 64", v64, vsize);

    runbench2(bench_matrix, "Matrix 32", m32);
    runbench2(bench_matrix, "Matrix 64", m64);
}
