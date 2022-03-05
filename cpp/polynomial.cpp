#include <node.h>
#include <node_object_wrap.h>

using node::ObjectWrap;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::PropertyCallbackInfo;
using v8::String;
using v8::Value;

class WrappedPoly : public ObjectWrap {
private:
  static Persistent<Function> constructor;
  static void New(const FunctionCallbackInfo<Value> &args);
  static void Roots(const FunctionCallbackInfo<Value> &args);
  static void At(const FunctionCallbackInfo<Value> &args);
  static void GetA(Local<String> property, const PropertyCallbackInfo<Value> &info);
  static void SetA(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info);

  double a;
  double b;
  double c;

  explicit WrappedPoly(double a = 0, double b = 0, double c = 0);
  ~WrappedPoly();

public:
  static void Init(Local<Object> exports);
};

Persistent<Function> WrappedPoly::constructor;

WrappedPoly::WrappedPoly(double a, double b, double c) {
  this->a = a;
  this->b = b;
  this->c = c;
}

WrappedPoly::~WrappedPoly() {}

void WrappedPoly::New(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  if (args.IsConstructCall()) {
    double a = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(context).ToChecked();
    double b = args[1]->IsUndefined() ? 0 : args[1]->NumberValue(context).ToChecked();
    double c = args[2]->IsUndefined() ? 0 : args[2]->NumberValue(context).ToChecked();
    WrappedPoly *obj = new WrappedPoly(a, b, c);
    obj->Wrap(args.This());
    // Return JavaScript's this
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function Polynomial(...),
    // turn into construct call.
    const int argc = 3;
    Local<Value> argv[argc] = {args[0], args[1], args[2]};
    // Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Function> cons = constructor.Get(isolate);
    args.GetReturnValue().Set(cons->NewInstance(context, argc, argv).ToLocalChecked());
  }
}

void WrappedPoly::Roots(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  args.GetReturnValue().Set(Number::New(isolate, -1));
}

void WrappedPoly::At(const FunctionCallbackInfo<Value> &args) {
  Isolate *isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  double x = args[0]->IsUndefined() ? 0 : args[0]->NumberValue(context).ToChecked();
  WrappedPoly *poly = ObjectWrap::Unwrap<WrappedPoly>(args.Holder());
  double result = poly->a * x * x + poly->b * x + poly->c;
  args.GetReturnValue().Set(Number::New(isolate, result));
}

void WrappedPoly::GetA(Local<String> property, const PropertyCallbackInfo<Value> &info) {
  Isolate *isolate = info.GetIsolate();
  WrappedPoly *poly = ObjectWrap::Unwrap<WrappedPoly>(info.This());
  info.GetReturnValue().Set(Number::New(isolate, poly->a));
}

void WrappedPoly::SetA(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info) {
  Isolate *isolate = info.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  WrappedPoly *poly = ObjectWrap::Unwrap<WrappedPoly>(info.This());
  poly->a = value->NumberValue(context).ToChecked();
}

void WrappedPoly::Init(Local<Object> exports) {
  Isolate *isolate = exports->GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8Literal(isolate, "Polynomial"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "roots", Roots);
  NODE_SET_PROTOTYPE_METHOD(tpl, "at", At);

  tpl->InstanceTemplate()->SetAccessor(String::NewFromUtf8Literal(isolate, "a"), GetA, SetA);

  // Store a reference to this constructor
  // so it can be called in New if New is called
  // without constructor syntax.
  constructor.Reset(isolate, tpl->GetFunction(context).ToLocalChecked());

  exports->Set(
      context,
      String::NewFromUtf8Literal(isolate, "Polynomial"),
      tpl->GetFunction(context).ToLocalChecked());
}

void InitPoly(Local<Object> exports) {
  WrappedPoly::Init(exports);
}

NODE_MODULE(polynomial, InitPoly);
