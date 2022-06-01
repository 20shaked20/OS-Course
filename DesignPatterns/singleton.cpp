/**
 * @file singleton.hpp
 * @author lior nagar & shaked levi
 * @brief
 * @version 0.1
 * @date 2022-05-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>

template <typename T>
class singleton
{

private:
    static singleton *_sgt;
    T _type;
    /*is private because i want only once to apply it.*/
    singleton(T type)
    {
        this->_type = type;
    }

public:
    /**
     * @brief returns pointer to the singleton
     *
     * @return singleton*
     */
    static singleton *Instance(T type)
    {
        if (_sgt != nullptr)
        {
            return _sgt;
        }
        else
        {
            _sgt = new singleton(type);
            return _sgt;
        }
    }

    /**
     * @brief destroyes the singleton object.
     *
     */
    void Destroy()
    {
        _type = NULL;
    }

    /*make sure this wont override and create another isntace*/
    singleton(const singleton &) = delete;
    singleton &operator=(const singleton &) = delete;
    singleton(singleton &&) = delete;
    singleton &operator=(singleton &&) = delete;
};

template <typename T> singleton<T> *singleton<T>::_sgt = 0;

class Car
{

public:
    std::string _color;
    std::string _type;

    Car(std::string color, std::string type)
    {
        _color = color;
        _type = type;
    }

    Car()
    {
        _color = "black";
        _type = "mazda";
    }
    ~Car() {}

    friend std::ostream &operator<<(std::ostream &output, Car const &car)
    {
        output << "Color:" << car._color << "\n"
               << "Type:" << car._type << "\n";
        return output;
    }
};

int main()
{

    Car *carri;
    singleton<Car *> *sgt1 = singleton<Car *>::Instance(carri);
    singleton<Car *> *sgt2 = singleton<Car *>::Instance(carri);

    if (sgt1 != sgt2)
    {
        std::cout << "FAILED: objects are not equal!"
                  << "\n";
    }
    else
    {
        std::cout << "Success: Objects are the same!"
                  << "\n";
    }
}