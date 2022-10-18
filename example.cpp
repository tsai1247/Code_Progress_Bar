#include "progress.h"
#include<thread>
#include<chrono>
#include<bits/stdc++.h>
#include<windows.h>

using namespace progress;
using namespace std;

int main()
{
    int size = 80;
    for(int i=0; i<size; i++)
    {
        record(i, size);
        for(int j=0; j<size; j++)
        {
            record(j, size);

            this_thread::sleep_for(chrono::microseconds(1000));
        }
        if(i % 2 == 0)
            record();

        if(i % 3 == 0)
            record();

        if(i % 5 == 0)
            record("here");
    }
}