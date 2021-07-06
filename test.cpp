#include <iostream>

class Container
{
public:
    virtual void func() = 0;
};

template<class T> class ContainerChild: public virtual Container
{
private:
    void (T::*callback)();
    T instance;
public:
    ContainerChild(void (T::*__callback)(), T __instance)
    {
        this->callback = __callback;
        this->instance = __instance;
    }

    void func()
    {
        (this->instance.*this->callback)();
    }
};

struct Button
{
    Container* cont;

    template<class G> void setCallback(void (G::*callback)(), G c)
    {
        this->cont = new ContainerChild<G>(callback, c);
    }

    void click()
    {
        this->cont->func();
    }
};

class UwU
{
private:
    int h = 621;
public:
    void owo()
    {
        std::cout << "OwO, what's this ?" << h << std::endl;
    }
};


template<class T> void executeCallback(void (T::*callback)(), T c)
{
    (c.*callback)();
}

int main(int argc, char const *argv[])
{
    UwU u;
    Button btn;
    btn.setCallback(&UwU::owo, u);
    btn.click();
    return 0;
}