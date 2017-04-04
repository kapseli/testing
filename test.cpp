#include <stdio.h>
#include <iostream>

#include <functional>
#include <memory>


template <typename Out, typename In>
using CallbackFunction = std::function<Out(In)>;


template <class C>
class Caller
{
public:
    void ConnectCallback(CallbackFunction<C,C> callback)
    {
        m_callback = callback;
    }


    void InitiateCallback()
    {
        printf("Caller::InitiateCallback() \n");
        C i = m_callback(10);

        std::cout << "Result:" << i << std::endl;
    }

private:
    CallbackFunction<C,C> m_callback;

};


template <class C>
class Callee
{
public:
    Callee(C i)
        : m_i(i) 
    {
        printf("ctor: Callee::Callee() \n"); 
    }

    // Copy ctor
    Callee(const Callee& other)
        : m_i(other.m_i)
    {
        printf("copy ctor: Callee::Callee(const Callee& other) \n"); 
    }

    // Move ctor
    Callee(Callee&& other)
        : m_i(std::move(other.m_i))
    {
        printf("move ctor: Callee::Callee(Callee&& other) \n");
    }

    Callee& operator=(const Callee& other)
    {
        printf("copy assignment of Callee \n");
        //std::swap(m_i, other.m_i);
        m_i = other.m_i;
        return *this;
    }

    Callee& operator=(Callee&& other)
    {
        printf("move assignment of Callee \n");
        m_i = std::move(other.m_i);
        return *this;
    }
    
    ~Callee()
    { 
        printf("dtor: Callee::~Callee() \n");
    }


    C callbackFunction(C i)
    {
        printf("  Callee::callbackFunction() \n");
        return m_i * i; 
    }

private:
    C m_i;
};

int main()
{
    {
        using MyType = int;
        std::unique_ptr<Caller<MyType>> caller(new Caller<MyType>());
        {
            std::shared_ptr<Callee<MyType>> callee(new Callee<MyType>(5));
            printf("callee.use_count(): %li \n ", callee.use_count());

            caller->ConnectCallback(std::bind(&Callee<MyType>::callbackFunction, callee, std::placeholders::_1));

            //caller->connectCallback([callee](int i) { return callee->callbackFunction(i); });
            //delete callee;
            //callee = nullptr;
            
            //Callee<MyType>* callee2(new Callee<MyType>(10));
            printf("callee.use_count(): %li \n ", callee.use_count());
            // [&callee](int i) { return callee.callbackFunction(i); });
        }
        //printf("callee.use_count(): %li \n ", callee.use_count());
        // Note how this is getting closer to the ultimate:
        //     caller.connectCallback(callee.callbackFunction);

        // Test the callback
        caller->InitiateCallback();
    }
    printf("End of main \n ");

    return 0;
}

