/**
 * @file guard.hpp
 * @author shaked levi & lior nagar
 * @brief 
 * @version 0.1
 * @date 2022-05-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <mutex>

class guard{

    private:
    mutable std::mutex _lock;

    public:

    guard();
    ~guard();

};