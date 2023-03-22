#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H

#include <memory>

class FunctionWrapper
{
    struct ImplementBase
    {
        virtual void Call() = 0;
        virtual ~ImplementBase() {}
    };

    template<typename F>
    struct ImplementType : ImplementBase
    {
        F m_f;
        ImplementType(F&& t_f)
            :
        m_f(std::move(t_f))
        {}

        void Call()
        {
            m_f();
        }
    };
public:
    FunctionWrapper() = default;
    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;

    template<typename F>
    FunctionWrapper(F&& f)
        :
    m_implement(new ImplementType<F>(std::forward<F>(f)))
    {}

    FunctionWrapper(FunctionWrapper&& other)
        :
    m_implement(std::move(other.m_implement))
    {}

    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        m_implement = std::move(other.m_implement);
        return *this;
    }

    void operator()()
    {
        m_implement->Call();
    }
private:
    std::unique_ptr<ImplementBase> m_implement;
};

#endif // FUNCTION_WRAPPER_H