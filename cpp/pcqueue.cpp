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
  bool isWriting = false;
  bool isDraining = false;
  mutex mutexLock;
  condition_variable condition;
  deque<Data> buffer;

public:
  PCQueue();
  void write(Data data);
  deque<Data> drain();
  ~PCQueue();
};

template<typename Data> 
PCQueue<Data>::PCQueue() {}

template<typename Data>
void PCQueue<Data>::write(Data data) {
  unique_lock<mutex> locker(mutexLock);
  condition.wait(locker, [this](){return !isDraining;});
  isWriting = true;
  buffer.push_back(data);
  isWriting = false;
  locker.unlock();
  condition.notify_one();
  return;
}

template<typename Data>
deque<Data> PCQueue<Data>::drain() {
  unique_lock<mutex> locker(mutexLock);
  condition.wait(locker, [this](){return !isWriting;});
  isDraining = true;
  deque<Data> result;
  copy(buffer.begin(), buffer.end(), back_inserter(result));
  buffer.clear();
  isDraining = false;
  locker.unlock();
  condition.notify_one();
  return result;
}

template<typename Data> 
PCQueue<Data>::~PCQueue() {}
