#include <node.h> 
#include <nan.h>

using v8::Local;
using v8::String;
using v8::FunctionTemplate;

using Nan::Set;
using Nan::New;
using Nan::GetFunction;

NAN_METHOD(Hello) {
  Local<String> value = New<String>("Hello NAN!").ToLocalChecked();
  info.GetReturnValue().Set(value);
}

NAN_MODULE_INIT(Init) {
  Set(
    target,
    New<String>("hello").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(Hello)).ToLocalChecked()
  );
}

NODE_MODULE(hellonan, Init);