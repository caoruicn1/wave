
#ifndef __SEMAPHORE_HPP__
#define __SEMAPHORE_HPP__

#include <mutex>
#include <condition_variable>

namespace wave{
  class semaphore
  {
  private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.

  public:
    void notify() {
      std::unique_lock<decltype(mutex_)> lock(mutex_);
      ++count_;
      condition_.notify_one();
    }

    void wait() {
      std::unique_lock<decltype(mutex_)> lock(mutex_);
      while(!count_) // Handle spurious wake-ups.
        condition_.wait(lock);
      --count_;
    }

    bool try_wait() {
      std::unique_lock<decltype(mutex_)> lock(mutex_);
      if(count_) {
        --count_;
        return true;
      }
      return false;
    }
  };
}


#endif
