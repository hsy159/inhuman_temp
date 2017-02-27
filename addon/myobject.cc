#include <node.h>
#include "myobject.h"

namespace demo
{

using v8::Context;
using v8::Exception; //
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope; //
using v8::Isolate;
using v8::JSON; //
using v8::Local;
using v8::Null; //
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> MyObject::constructor;
CNextBody MyObject::m_NB;

MyObject::MyObject(double value) : value_(value)
{
}

MyObject::~MyObject()
{
}

void MyObject::Init(Isolate *isolate)
{
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getLandmark", GetLandmark);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getBodySize", GetBodySize);
  NODE_SET_PROTOTYPE_METHOD(tpl, "calcVnSData", CalcVnSData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "calcLateralData", CalcLateralData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "calcHipKneeData", CalcHipKneeData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "calcSpineData", CalcSpineData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "calcShapeData", CalcShapeData);

  constructor.Reset(isolate, tpl->GetFunction());
}

void MyObject::New(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  if (args.IsConstructCall())
  {
    // Invoked as constructor: `new MyObject(...)`
    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    MyObject *obj = new MyObject(value);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  }
  else
  {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = {args[0]};
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> instance =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(instance);
  }
}

void MyObject::NewInstance(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  const unsigned argc = 1;
  Local<Value> argv[argc] = {args[0]};
  Local<Function> cons = Local<Function>::New(isolate, constructor);
  Local<Context> context = isolate->GetCurrentContext();
  Local<Object> instance =
      cons->NewInstance(context, argc, argv).ToLocalChecked();

  args.GetReturnValue().Set(instance);
}

void MyObject::PlusOne(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  MyObject *obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}

void MyObject::GetLandmark(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 2)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber() || !args[1]->IsString())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<String> str = String::NewFromUtf8(isolate, "{}");

  //CNextBody nb;
  int32_t value = args[0]->Int32Value();
  m_NB.SetGender(value);

  //const std::string strPath = args[1]->Int32Value();
  String::Utf8Value strUtf8(args[1]->ToString());
  std::string args1 = std::string(*strUtf8);
  //std::string args1 = *String::Utf8Value(args[1]->ToString());

  //printf("[%d, %s]", value, args1.c_str());

  //if (m_NB.SetFile("addon/20161013_125305_3914.stl"))
  if (m_NB.SetFile(args1))
  {
    const std::string strJson = m_NB.FindAllLandmarks();
    //printf("\n--[\n%s\n]--\n", strJson.c_str());
    str = String::NewFromUtf8(isolate, strJson.c_str());
  }

  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();

  args.GetReturnValue().Set(json);
}

void MyObject::GetBodySize(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 2)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsNumber() || !args[1]->IsString())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<String> str = String::NewFromUtf8(isolate, "{}");

  //CNextBody nb;
  int32_t value = args[0]->Int32Value();
  m_NB.SetGender(value);

  //const std::string strPath = args[1]->Int32Value();
  String::Utf8Value strUtf8(args[1]->ToString());
  std::string args1 = std::string(*strUtf8);
  //std::string args1 = *String::Utf8Value(args[1]->ToString());

  //printf("[%d, %s]", value, args1.c_str());

  //if (m_NB.SetFile("addon/20161013_125305_3914.stl"))
  if (m_NB.SetFile(args1))
  {
    const std::string strJson2 = m_NB.FindAllLandmarks(false);
    const std::string strJson = m_NB.MeasureBody();
    //printf("\n--[\n%s\n]--\n", strJson.c_str());
    str = String::NewFromUtf8(isolate, strJson.c_str());
  }

  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();

  args.GetReturnValue().Set(json);
}

void MyObject::CalcVnSData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  CGccAnalysis ga;
  const std::string strJson = ga.CalcVnSData();
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();

  //Local<Object> obj = Object::New(isolate);
  //obj->Set(String::NewFromUtf8(isolate, "hello"), String::NewFromUtf8(isolate, "Why hello there."));

  args.GetReturnValue().Set(json);
}

void MyObject::CalcLateralData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  CGccAnalysis ga;
  const std::string strJson = ga.CalcLateralData();
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcHipKneeData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  CGccAnalysis ga;
  const std::string strJson = ga.CalcHipKneeData();
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcSpineData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  CGccAnalysis ga;
  const std::string strJson = ga.CalcSpineData();
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcShapeData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  CGccAnalysis ga;
  const std::string strJson = ga.CalcShapeData();
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

} // namespace demo
