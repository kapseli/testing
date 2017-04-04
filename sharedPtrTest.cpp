#include <memory>
#include <stdio.h>

class SharedPtrClass
    : public std::enable_shared_from_this<SharedPtrClass>
{
public:
    SharedPtrClass()
    {
        printf("ctor: SharedPtrClass::SharedPtrClass() \n"); 
    }

    // Copy ctor
    SharedPtrClass(const SharedPtrClass& other)
    {
        printf("copy ctor: SharedPtrClass::SharedPtrClass(const SharedPtrClass& other) \n"); 
    }

    // Move ctor
    SharedPtrClass(SharedPtrClass&& other)
    {
        printf("move ctor: SharedPtrClass::SharedPtrClass(SharedPtrClass&& other) \n");
    }

    SharedPtrClass& operator=(const SharedPtrClass& other)
    {
        printf("copy assignment of SharedPtrClass \n");
        return *this;
    }

    SharedPtrClass& operator=(SharedPtrClass&& other)
    {
        printf("move assignment of SharedPtrClass \n");
        return *this;
    }
    
    ~SharedPtrClass()
    { 
        printf("dtor: SharedPtrClass::~SharedPtrClass() \n");
    }

    void CreateSharedFromThisInsideMemberFunction()
    {
        
        auto spThis = shared_from_this();
        printf("SharedPtrClass::CreateSharedFromThisInsideMemberFunction, AFTER use_count: %d \n", spThis.use_count() );
    }

    std::shared_ptr<SharedPtrClass> CreateSharedFromThisInsideMemberFunction2()
    {
        
        auto spThis = shared_from_this();
        printf("SharedPtrClass::CreateSharedFromThisInsideMemberFunction2, AFTER use_count: %d \n", spThis.use_count() );
        return spThis;
    }
};

template <class Ctype>
class WeakPtrTestClass
{
public:
    WeakPtrTestClass(std::weak_ptr<Ctype> wp)
        : m_wp(wp)
    {
        printf("ctor: WeakPtrTestClass::WeakPtrTestClass() \n"); 
    }

    // Copy ctor
    WeakPtrTestClass(const WeakPtrTestClass& other)
    {
        printf("copy ctor: WeakPtrTestClass::WeakPtrTestClass(const WeakPtrTestClass& other) \n"); 
    }

    // Move ctor
    WeakPtrTestClass(WeakPtrTestClass&& other)
    {
        printf("move ctor: WeakPtrTestClass::WeakPtrTestClass(WeakPtrTestClass&& other) \n");
    }

    WeakPtrTestClass& operator=(const WeakPtrTestClass& other)
    {
        printf("copy assignment of WeakPtrTestClass \n");
        return *this;
    }

    WeakPtrTestClass& operator=(WeakPtrTestClass&& other)
    {
        printf("move assignment of WeakPtrTestClass \n");
        return *this;
    }
    
    ~WeakPtrTestClass()
    { 
        printf("dtor: WeakPtrTestClass::~WeakPtrTestClass() \n");
    }

    void TestWeakPtr()
    {
        printf("WeakPtrTestClass::TestWeakPtr() \n");
        
        if(auto sp = m_wp.lock())
        {
            printf("WeakPtrTestClass::TestWeakPtr(), use_count: %d \n", sp.use_count() );
            printf("WeakPtrTestClass::TestWeakPtr() \n");
            sp->CreateSharedFromThisInsideMemberFunction();
        }
    }

private:
    std::weak_ptr<Ctype> m_wp;
    // void CreateSharedFromThisInsideMemberFunction()
    // {
        
    //     auto spThis = shared_from_this();
    //     printf("WeakPtrTestClass::CreateSharedFromThisInsideMemberFunction, AFTER use_count: %d \n", spThis.use_count() );
    // }
};

int main()
{
    {
        auto spClass = std::make_shared<SharedPtrClass>();
        spClass->CreateSharedFromThisInsideMemberFunction();
    }
    
    {
        auto spClass = new SharedPtrClass();
        try
        {
            spClass->CreateSharedFromThisInsideMemberFunction();
        }
        catch(const std::bad_weak_ptr& e)
        {
            printf("Exception thrown, e.what: %s \n", e.what());
        }
        catch(const std::exception &e)
        {
            printf("Exception thrown, e.what: %s \n", e.what());
        }
        
    }

    {
        std::unique_ptr<WeakPtrTestClass<SharedPtrClass>> upWpClass;
        {
            auto spClass = std::make_shared<SharedPtrClass>();
            upWpClass = std::make_unique<WeakPtrTestClass<SharedPtrClass>>(spClass->CreateSharedFromThisInsideMemberFunction2());
            printf("main(), spClass.use_count: %d \n", spClass.use_count() );
        }
        
        upWpClass->TestWeakPtr();
        
    }
    printf("\n :::End of main::: \n ");

    return 0;
}