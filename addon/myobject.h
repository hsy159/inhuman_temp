#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <node.h>
#include <node_object_wrap.h>

#include "GSizerKLib/NextBody.h"
#include "GccAnalysis.h"

namespace demo
{

class MyObject : public node::ObjectWrap
{
public:
  static void Init(v8::Isolate *isolate);
  static void NewInstance(const v8::FunctionCallbackInfo<v8::Value> &args);

private:
  explicit MyObject(double value = 0);
  ~MyObject();

  static void New(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void PlusOne(const v8::FunctionCallbackInfo<v8::Value> &args);
  static v8::Persistent<v8::Function> constructor;

  double value_;

  static CNextBody m_NB;
  static void GetLandmark(const v8::FunctionCallbackInfo<v8::Value> &args);
  static void GetBodySize(const v8::FunctionCallbackInfo<v8::Value> &args);

  //CGccAnalysis m_ga;

  // CGccAnalysis
  static void CalcVnSData(const v8::FunctionCallbackInfo<v8::Value> &args);     // volume
  static void CalcLateralData(const v8::FunctionCallbackInfo<v8::Value> &args); // lateral
  static void CalcHipKneeData(const v8::FunctionCallbackInfo<v8::Value> &args); // hip&knee
  static void CalcSpineData(const v8::FunctionCallbackInfo<v8::Value> &args);   // scoliosis
  static void CalcShapeData(const v8::FunctionCallbackInfo<v8::Value> &args);   // shape
};

} // namespace demo

#endif
