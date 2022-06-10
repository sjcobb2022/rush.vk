#pragma once

#include <deque>
#include <functional>

/**
 * @brief Function queue struct used for deletion queues and other function
 *
 */
struct f_Queue
{

    inline std::reverse_iterator<std::deque<std::function<void()>>::iterator> rbegin()
    {
        return m_Queue.rbegin();
    }

    inline std::reverse_iterator<std::deque<std::function<void()>>::iterator> rend()
    {
        return m_Queue.rend();
    }

    template <typename F>
    void push_function(F &&function)
    {
        m_Queue.push_back(function);
    }

    template <typename F, typename... Args>
    void push_function(F &&function, Args... args)
    {
        m_Queue.push_back([&](){
            function(std::forward<Args>(args)...); //forward the args to the function that are captured by reference });
    }); 
    }

    void flush()
    {
        // reverse iterate the deletion queue to execute all the functions
        for (auto it = m_Queue.rbegin(); it != m_Queue.rend(); it++)
        {
            (*it)(); // call functors
        }

        m_Queue.clear();
    }

private:
    std::deque<std::function<void()>> m_Queue;
};