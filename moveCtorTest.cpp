
#include <memory>
#include <stdio.h>
#include <vector>

#define DPRINT(fmt, ...) printf("[%s] \t\t" fmt "\n", __FUNCTION__, ##__VA_ARGS__)

class PtrHolder
{
public:
    size_t m_size;
    unsigned char* m_blob;

    PtrHolder(const size_t initSize)
        : m_size(initSize)
        , m_blob(nullptr)
    {
        m_blob = new unsigned char[initSize];
        // printf("PtrHolder ctor (0x%p) \n ", this);
        DPRINT("ctor (0x%p), created m_blob=0x%p", this, m_blob);
    }

    ~PtrHolder()
    {
        if(m_blob)
        {
            DPRINT("dtor (0x%p), deleting m_blob=0x%p", this, m_blob);
            delete[] m_blob;
            m_blob = nullptr;
            m_size = 0;
        }
        else
        {
            DPRINT("dtor (0x%p)", this);
        }
    }

    PtrHolder(PtrHolder&& obj) noexcept
        : m_size(obj.m_size)
        , m_blob(std::move(obj.m_blob))
    {
        DPRINT("move ctor (0x%p), moved m_blob=0x%p from 0x%p", this, m_blob, &obj);
        obj.m_blob = nullptr;
        obj.m_size = 0;
    }

    // deleting copy constructor
    PtrHolder(const PtrHolder&) = delete;


};


int main()
{
    printf("\n :::Move Ctor test::: \n ");
    auto holderVec = std::vector<PtrHolder>();

    DPRINT("holderVec capacity: %ld", holderVec.capacity());
    holderVec.reserve(3);
    DPRINT("holderVec capacity: %ld, after reserve", holderVec.capacity());
    // auto* pTest = new int(1);

    
    for(size_t i = 0; i < 3; i++)
    {
        PtrHolder holder(10);
        holderVec.push_back(std::move(holder));
    }
    


    printf("\n :::End of main::: \n\n ");

    return 0;
}