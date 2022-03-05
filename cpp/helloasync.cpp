#include <thread>
#include <chrono>

#include <node.h>
#include <uv.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Persistent;
using v8::Object;
using v8::Value;
using v8::String;
using v8::Function;
using v8::Undefined;
using v8::Null;

struct Work {
    uv_work_t request;
    Persistent<Function> callback;
    std::string name;
    std::string greeting;
};

void RunHelloInWorkerThread(uv_work_t* req) {
    Work* work = static_cast<Work*>(req->data);
    work->greeting = std::string("Hello, ").append(work->name);

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void HandleHelloInWorkerThreadComplete(uv_work_t* req, int status) {
    Isolate* isolate = Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    Work* work = static_cast<Work*>(req->data);
    Local<String> greeting = String::NewFromUtf8(isolate, work->greeting.c_str())
        .ToLocalChecked();
    Local<Value> argv[] = {greeting};
    Local<Function>::New(isolate, work->callback)
        ->Call(isolate->GetCurrentContext(), Null(isolate), 1, argv);
}

void HelloAsync(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Work* work = new Work();
    work->request.data = work;
    work->name = std::string(*(String::Utf8Value(isolate, args[0])));
    Local<Function> callback = Local<Function>::Cast(args[1]);
    work->callback.Reset(isolate, callback);

    uv_queue_work(
        uv_default_loop(), 
        &(work->request), 
        RunHelloInWorkerThread, 
        HandleHelloInWorkerThreadComplete
    );

    args.GetReturnValue().Set(Undefined(isolate));
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", HelloAsync);
}

NODE_MODULE(helloasync, init);
