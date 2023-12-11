#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace chrono;
using namespace this_thread;

int main(int argc, char const *argv[])
{
    if (argc < 2){
        cerr << "parm miss\n";
        return -1;
    }
    
    int total_of_produce{},total_of_customers{},number{};
    bool done{};

    vector<thread> v;
    mutex mux;

    for (int i {1}; i < argc ; i++){

        const auto t_val {stoi(argv[i])};

        total_of_produce += t_val;

        v.emplace_back([&](const int n){

            for(int i{};i < n;++i){
                unique_lock<mutex> lock(mux);
                cout << "thread id : " << get_id() << " , Production quantity = " << ++number << "\n" << flush;
            }

        },t_val);
    }
    
    while(!done){

        unique_lock<mutex> lock(mux);
        
        while (number > 0){
            ++total_of_customers;
            cout << "consume a product , Quantity : " << --number << "\n" << flush;
            done = total_of_customers >= total_of_produce;
        }
    }
    
    for(auto& i:v){
        i.join();
    }

    return 0;
}




