#pragma once
namespace owo
{
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

    /**
     * @brief Contains a void callback function
     */
    class VoidCallbackContainer: public virtual CallbackContainer
    {
    private:
        void (*function)();
    public:
        VoidCallbackContainer(void (*callback)())
        {
            this->function = callback;
        }

        void func()
        {
            this->function();
        }
    };

    /**
     * @brief Parent class, contains the [func] callback template
     */
    class BoolCallbackContainer
    {
    public:
        virtual void func(bool state) = 0;
    };


    /**
     * @brief Contains the type class method and instance 
     * @tparam T Type of the stored class
     */
    template<class T>
    class TypedBoolCallbackContainer: public virtual BoolCallbackContainer
    {
    private:
        void (T::*callback)(bool);
        T* instance;
    public:
        TypedBoolCallbackContainer(void (T::*__callback)(bool), T* __instance)
        {
            this->callback = __callback;
            this->instance = __instance;
        }

        void func(bool state)
        {
            (this->instance->*this->callback)(state);
        }
    };

    /**
     * @brief Contains a void callback function
     */
    class VoidBoolCallbackContainer: public virtual BoolCallbackContainer
    {
    private:
        void (*function)(bool);
    public:
        VoidBoolCallbackContainer(void (*callback)(bool))
        {
            this->function = callback;
        }

        void func(bool state)
        {
            this->function(state);
        }
    };

    /**
     * @brief Parent class, contains the [func] callback template
     */
    class StringCallbackContainer
    {
    public:
        virtual void func(std::string val) = 0;
    };


    /**
     * @brief Contains the type class method and instance 
     * @tparam T Type of the stored class
     */
    template<class T>
    class TypedStringCallbackContainer: public virtual StringCallbackContainer
    {
    private:
        void (T::*callback)(std::string);
        T* instance;
    public:
        TypedStringCallbackContainer(void (T::*__callback)(std::string), T* __instance)
        {
            this->callback = __callback;
            this->instance = __instance;
        }

        void func(std::string val)
        {
            (this->instance->*this->callback)(val);
        }
    };

    /**
     * @brief Contains a void callback function
     */
    class VoidStringCallbackContainer: public virtual StringCallbackContainer
    {
    private:
        void (*function)(std::string);
    public:
        VoidStringCallbackContainer(void (*callback)(std::string))
        {
            this->function = callback;
        }

        void func(std::string val)
        {
            this->function(val);
        }
    };
}