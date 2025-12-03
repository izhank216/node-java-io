#include <napi.h>
#include <jni.h>
#include <string>
#include <vector>

JavaVM* jvm = nullptr;
JNIEnv* env = nullptr;

void initJVM() {
    if (jvm != nullptr) return;

    JavaVMInitArgs vm_args;
    JavaVMOption options[1];
    options[0].optionString = (char*)"-Djava.class.path=./bin"; // compiled Java classes
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
}

jobjectArray vectorToJObjectArray(const std::vector<jobject>& vec) {
    jobjectArray array = env->NewObjectArray(vec.size(), env->FindClass("java/lang/Object"), nullptr);
    for (size_t i = 0; i < vec.size(); i++) {
        env->SetObjectArrayElement(array, i, vec[i]);
    }
    return array;
}

Napi::Value Create(const Napi::CallbackInfo& info) {
    Napi::Env napiEnv = info.Env();
    initJVM();

    std::string className = info[0].As<Napi::String>().Utf8Value();
    std::vector<jobject> args;

    for (size_t i = 1; i < info.Length(); i++) {
        Napi::Value val = info[i];
        if (val.IsString()) {
