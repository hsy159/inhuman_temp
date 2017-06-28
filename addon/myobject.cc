#include <node.h>
#include "myobject.h"
#include "Gcc3dType.h"

namespace demo
{

using v8::Array;
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
CGccAnalysis MyObject::m_ga;

//MyObject::MyObject(double value) : value_(value)
MyObject::MyObject()
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
  //NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getLandmark", GetLandmark);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getBodySize", GetBodySize);
  NODE_SET_PROTOTYPE_METHOD(tpl, "loadFile", LoadFile);
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
    //double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    //MyObject *obj = new MyObject(value);
    MyObject *obj = new MyObject();
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

/*
void MyObject::PlusOne(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  MyObject *obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
  obj->value_ += 1;

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
*/

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

void MyObject::LoadFile(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsString())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  String::Utf8Value strUtf8(args[0]->ToString());
  std::string args0 = std::string(*strUtf8);

  m_ga.LoadFile(args0);
}

void MyObject::CalcVnSData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsArray())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<Array> aryArgs = Local<Array>::Cast(args[0]);
  int len = aryArgs->Length();

  if (8 != len)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong array arguments / req 8")));
    return;
  }

  /*
	input : Img/VnS_F.png, VnS_M.png
	m_ClickPoints 8개
	1 - 11 목부위 목뒤점 Cervicale >>>> 목뒤점
	2 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R
	3 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L
	4 - 28 몸통부위 젖가슴아래점* Inferior Breast >>>> 젖가슴아래점(UnderBust) >>>> 남자 값 필요!!!
	5 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
	6 - 40 몸통부위 샅점* Crotch >>>> 샅점(Crotch)
	7 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
	8 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
	*/

  VERTEX v;
  std::vector<VERTEX> vertex;

  for (int i = 0; i < len; i++)
  {
    Local<Object> obj = Local<Object>::Cast(aryArgs->Get(i));
    //printf("-----x[%f] y[%f] z[%f]\n", obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue(), obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue(), obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue());
    v.x = obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
    v.y = obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
    v.z = obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue();
    //printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
    vertex.push_back(v);
  }

  len = vertex.size();
  for (int i = 0; i < len; i++)
  {
    v = vertex[i];
    printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
  }

  //CGccAnalysis ga;
  const std::string strJson = m_ga.CalcVnSData(vertex);
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

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsArray())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<Array> aryArgs = Local<Array>::Cast(args[0]);
  int len = aryArgs->Length();

  if (10 != len)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong array arguments / req 10")));
    return;
  }

	/*
	input : Img/Lateral_F.png, Lateral_M.png
	m_ClickPoints 10개
	1 - 귀R 필요!!!
	2 - 귀L 필요!!!
	3 - 16 몸통부위 어깨가쪽점 -오른쪽과 왼쪽 Lateral Shoulder >>>> 어깨가쪽점R(Shoulder-R)
	4 - L
	5 - 엉덩이 돌출부의 옆점 RL 필요!!!
	6 - L
	7 - 무릎 관절점 RL 필요!!!
	8 - L
	9 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
	10 - L
	*/

  VERTEX v;
  std::vector<VERTEX> vertex;

  for (int i = 0; i < len; i++)
  {
    Local<Object> obj = Local<Object>::Cast(aryArgs->Get(i));
    v.x = obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
    v.y = obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
    v.z = obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue();
    //printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
    vertex.push_back(v);
  }

  //CGccAnalysis ga;
  const std::string strJson = m_ga.CalcLateralData(vertex);
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcHipKneeData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsArray())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<Array> aryArgs = Local<Array>::Cast(args[0]);
  int len = aryArgs->Length();

  if (6 != len)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong array arguments / req 6")));
    return;
  }

  /*
	input : Img/HipKnee_F.png, HipKnee_M.png
	m_ClickPoints 6개
	1 - 51 다리부위 가쪽복사점 Lateral Malleous >>>> 가쪽복사점(Ankle) RL 필요!!!
	2 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) R 필요!!!
	3 - 44 다리부위 무릎뼈가운데점 Midpatella >>>> 무릎뼈가운데점(Knee) L 필요!!!
	4 - 46 다리부위 넙다리가운데점 Midthigh 필요!!!
	5 - 67 팔부위 손목가쪽점 Radial Styloid >>>> 손목가쪽점-R(Wrist Out)
	6 - 67 팔부위 손목가쪽점 Radial Styloid >>>> L 필요!!!
	*/

  VERTEX v;
  std::vector<VERTEX> vertex;

  for (int i = 0; i < len; i++)
  {
    Local<Object> obj = Local<Object>::Cast(aryArgs->Get(i));
    v.x = obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
    v.y = obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
    v.z = obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue();
    //printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
    vertex.push_back(v);
  }

  //CGccAnalysis ga;
  const std::string strJson = m_ga.CalcHipKneeData(vertex);
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcSpineData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsArray())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<Array> aryArgs = Local<Array>::Cast(args[0]);
  int len = aryArgs->Length();

  if (5 != len)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong array arguments / req 5")));
    return;
  }

	/*
	input : Img/Spine_F.png, Spine_M.png
	m_ClickPoints 5개
	1 - 턱 점 필요!!!
	2 - 16 몸통부위 어깨가쪽점 Lateral Shoulder R >>>> 어깨가쪽점R(Shoulder-R)
	3 - 16 몸통부위 어깨가쪽점 Lateral Shoulder L >>>> 어깨가쪽점L(Shoulder-L)
	4 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점R(Armpit-R)
	5 - 18 몸통부위 겨드랑점* Axilla >>>> 겨드랑점L(Armpit-L)
	*/

  VERTEX v;
  std::vector<VERTEX> vertex;

  for (int i = 0; i < len; i++)
  {
    Local<Object> obj = Local<Object>::Cast(aryArgs->Get(i));
    v.x = obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
    v.y = obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
    v.z = obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue();
    //printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
    vertex.push_back(v);
  }

  //CGccAnalysis ga;
  const std::string strJson = m_ga.CalcSpineData(vertex);
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

void MyObject::CalcShapeData(const FunctionCallbackInfo<Value> &args)
{
  Isolate *isolate = args.GetIsolate();
  HandleScope scope(isolate);

  // Check the number of arguments passed.
  if (args.Length() < 1)
  {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  // Check the argument types
  if (!args[0]->IsArray())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Local<Array> aryArgs = Local<Array>::Cast(args[0]);
  int len = aryArgs->Length();

  if (3 != len)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong array arguments / req 3")));
    return;
  }
  
	/*
	input : Img/Shape_F.png, Shape_M.png
	m_ClickPoints 3개
	1 - 27 몸통부위 젖꼭지점 Nipple >>>> 젖꼭지점RL 평균
	2 - 34 몸통부위 배꼽점* Anterior Waist >>>> 배꼽점(Navel)
	3 - 38 몸통부위 엉덩이돌출점 Buttock Protrusion >>>> 엉덩이돌출점(Hip)
	*/

  VERTEX v;
  std::vector<VERTEX> vertex;

  for (int i = 0; i < len; i++)
  {
    Local<Object> obj = Local<Object>::Cast(aryArgs->Get(i));
    v.x = obj->Get(String::NewFromUtf8(isolate, "x"))->NumberValue();
    v.y = obj->Get(String::NewFromUtf8(isolate, "y"))->NumberValue();
    v.z = obj->Get(String::NewFromUtf8(isolate, "z"))->NumberValue();
    //printf("-----x[%f] y[%f] z[%f]\n", v.x, v.y, v.z);
    vertex.push_back(v);
  }

  //CGccAnalysis ga;
  const std::string strJson = m_ga.CalcShapeData(vertex);
  //printf("--%s--\n", strJson.c_str());
  Local<String> str = String::NewFromUtf8(isolate, strJson.c_str());
  Local<Value> json = JSON::Parse(isolate, str).ToLocalChecked();
  args.GetReturnValue().Set(json);
}

} // namespace demo
