/**
 * @brief Parent class, contains the [func] callback template
 */
class CallbackContainer
{
public:
    virtual void func() = 0;
};


/**
 * @brief Contains the type class method and instance 
 * @tparam T Type of the stored class
 */
template<class T>
class TypedCallbackContainer: public virtual CallbackContainer
{
private:
    void (T::*callback)();
    T* instance;
public:
    TypedCallbackContainer(void (T::*__callback)(), T* __instance)
    {
        this->callback = __callback;
        this->instance = __instance;
    }

    void func()
    {
        (this->instance->*this->callback)();
    }
};