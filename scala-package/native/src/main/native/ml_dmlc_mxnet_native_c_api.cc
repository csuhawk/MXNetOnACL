#include <iostream>
#include <cstdlib>
#include <functional>
#include <mxnet/ndarray.h>
#include <mxnet/kvstore.h>
#include <mxnet/symbolic.h>
#include <mxnet/c_api.h>

#include "jni_helper_func.h"
#include "ml_dmlc_mxnet_native_c_api.h" // generated by javah

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayCreateNone
  (JNIEnv *env, jobject obj, jobject ndArrayHandle) {
  NDArrayHandle out;
  int ret = MXNDArrayCreateNone(&out);
  jclass ndClass = env->GetObjectClass(ndArrayHandle);
  jfieldID ptr = env->GetFieldID(ndClass, "value", "J");
  env->SetLongField(ndArrayHandle, ptr, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayCreate(JNIEnv *env, jobject obj,
                                                              jintArray shape,
                                                              jint ndim,
                                                              jint devType,
                                                              jint devId,
                                                              jint delayAlloc,
                                                              jobject ndArrayHandle) {
  jint *shapeArr = env->GetIntArrayElements(shape, NULL);
  NDArrayHandle out;
  int ret = MXNDArrayCreate((mx_uint *)shapeArr, (mx_uint)ndim, devType, devId, delayAlloc, &out);
  env->ReleaseIntArrayElements(shape, shapeArr, 0);
  jclass ndClass = env->GetObjectClass(ndArrayHandle);
  jfieldID ptr = env->GetFieldID(ndClass, "value", "J");
  env->SetLongField(ndArrayHandle, ptr, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayWaitAll(JNIEnv *env, jobject obj) {
  return MXNDArrayWaitAll();
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxListFunctions
  (JNIEnv *env, jobject obj, jobject functions) {
  jclass longCls = env->FindClass("java/lang/Long");
  jmethodID longConst = env->GetMethodID(longCls, "<init>", "(J)V");

  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  // Get function list
  FunctionHandle *outArray;
  mx_uint outSize;
  int ret = MXListFunctions(&outSize, &outArray);
  for (int i = 0; i < outSize; ++i) {
    env->CallObjectMethod(functions, listAppend,
                          env->NewObject(longCls, longConst, outArray[i]));
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncDescribe
  (JNIEnv *env, jobject obj, jlong funcPtr, jobject nUsedVars,
   jobject nScalars, jobject nMutateVars, jobject typeMask) {
  mx_uint numUseVars;
  mx_uint numScalars;
  mx_uint numMutateVars;
  int type;
  int ret = MXFuncDescribe((FunctionHandle)funcPtr, &numUseVars,
                            &numScalars, &numMutateVars, &type);

  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID value = env->GetFieldID(refIntClass, "value", "I");
  env->SetIntField(nUsedVars, value, (jint)numUseVars);
  env->SetIntField(nScalars, value, (jint)numScalars);
  env->SetIntField(nMutateVars, value, (jint)numMutateVars);
  env->SetIntField(typeMask, value, (jint)type);

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncGetInfo
  (JNIEnv *env, jobject obj, jlong funcPtr, jobject name,
   jobject desc, jobject numArgs, jobject argNames, jobject argTypes, jobject argDescs) {
  const char *cName;
  const char *cDesc;
  mx_uint cNumArgs;
  const char **cArgNames;
  const char **cArgTypes;
  const char **cArgDescs;
  int ret = MXFuncGetInfo((FunctionHandle)funcPtr,
                          &cName, &cDesc, &cNumArgs,
                          &cArgNames, &cArgTypes, &cArgDescs);

  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID valueInt = env->GetFieldID(refIntClass, "value", "I");

  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");

  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass, "$plus$eq",
      "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  env->SetObjectField(name, valueStr, env->NewStringUTF(cName));
  env->SetObjectField(desc, valueStr, env->NewStringUTF(cDesc));
  env->SetIntField(numArgs, valueInt, (jint)cNumArgs);
  for (int i = 0; i < cNumArgs; ++i) {
    env->CallObjectMethod(argNames, listAppend, env->NewStringUTF(cArgNames[i]));
    env->CallObjectMethod(argTypes, listAppend, env->NewStringUTF(cArgTypes[i]));
    env->CallObjectMethod(argDescs, listAppend, env->NewStringUTF(cArgDescs[i]));
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxFuncInvoke
  (JNIEnv *env, jobject obj, jlong funcPtr, jlongArray useVars,
   jfloatArray scalarArgs, jlongArray mutateVars) {
  jlong *cUseVars = env->GetLongArrayElements(useVars, NULL);
  jfloat *cScalarArgs = env->GetFloatArrayElements(scalarArgs, NULL);
  jlong *cMutateVars = env->GetLongArrayElements(mutateVars, NULL);
  int ret = MXFuncInvoke((FunctionHandle)funcPtr,
                         (NDArrayHandle *)cUseVars,
                         (mx_float *)cScalarArgs,
                         (NDArrayHandle *)cMutateVars);
  env->ReleaseLongArrayElements(useVars, cUseVars, 0);
  env->ReleaseFloatArrayElements(scalarArgs, cScalarArgs, 0);
  env->ReleaseLongArrayElements(mutateVars, cMutateVars, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayGetShape
  (JNIEnv *env, jobject obj, jlong ndArrayPtr, jobject ndimRef, jobject dataBuf) {
  mx_uint ndim;
  const mx_uint *pdata;
  int ret = MXNDArrayGetShape((NDArrayHandle)ndArrayPtr, &ndim, &pdata);

  // fill dataBuf
  jclass integerClass = env->FindClass("java/lang/Integer");
  jmethodID newInteger = env->GetMethodID(integerClass, "<init>", "(I)V");

  jclass arrayClass = env->FindClass("scala/collection/mutable/ArrayBuffer");
  jmethodID arrayAppend = env->GetMethodID(arrayClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ArrayBuffer;");
  for (int i = 0; i < ndim; ++i) {
    jobject data = env->NewObject(integerClass, newInteger, pdata[i]);
    env->CallObjectMethod(dataBuf, arrayAppend, data);
  }

  // set ndimRef
  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID valueInt = env->GetFieldID(refIntClass, "value", "I");
  env->SetIntField(ndimRef, valueInt, ndim);

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySyncCopyToCPU
  (JNIEnv *env, jobject obj, jlong ndArrayPtr, jfloatArray data, jint size) {
  jfloat *pdata = env->GetFloatArrayElements(data, NULL);
  int ret = MXNDArraySyncCopyToCPU((NDArrayHandle)ndArrayPtr, (mx_float *)pdata, size);
  env->ReleaseFloatArrayElements(data, pdata, 0); // copy back to java array automatically
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySlice
  (JNIEnv *env, jobject obj, jlong ndArrayPtr, jint start, jint end, jobject slicedHandle) {
  NDArrayHandle out;
  int ret = MXNDArraySlice((NDArrayHandle)ndArrayPtr, start, end, &out);
  setLongField(env, slicedHandle, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArraySyncCopyFromCPU
  (JNIEnv *env, jobject obj, jlong arrayPtr, jfloatArray sourceArr, jint arrSize) {
  jfloat *sourcePtr = env->GetFloatArrayElements(sourceArr, NULL);
  int ret = MXNDArraySyncCopyFromCPU((NDArrayHandle)arrayPtr, (const mx_float *)sourcePtr, arrSize);
  env->ReleaseFloatArrayElements(sourceArr, sourcePtr, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxNDArrayFree
  (JNIEnv * env, jobject obj, jlong ndArrayHandle) {
  return MXNDArrayFree((NDArrayHandle) ndArrayHandle);
}

// The related c api MXKVStoreSetUpdater function takes a c function pointer as its parameter,
// while we write java functions here in scala-package.
// Thus we have to wrap the function in a java object, and run env->CallVoidMethod(obj) once updater is invoked,
// which implies the function registered to KVStore must be stateful.
// This is why we re-implement MXKVStoreSetUpdater as follows.
JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreSetUpdater(JNIEnv *env, jobject obj,
                                                                      jobject kvStoreHandle,
                                                                      jobject updaterFuncObj,
                                                                      jobject updaterHandle) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jobject updaterFuncObjGlb = env->NewGlobalRef(updaterFuncObj);
  jobject updaterHandleGlb = env->NewGlobalRef(updaterHandle);
  std::function<void(int, const mxnet::NDArray&, mxnet::NDArray*)> updt
  = [env, updaterFuncObjGlb, updaterHandleGlb](int key, const mxnet::NDArray& recv, mxnet::NDArray* local) {
    // find java updater method
    jclass updtClass = env->GetObjectClass(updaterFuncObjGlb);
    jmethodID updtFunc = env->GetMethodID(updtClass,
      "update", "(ILml/dmlc/mxnet/NDArray;Lml/dmlc/mxnet/NDArray;Ljava/lang/Object;)V");

    // find java NDArray constructor
    jclass ndObjClass = env->FindClass("ml/dmlc/mxnet/NDArray");
    jmethodID ndObjConstructor = env->GetMethodID(ndObjClass, "<init>", "(JZ)V");

    mxnet::NDArray *recvCopy = new mxnet::NDArray();
    *recvCopy = recv;
    jobject jNdRecvCopy = env->NewObject(ndObjClass, ndObjConstructor, (jlong)recvCopy, true);

    mxnet::NDArray *localCopy = new mxnet::NDArray();
    *localCopy = *local;
    jobject jNdLocalCopy = env->NewObject(ndObjClass, ndObjConstructor, (jlong)localCopy, true);

    env->CallVoidMethod(updaterFuncObjGlb, updtFunc, key, jNdRecvCopy, jNdLocalCopy, updaterHandleGlb);
    env->DeleteGlobalRef(updaterFuncObjGlb);
    env->DeleteGlobalRef(updaterHandleGlb);
  };
  try {
    static_cast<mxnet::KVStore*>((KVStoreHandle)kvStorePtr)->set_updater(updt);
  } catch(dmlc::Error &except) {
    // It'll be too complicated to set & get mx error in jni code.
    // thus simply return -1 to indicate a failure.
    // Notice that we'll NOT be able to run MXGetLastError
    // to get the error message after this function fails.
    return -1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreCreate(JNIEnv *env, jobject obj,
                                                                  jstring name,
                                                                  jobject kvStoreHandle) {
  jclass refLongClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
  jfieldID refLongFid = env->GetFieldID(refLongClass, "value", "J");

  KVStoreHandle out;
  const char *type = env->GetStringUTFChars(name, 0);
  int ret = MXKVStoreCreate(type, &out);
  env->ReleaseStringUTFChars(name, type);

  env->SetLongField(kvStoreHandle, refLongFid, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreInit(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray values) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *valueArray = env->GetLongArrayElements(values, NULL);
  int ret = MXKVStoreInit((KVStoreHandle) kvStorePtr,
                          (mx_uint) len,
                          (const int *) keyArray,
                          (NDArrayHandle *) valueArray);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(values, valueArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStorePush(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray values,
                                                                jint priority) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *valueArray = env->GetLongArrayElements(values, NULL);
  int ret = MXKVStorePush((KVStoreHandle)kvStorePtr,
                          (mx_uint)len,
                          (const int *)keyArray,
                          (NDArrayHandle *)valueArray,
                          priority);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(values, valueArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStorePull(JNIEnv *env, jobject obj,
                                                                jobject kvStoreHandle,
                                                                jint len,
                                                                jintArray keys,
                                                                jlongArray outs,
                                                                jint priority) {
  // get kv store ptr
  jlong kvStorePtr = getLongField(env, kvStoreHandle);

  jint *keyArray = env->GetIntArrayElements(keys, NULL);
  jlong *outArray = env->GetLongArrayElements(outs, NULL);
  int ret = MXKVStorePull((KVStoreHandle)kvStorePtr,
                          (mx_uint)len,
                          (const int *)keyArray,
                          (NDArrayHandle *)outArray,
                          priority);
  env->ReleaseIntArrayElements(keys, keyArray, 0);
  env->ReleaseLongArrayElements(outs, outArray, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetType
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject kvType) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  const char *type;
  int ret = MXKVStoreGetType((KVStoreHandle)kvStorePtr, &type);
  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");
  env->SetObjectField(kvType, valueStr, env->NewStringUTF(type));
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreSendCommmandToServers
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jint head, jstring body) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  const char *bodyCStr = env->GetStringUTFChars(body, 0);
  int ret = MXKVStoreSendCommmandToServers((KVStoreHandle)kvStorePtr, head, bodyCStr);
  env->ReleaseStringUTFChars(body, bodyCStr);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreBarrier
  (JNIEnv *env, jobject obj, jobject kvStoreHandle) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  return MXKVStoreBarrier((KVStoreHandle)kvStorePtr);
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetGroupSize
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject sizeRef) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  int size;
  int ret = MXKVStoreGetGroupSize((KVStoreHandle)kvStorePtr, &size);
  setIntField(env, sizeRef, size);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxKVStoreGetRank
  (JNIEnv *env, jobject obj, jobject kvStoreHandle, jobject rankRef) {
  jlong kvStorePtr = getLongField(env, kvStoreHandle);
  int rank;
  int ret = MXKVStoreGetRank((KVStoreHandle)kvStorePtr, &rank);
  setIntField(env, rankRef, rank);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorOutputs
  (JNIEnv *env, jobject obj, jobject executorHandle, jobject outputs) {

  jlong executorPtr = getLongField(env, executorHandle);
  mx_uint outSize;
  NDArrayHandle *out;
  int ret = MXExecutorOutputs((ExecutorHandle)executorPtr, &outSize, &out);

  jclass longCls = env->FindClass("java/lang/Long");
  jmethodID longConst = env->GetMethodID(longCls, "<init>", "(J)V");

  // fill java outputs
  jclass arrayClass = env->FindClass("scala/collection/mutable/ArrayBuffer");
  jmethodID arrayAppend = env->GetMethodID(arrayClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ArrayBuffer;");
  for (int i = 0; i < outSize; ++i) {
    env->CallObjectMethod(outputs, arrayAppend,
                          env->NewObject(longCls, longConst, out[i]));
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorFree
  (JNIEnv * env, jobject obj, jobject handle) {
  jlong ptr = getLongField(env, handle);
  return MXExecutorFree((ExecutorHandle) ptr);
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorForward
  (JNIEnv * env, jobject obj, jobject handle, jint isTrain) {
  jlong ptr = getLongField(env, handle);
  return MXExecutorForward((ExecutorHandle)ptr, (int)isTrain);
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorBackward
  (JNIEnv * env, jobject obj, jobject handle, jint gradsSize, jlongArray grads) {
  jlong executorPtr = getLongField(env, handle);
  jlong *gradArr = env->GetLongArrayElements(grads, NULL);
  int ret = MXExecutorBackward((ExecutorHandle)executorPtr,
                               (mx_uint)gradsSize,
                               (NDArrayHandle *)gradArr);
  env->ReleaseLongArrayElements(grads, gradArr, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorPrint
  (JNIEnv * env, jobject obj, jobject handle, jobject debugStr) {
  jlong ptr = getLongField(env, handle);
  const char *retDebugStr;
  int ret = MXExecutorPrint((ExecutorHandle)handle, &retDebugStr);
  setStringField(env, debugStr, retDebugStr);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxExecutorSetMonitorCallback
  (JNIEnv *env, jobject obj, jobject handle, jobject callbackFuncObj) {
  jlong executorPtr = getLongField(env, handle);
  jobject callbackFuncObjGlb = env->NewGlobalRef(callbackFuncObj);
  std::function<void(const char *, NDArrayHandle)> callback
  = [env, callbackFuncObjGlb](const char *name, NDArrayHandle array) {
    // find java callback method
    jclass callbackClass = env->GetObjectClass(callbackFuncObjGlb);
    jmethodID invokeFunc = env->GetMethodID(callbackClass,
      "invoke", "(Ljava/lang/String;Lml/dmlc/mxnet/Base$RefLong;)V");

    jstring jname = env->NewStringUTF(name);
    // ndArray handle
    jclass ndHandleClass = env->FindClass("ml/dmlc/mxnet/Base$RefLong");
    jmethodID ndHandleCont = env->GetMethodID(ndHandleClass,"<init>","(J)V");
    jobject jNDArrayHandle = env->NewObject(ndHandleClass, ndHandleCont, (long)array);

    env->CallVoidMethod(callbackFuncObjGlb, invokeFunc, jname, jNDArrayHandle);
    env->DeleteGlobalRef(callbackFuncObjGlb);
  };
  /* TODO: we need to modify Executor::SetMonitorCallback, make it take std::function as param
  try {
    mxnet::Executor *exec = static_cast<mxnet::Executor*>((ExecutorHandle)executorPtr);
    exec->SetMonitorCallback(callback);
  } catch(dmlc::Error &except) {
    // It'll be too complicated to set & get mx error in jni code.
    // thus simply return -1 to indicate a failure.
    // Notice that we'll NOT be able to run MXGetLastError
    // to get the error message after this function fails.
    return -1;
  }
  */
  return 0;
}

JNIEXPORT jstring JNICALL Java_ml_dmlc_mxnet_LibInfo_mxGetLastError(JNIEnv * env, jobject obj) {
  char *tmpstr = "MXNetError";
  jstring rtstr = env->NewStringUTF(tmpstr);
  return rtstr;
}

//IO funcs
JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxListDataIters
  (JNIEnv * env, jobject obj, jobject creators) {
    jclass longCls = env->FindClass("java/lang/Long");
    jmethodID longConst = env->GetMethodID(longCls, "<init>", "(J)V");

    // scala.collection.mutable.ListBuffer append method
    jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
    jmethodID listAppend = env->GetMethodID(listClass,
      "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

    // Get function list
    DataIterCreator *outArray;
    mx_uint outSize;
    int ret = MXListDataIters(&outSize, &outArray);
    for (int i = 0; i < outSize; ++i) {
      env->CallObjectMethod(creators, listAppend,
                            env->NewObject(longCls, longConst, (long)outArray[i]));
    }
    return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterCreateIter
  (JNIEnv * env, jobject obj, jlong creator, jobjectArray jkeys,
   jobjectArray jvals, jobject dataIterHandleRef) {
  //keys and values
  int paramSize = env->GetArrayLength(jkeys);
  char** keys = new char*[paramSize];
  char** vals = new char*[paramSize];
  jstring jkey, jval;
  //use strcpy and release char* created by JNI inplace
  for(int i=0; i<paramSize; i++) {
    jkey = (jstring) env->GetObjectArrayElement(jkeys, i);
    const char* ckey = env->GetStringUTFChars(jkey, 0);
    keys[i] = new char[env->GetStringLength(jkey)];
    strcpy(keys[i], ckey);
    env->ReleaseStringUTFChars(jkey, ckey);

    jval = (jstring) env->GetObjectArrayElement(jvals, i);
    const char* cval = env->GetStringUTFChars(jval, 0);
    vals[i] = new char[env->GetStringLength(jval)];
    strcpy(vals[i], cval);
    env->ReleaseStringUTFChars(jval, cval);
  }

  //create iter
  DataIterHandle out;
  int ret = MXDataIterCreateIter((DataIterCreator)creator,
                                  (mx_uint) paramSize,
                                  (const char**) keys,
                                  (const char**) vals,
                                  &out);
  setLongField(env, dataIterHandleRef, (long)out);

  //release keys and vals
  for(int i=0; i<paramSize; i++) {
      delete[] keys[i];
      delete[] vals[i];
  }
  delete[] keys;
  delete[] vals;

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetIterInfo
  (JNIEnv * env, jobject obj, jlong creator, jobject jname,
   jobject jdesc, jobject jargNames, jobject jargTypeInfos, jobject jargDescs) {
  const char* name;
  const char* description;
  mx_uint numArgs;
  const char** argNames;
  const char** argTypeInfos;
  const char** argDescs;
  int ret = MXDataIterGetIterInfo((DataIterCreator)creator,
                                   &name,
                                   &description,
                                   &numArgs,
                                   &argNames,
                                   &argTypeInfos,
                                   &argDescs);

  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");
  //set params
  env->SetObjectField(jname, valueStr, env->NewStringUTF(name));
  env->SetObjectField(jdesc, valueStr, env->NewStringUTF(description));
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");
  for(int i=0; i<numArgs; i++) {
    env->CallObjectMethod(jargNames, listAppend, env->NewStringUTF(argNames[i]));
    env->CallObjectMethod(jargTypeInfos, listAppend, env->NewStringUTF(argTypeInfos[i]));
    env->CallObjectMethod(jargDescs, listAppend, env->NewStringUTF(argDescs[i]));
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterFree
  (JNIEnv *env, jobject obj, jlong handle) {
  int ret = MXDataIterFree((DataIterHandle) handle);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterBeforeFirst
  (JNIEnv *env, jobject obj, jlong handle) {
  int ret = MXDataIterBeforeFirst((DataIterHandle) handle);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterNext
  (JNIEnv *env, jobject obj, jlong handle, jobject out) {
  int cout;
  int ret = MXDataIterNext((DataIterHandle)handle, &cout);
  setIntField(env, out, cout);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetLabel
  (JNIEnv *env, jobject obj, jlong handle, jobject ndArrayHandleRef) {
  NDArrayHandle out;
  int ret = MXDataIterGetLabel((DataIterHandle)handle, &out);
  setLongField(env, ndArrayHandleRef, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetData
  (JNIEnv *env, jobject obj, jlong handle, jobject ndArrayHandleRef) {
  NDArrayHandle out;
  int ret = MXDataIterGetData((DataIterHandle)handle, &out);
  setLongField(env, ndArrayHandleRef, (jlong)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetIndex
  (JNIEnv *env, jobject obj, jlong handle, jobject outIndex, jobject outSize) {
  uint64_t* coutIndex;
  uint64_t coutSize;
  int ret = MXDataIterGetIndex((DataIterHandle)handle, &coutIndex, &coutSize);
  //set field
  setLongField(env, outSize, (long)coutSize);
  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  for(int i=0; i<coutSize; i++) {
    env->CallObjectMethod(outIndex, listAppend, (jlong)coutIndex[i]);
  }
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxDataIterGetPadNum
  (JNIEnv *env, jobject obj, jlong handle, jobject pad) {
  int cpad;
  int ret = MXDataIterGetPadNum((DataIterHandle)handle, &cpad);
  setIntField(env, pad, cpad);
  return ret;
}

// Symbol functions
JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolListAtomicSymbolCreators
  (JNIEnv *env, jobject obj, jobject symbolList) {
  mx_uint outSize;
  AtomicSymbolCreator *outArray;
  int ret = MXSymbolListAtomicSymbolCreators(&outSize, &outArray);

  jclass longCls = env->FindClass("java/lang/Long");
  jmethodID longConst = env->GetMethodID(longCls, "<init>", "(J)V");

  jclass listCls = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listCls,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  for (int i = 0; i < outSize; ++i) {
    env->CallObjectMethod(symbolList, listAppend,
                          env->NewObject(longCls, longConst, outArray[i]));
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolGetAtomicSymbolInfo
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobject name, jobject desc, jobject numArgs,
   jobject argNames, jobject argTypes, jobject argDescs, jobject keyVarNumArgs) {

  const char *cName;
  const char *cDesc;
  mx_uint cNumArgs;
  const char **cArgNames;
  const char **cArgTypes;
  const char **cArgDescs;
  const char *cKeyVarNumArgs;

  int ret = MXSymbolGetAtomicSymbolInfo((AtomicSymbolCreator) symbolPtr,
                                         &cName, &cDesc, &cNumArgs,
                                         &cArgNames, &cArgTypes, &cArgDescs,
                                         &cKeyVarNumArgs);

  jclass refIntClass = env->FindClass("ml/dmlc/mxnet/Base$RefInt");
  jfieldID valueInt = env->GetFieldID(refIntClass, "value", "I");

  jclass refStringClass = env->FindClass("ml/dmlc/mxnet/Base$RefString");
  jfieldID valueStr = env->GetFieldID(refStringClass, "value", "Ljava/lang/String;");

  // scala.collection.mutable.ListBuffer append method
  jclass listClass = env->FindClass("scala/collection/mutable/ListBuffer");
  jmethodID listAppend = env->GetMethodID(listClass, "$plus$eq",
      "(Ljava/lang/Object;)Lscala/collection/mutable/ListBuffer;");

  env->SetObjectField(name, valueStr, env->NewStringUTF(cName));
  env->SetObjectField(desc, valueStr, env->NewStringUTF(cDesc));
  env->SetObjectField(keyVarNumArgs, valueStr, env->NewStringUTF(cKeyVarNumArgs));
  env->SetIntField(numArgs, valueInt, (jint)cNumArgs);
  for (int i = 0; i < cNumArgs; ++i) {
    env->CallObjectMethod(argNames, listAppend, env->NewStringUTF(cArgNames[i]));
    env->CallObjectMethod(argTypes, listAppend, env->NewStringUTF(cArgTypes[i]));
    env->CallObjectMethod(argDescs, listAppend, env->NewStringUTF(cArgDescs[i]));
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolCreateAtomicSymbol
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobjectArray paramKeys,
   jobjectArray paramVals, jobject symbolRef) {
  int paramSize = env->GetArrayLength(paramKeys);
  const char **keys = new const char*[paramSize];
  const char **vals = new const char*[paramSize];
  for (int i = 0; i < paramSize; i++) {
    jstring key = (jstring) env->GetObjectArrayElement(paramKeys, i);
    const char *rawKey = env->GetStringUTFChars(key, 0);
    keys[i] = rawKey;

    jstring value = (jstring) env->GetObjectArrayElement(paramVals, i);
    const char *rawValue = env->GetStringUTFChars(value, 0);
    vals[i] = rawValue;
  }

  SymbolHandle out;
  int ret = MXSymbolCreateAtomicSymbol(
    (AtomicSymbolCreator) symbolPtr, (mx_uint) paramSize, keys, vals, &out);
  setLongField(env, symbolRef, (jlong) out);

  // release keys and vals
  for (int i = 0; i < paramSize; i++) {
    jstring key = (jstring) env->GetObjectArrayElement(paramKeys, i);
    env->ReleaseStringUTFChars(key, keys[i]);
    jstring value = (jstring) env->GetObjectArrayElement(paramVals, i);
    env->ReleaseStringUTFChars(value, vals[i]);
  }
  delete[] keys;
  delete[] vals;

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolSetAttr
  (JNIEnv *env, jobject obj, jlong symbolPtr, jstring jkey, jstring jvalue) {
  const char *ckey = env->GetStringUTFChars(jkey, 0);
  const char *cvalue = env->GetStringUTFChars(jvalue, 0);
  int ret = MXSymbolSetAttr((SymbolHandle) symbolPtr, ckey, cvalue);
  env->ReleaseStringUTFChars(jkey, ckey);
  env->ReleaseStringUTFChars(jvalue, cvalue);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolCompose
  (JNIEnv *env, jobject obj, jlong symbolPtr, jstring jname,
   jobjectArray jkeys, jlongArray jargs) {
  int argSize = env->GetArrayLength(jargs);
  const char **keys = NULL;
  if (jkeys != NULL) {
    keys = new const char*[argSize];
    for (int i = 0; i < argSize; i++) {
      jstring jkey = (jstring) env->GetObjectArrayElement(jkeys, i);
      const char *key = env->GetStringUTFChars(jkey, 0);
      keys[i] = key;
    }
  }
  jlong *args = env->GetLongArrayElements(jargs, NULL);
  const char *name = env->GetStringUTFChars(jname, 0);
  int ret = MXSymbolCompose((SymbolHandle) symbolPtr,
                            name, (mx_uint) argSize, keys,
                            (SymbolHandle*) args);
  // release allocated memory
  if (jkeys != NULL) {
    for (int i = 0; i < argSize; i++) {
      jstring jkey = (jstring) env->GetObjectArrayElement(jkeys, i);
      env->ReleaseStringUTFChars(jkey, keys[i]);
    }
    delete[] keys;
  }
  env->ReleaseStringUTFChars(jname, name);
  env->ReleaseLongArrayElements(jargs, args, 0);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolCreateVariable
  (JNIEnv *env, jobject obj, jstring jname, jobject handle) {
  SymbolHandle out;
  const char *name = env->GetStringUTFChars(jname, 0);
  int ret = MXSymbolCreateVariable(name, &out);
  env->ReleaseStringUTFChars(jname, name);
  setLongField(env, handle, (long)out);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolGetAttr
  (JNIEnv *env, jobject obj, jlong symbolPtr, jstring jkey, jobject retRef, jobject successRef) {

  const char *out;
  int success;
  const char *key = env->GetStringUTFChars(jkey, 0);
  int ret = MXSymbolGetAttr((SymbolHandle) symbolPtr, key, &out, &success);
  env->ReleaseStringUTFChars(jkey, key);

  setStringField(env, retRef, out);
  setIntField(env, successRef, success);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolListArguments
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobject arguments) {
  mx_uint outSize;
  const char **outStrArray;
  int ret = MXSymbolListArguments((SymbolHandle) symbolPtr, &outSize, &outStrArray);

  jclass arrayClass = env->FindClass("scala/collection/mutable/ArrayBuffer");
  jmethodID arrayAppend = env->GetMethodID(arrayClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ArrayBuffer;");
  for (int i = 0; i < outSize; i++) {
    jstring argument = env->NewStringUTF(outStrArray[i]);
    env->CallObjectMethod(arguments, arrayAppend, argument);
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolListOutputs
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobject outputs) {
  mx_uint outSize;
  const char **outStrArray;
  int ret = MXSymbolListOutputs((SymbolHandle) symbolPtr, &outSize, &outStrArray);

  jclass arrayClass = env->FindClass("scala/collection/mutable/ArrayBuffer");
  jmethodID arrayAppend = env->GetMethodID(arrayClass,
    "$plus$eq", "(Ljava/lang/Object;)Lscala/collection/mutable/ArrayBuffer;");
  for (int i = 0; i < outSize; i++) {
    jstring output = env->NewStringUTF(outStrArray[i]);
    env->CallObjectMethod(outputs, arrayAppend, output);
  }

  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolCopy
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobject clonedSymbolRef) {
  SymbolHandle clonedSymbol;
  int ret = MXSymbolCopy((SymbolHandle) symbolPtr, &clonedSymbol);
  setLongField(env, clonedSymbolRef, (long)clonedSymbol);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolCreateGroup
  (JNIEnv *env, jobject obj, jlongArray jsymbols, jobject out) {
  int numSymbols = env->GetArrayLength(jsymbols);
  SymbolHandle handle;
  jlong *symbols = env->GetLongArrayElements(jsymbols, NULL);
  int ret = MXSymbolCreateGroup(numSymbols, (SymbolHandle *)symbols, &handle);
  env->ReleaseLongArrayElements(jsymbols, symbols, 0);
  setLongField(env, out, (long)handle);
  return ret;
}

JNIEXPORT jint JNICALL Java_ml_dmlc_mxnet_LibInfo_mxSymbolPrint
  (JNIEnv *env, jobject obj, jlong symbolPtr, jobject out) {
  const char *outStr;
  int ret = MXSymbolPrint((SymbolHandle) symbolPtr, &outStr);
  setStringField(env, out, outStr);
  return ret;
}
