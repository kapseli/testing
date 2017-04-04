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
    void ConnectCallback(CallbackFunction<C, C> callback)
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
    CallbackFunction<C, C> m_callback;
};

template <class C>
class Callee
{
  public:
    Callee(C i)
        : m_value(i)
    {
        printf("ctor: Callee::Callee() \n");
    }

    // Copy ctor
    Callee(const Callee &other)
        : m_value(other.m_value)
    {
        printf("copy ctor: Callee::Callee(const Callee& other) \n");
    }

    // Move ctor
    Callee(Callee &&other)
        : m_value(std::move(other.m_value))
    {
        printf("move ctor: Callee::Callee(Callee&& other) \n");
    }

    Callee &operator=(const Callee &other)
    {
        printf("copy assignment of Callee \n");
        //std::swap(m_value, other.m_value);
        m_value = other.m_value;
        return *this;
    }

    Callee &operator=(Callee &&other)
    {
        printf("move assignment of Callee \n");
        m_value = std::move(other.m_value);
        return *this;
    }

    ~Callee()
    {
        printf("dtor: Callee::~Callee() \n");
    }

    C CallbackFunctionImp(C value)
    {
        printf("  Callee::CallbackFunctionImp() \n");
        return m_value * value;
    }

  private:
    C m_value;
};

int main()
{
    {
        using MyType = int;
        std::unique_ptr<Caller<MyType>> caller(new Caller<MyType>());
        {
            std::shared_ptr<Callee<MyType>> callee(new Callee<MyType>(10));
            printf("callee.use_count(): %li \n ", callee.use_count());

            caller->ConnectCallback(std::bind(&Callee<MyType>::CallbackFunctionImp, callee, std::placeholders::_1));

            // Important to note that you can achieve same results with Lambda expression but
            // if you capture callee by reference ref count of the shared_ptr is not increased.
            // Leading to situation where object is freed and callback called to invalid memory.
            // Where std::bind gives compile time error.
            //
            // caller->ConnectCallback([&callee](int i) { return callee->CallbackFunctionImp(i); });  // Ref count issue
            // caller->ConnectCallback([&callee](int i) { return callee->CallbackFunctionImp(i); });  // OK

            printf("callee.use_count(): %li \n ", callee.use_count());
        }

        // Test the callback
        caller->InitiateCallback();
    }
    
    // Error situation implementation with Lambda and capture by reference
    {
        using MyType = int;
        std::unique_ptr<Caller<MyType>> caller(new Caller<MyType>());
        {
            std::shared_ptr<Callee<MyType>> callee(new Callee<MyType>(5));
            printf("callee.use_count(): %li \n ", callee.use_count());

            caller->ConnectCallback([&callee](int i) { return callee->CallbackFunctionImp(i); });

            printf("callee.use_count(): %li \n ", callee.use_count());
        }
        // Test the callback
        caller->InitiateCallback();
    }
    printf("\n :::End of main::: \n ");

    return 0;
}
