/**
 * @file guard.cpp
 * @author lior nagar and shaked levi
 * @brief 
 * @version 0.1
 * @date 2022-05-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "guard.hpp"
#include <iostream>

guard::guard(){
    std::unique_lock<std::mutex> lock(this->_lock);
}

guard::~guard(){
    std::unique_lock<std::mutex> unlock(this->_lock);
}


/*TESTING*/

int b = 15;

void *sum(void *a)
{
    int i = 0;
    
    guard *check = new guard();
    while(i < 20){
    std::cout << "B is now currently: " << b <<"\n";
    
    int data = *(int*)a; /* cast to int */
    if(data == 15){
        /*thread 1*/
        b +=data;
        std::cout<<"THREAD 1 added 15 to our data: " << b << "\n";
    }
    if(data == 5)
    {
        /*thread 2*/
        b-=data;
        std::cout<<"THREAD 2 removed 5 from data: " << b << "\n";
    }
    ++i;
    }
    return NULL;
}


int main(){
    
    pthread_t t1;
    pthread_t t2;
    int a1 = 15;
    int a2 = 5;

    pthread_create(&t1,NULL, &sum,&a1);
    pthread_join(t1,NULL);
    pthread_create(&t2,NULL,&sum,&a2);
    pthread_join(t2,NULL);

    std::cout<<"Guard test done." << std::endl;

    return 0;
}