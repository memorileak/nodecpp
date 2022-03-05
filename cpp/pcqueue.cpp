#include <mutex>
#include <deque>
#include <iterator>

using std::unique_lock;
using std::mutex;
using std::condition_variable;
using std::deque;
using std::copy;
using std::back_inserter;

template<typename Data> 
class PCQueue {
  private:
    mutex mutexLock;
    condition_variable condition;
    deque<Data> buffer;

  public:
    PCQueue();
    void write(Data data);
    Data read();
    deque<Data> readAll();
    ~PCQueue();
};

template<typename Data> 
PCQueue<Data>::PCQueue() {}

template<typename Data>
void PCQueue<Data>::write(Data data) {
  while (true) {
    unique_lock<mutex> locker(mutexLock);
    buffer.push_back(data);
    locker.unlock();
    condition.notify_all();
    return;
  }
}

template<typename Data>
Data PCQueue<Data>::read() {
  while (true) {
    unique_lock<mutex> locker(mutexLock);
    condition.wait(locker, [this](){
      return buffer.size() > 0;
    });
    Data data = buffer.front();
    buffer.pop_front();
    locker.unlock();
    condition.notify_all();
    return data;
  }
}

template<typename Data>
deque<Data> PCQueue<Data>::readAll() {
  unique_lock<mutex> locker(mutexLock);
  deque<Data> result;
  copy(buffer.begin(), buffer.end(), back_inserter(result));
  buffer.clear();
  locker.unlock();
  return result;
}

template<typename Data> 
PCQueue<Data>::~PCQueue() {}