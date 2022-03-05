#include <node.h>

using v8::Local;
using v8::Object;
using v8::FunctionCallbackInfo;
using v8::Value;
using v8::Isolate;
using v8::String;

void Hello(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<String> returnValue = String::NewFromUtf8Literal(isolate, "Hello World!");
    args.GetReturnValue().Set(returnValue);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "hello", Hello);
}

NODE_MODULE(helloworld, init);
