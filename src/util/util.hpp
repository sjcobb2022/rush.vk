#pragma once

namespace rush
{
    std::deque<std::function<void()>> deletionQueue;
    
    /**
     * @brief Add a function to the deletion queue
     *
     * @tparam T Return type of function
     * @tparam Args Type of arguments
     * @param func Function to be passed to the deletion queue
     * @param args Args for specified function
     */
    template <typename Func, typename... Args>
    inline void delq_push(Func func, Args ...args)
    {
        // push back a function called with the args
        deletionQueue.push_back(
            [&](Args... args)
            {
                func(std::forward<Args>(args)...);
            });
    };
    
    template <typename Func>
    inline void delq_push(Func func)
    {
        // push back a function called with the args
        deletionQueue.push_back(func);
    }

    /**
     * @brief Flushes the deletion queue
     * 
     */
    inline void delq_flush()
    {
        for (auto it = deletionQueue.rbegin(); it != deletionQueue.rend(); it++)
        {
            (*it)(); // call functors
        }
    }
}