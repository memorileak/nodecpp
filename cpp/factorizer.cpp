#include <thread>
#include <chrono>
#include <deque>
#include <node.h>
#include <nan.h>

#include "./pcqueue.cpp"

using namespace v8;
using namespace Nan;
using std::deque;

class Factorizer: public AsyncProgressWorker {
private:
  PCQueue<uint32_t> messageQueue;
  void drainMessageQueue();
  void writeToNode(const AsyncProgressWorker::ExecutionProgress& progress, uint32_t& factor);

protected:
  Callback* progressCallback;
  uint32_t n;

public: 
  Factorizer(Callback* progress, Callback* callback, uint32_t n);
  ~Factorizer();
  void Execute(const AsyncProgressWorker::ExecutionProgress& progress);
  void HandleProgressCallback(const char* data, size_t size);
  void HandleOKCallback();
};

Factorizer::Factorizer(Callback* progressCallback, Callback* completeCallback, uint32_t n) 
  : AsyncProgressWorker(completeCallback) 
{
  this->progressCallback = progressCallback;
  this->n = n;
}

// Run in Event loop
void Factorizer::drainMessageQueue() {
  Nan::HandleScope scope;
  for (uint32_t& item: messageQueue.drain()) {
    Local<Value> argv[] = {
      New<Integer>(*reinterpret_cast<int*>(&item))
    };
    progressCallback->Call(1, argv, async_resource);
  }
}

void Factorizer::writeToNode(
  const AsyncProgressWorker::ExecutionProgress& progress, 
  uint32_t& factor
) {
  messageQueue.write(factor);
  progress.Send(reinterpret_cast<const char*>(""), sizeof(""));
}

void Factorizer::Execute(const AsyncProgressWorker::ExecutionProgress& progress) {
  uint32_t factor = 2;
  while (n % 2 == 0) {
    writeToNode(progress, factor);
    n = n/2;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  for (uint32_t i = 3; i <= n; i = i+2) {
    while (n % i == 0) {
      writeToNode(progress, i);
      n = n/i;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
}

// Run in Event loop
void Factorizer::HandleProgressCallback(const char* data, size_t size) {
  drainMessageQueue();
}

// Run in Event loop
void Factorizer::HandleOKCallback() {
  drainMessageQueue();
  callback->Call(0, NULL, async_resource);
}

Factorizer::~Factorizer() {}

NAN_METHOD(Factor) {
  Callback* progressCallback = new Callback(info[1].As<v8::Function>());
  Callback* completeCallback = new Callback(info[2].As<v8::Function>());
  AsyncQueueWorker(
    new Factorizer(progressCallback, completeCallback, Nan::To<uint32_t>(info[0]).FromJust())
  );
}

NAN_MODULE_INIT(Init) {
  Nan::Set(
    target,
    New<v8::String>("factorize").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(Factor)).ToLocalChecked()
  );
}

NODE_MODULE(factorizer, Init);
