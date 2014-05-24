#include <string>
#include <iostream>
#include <thread>
#include <stdlib.h>
#include <time.h>

using namespace std;

void task1(string msg)
{
    cout << "task1 says: " << msg << endl;
}

void randos(double* num, int iterations, string str)
{
    double sum = 0;
    srand(time(NULL));
    for (int i = 0; i < iterations; i++) {
        sum += (double)rand()/RAND_MAX;
    }
    double average = *num = (double)sum/(double)iterations;
    cout << str << " average: " << average << endl;
    //cout << "average: " << average << endl;
}

int main()
{
    //thread t1(task1, "sup world");
    /*
    double a = 0;
    double b = 0;
    double c = 0;
    double d = 0;
    */
    double a,b,c,d = 0;
    thread t1(randos,&a,10000000, "a");
    thread t2(randos,&b,10000000, "b");
    thread t3(randos,&c,10000000, "c");
    thread t4(randos,&d,10000000, "d");
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << "\ntotal average = " << (a+b+c+d)/4 << endl;
}
