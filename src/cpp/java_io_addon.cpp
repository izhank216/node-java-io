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
            args.push_back(env->NewStringUTF(val.As<Napi::String>().Utf8Value().c_str()));
        }
        // You can extend to other types (int, bool) if needed
    }

    jclass cls = env->FindClass(("io/JavaIOWrapper").c_str());
    jmethodID mid = env->GetStaticMethodID(cls, "create", "(Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");

    jobjectArray jargs = vectorToJObjectArray(args);
    jstring jclassName = env->NewStringUTF(className.c_str());

    jobject obj = env->CallStaticObjectMethod(cls, mid, jclassName, jargs);

    return Napi::External<jobject>::New(napiEnv, obj);
}

Napi::Value Call(const Napi::CallbackInfo& info) {
    Napi::Env napiEnv = info.Env();
    initJVM();

    jobject obj = info[0].As<Napi::External<jobject>>().Data();
    std::string methodName = info[1].As<Napi::String>().Utf8Value();

    std::vector<jobject> args;
    for (size_t i = 2; i < info.Length(); i++) {
        Napi::Value val = info[i];
        if (val.IsString()) {
            args.push_back(env->NewStringUTF(val.As<Napi::String>().Utf8Value().c_str()));
        }
    }

    jobjectArray jargs = vectorToJObjectArray(args);

    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetStaticMethodID(cls, "call", "(Ljava/lang/Object;Ljava/lang/String;[Ljava/lang/Object;)Ljava/lang/Object;");

    jstring jmethodName = env->NewStringUTF(methodName.c_str());
    jobject result = env->CallStaticObjectMethod(cls, mid, obj, jmethodName, jargs);

    return Napi::String::New(napiEnv, env->GetStringUTFChars((jstring)result, nullptr));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("create", Napi::Function::New(env, Create));
    exports.Set("call", Napi::Function::New(env, Call));
    return exports;
}

NODE_API_MODULE(java_io, Init)
