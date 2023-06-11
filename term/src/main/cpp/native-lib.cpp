#include <jni.h>
#include <string>
#include "model/some_cmd.h"
#include "model/some_event.h"
#include "model/meta.h"
#include "com_example_engine_JniEngine.h"
#include "jni.pb.h"


std::list<jobject> listeners;
int jniInited = 0;

struct JSomeCmdObject {
    jfieldID idField;
    jfieldID countField;
    jfieldID valueField;
    jfieldID valueDField;
    jfieldID nameField;
    jfieldID metaField;
} jSomeCmdObject;

struct JSomeEventObject {
    jclass jClass;
    jmethodID initMethod;
} jSomeEventObject;

struct JMetaObject {
    jclass jClass;
    jmethodID initMethod;
    jfieldID tagField;
    jfieldID valueField;
} jMetaObject;

struct JEngineObject {
    jmethodID onEventMethod;
    jmethodID onEventMethodBytes;
} jListenerObject;

void initJni(JNIEnv *env) {
    // cmd
    jSomeCmdObject.idField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"), "id", "I");
    jSomeCmdObject.countField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"), "count", "J");
    jSomeCmdObject.valueField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"), "value", "F");
    jSomeCmdObject.valueDField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"), "valueD", "D");
    jSomeCmdObject.nameField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"), "name",
                                               "Ljava/lang/String;");
    jSomeCmdObject.metaField = env->GetFieldID(env->FindClass("com/example/model/SomeCmd"),
                                               "meta", "[Lcom/example/model/Meta;");


    // meta
    jMetaObject.jClass = (jclass) env->NewGlobalRef(env->FindClass("com/example/model/Meta"));
    jMetaObject.initMethod = env->GetMethodID(env->FindClass("com/example/model/Meta"), "<init>", "()V");
    jMetaObject.tagField = env->GetFieldID(env->FindClass("com/example/model/Meta"),
                                           "tag", "Ljava/lang/String;");
    jMetaObject.valueField = env->GetFieldID(env->FindClass("com/example/model/Meta"),
                                             "value", "Ljava/lang/String;");


    // event
    jSomeEventObject.jClass = (jclass) env->NewGlobalRef(env->FindClass("com/example/model/SomeEvent"));
    jSomeEventObject.initMethod = env->GetMethodID(env->FindClass("com/example/model/SomeEvent"),
                                                   "<init>", "(IJFDLjava/lang/String;[Lcom/example/model/Meta;)V");

    //listener
    jListenerObject.onEventMethod = env->GetMethodID(
            env->FindClass("com/example/engine/JniListener"),
            "onEvent", "(Lcom/example/model/SomeEvent;)V"
    );

    jListenerObject.onEventMethodBytes = env->GetMethodID(
            env->FindClass("com/example/engine/JniListener"),
            "onEvent", "([B)V"
    );
}

/*
 * Class:     com_example_engine_JniEngine
 * Method:    cmdFlat
 * Signature: ([B)I
 */
JNIEXPORT jint JNICALL Java_com_example_engine_JniEngine_cmdFlat(JNIEnv *env, jobject, jbyteArray jCmd) {
    if (!jniInited) {
        initJni(env);
        jniInited = 1;
    }
    int len = env->GetArrayLength(jCmd);
    jbyte *cmdFlat = env->GetByteArrayElements(jCmd, 0);
    SomeCmdModel cmd = SomeCmdModel{};
    cmd.ParseFromArray(cmdFlat, len);
    env->ReleaseByteArrayElements(jCmd, cmdFlat, 0);

    SomeEventModel eventModel{};
    eventModel.set_id(cmd.id());
    eventModel.set_count(cmd.count());
    eventModel.set_value(cmd.value());
    eventModel.set_valued(cmd.valued());
    eventModel.set_name(cmd.name());
    for (const auto &item: cmd.meta()) {
        MetaModel *m = eventModel.add_meta();
        m->set_tag(item.tag());
        m->set_value(item.value());
    }

    auto eventSize = eventModel.ByteSizeLong();
    jbyteArray jEventFlat = env->NewByteArray((int) eventSize);
    jbyte *eventFlat = new jbyte[eventSize];
    eventModel.SerializePartialToArray(eventFlat, (int) eventSize);
    env->SetByteArrayRegion(jEventFlat, 0, eventSize, eventFlat);
    for (const auto &item: listeners) {
        env->CallObjectMethod((jobject) item, jListenerObject.onEventMethodBytes, jEventFlat);
    }
    delete[]eventFlat;
    env->DeleteLocalRef(jEventFlat);

    return 1;
}


/*
 * Class:     com_example_engine_JniEngine
 * Method:    helloWorld
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_com_example_engine_JniEngine_helloWorld(JNIEnv *env, jobject) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


/*
 * Class:     com_example_engine_JniEngine
 * Method:    cmd
 * Signature: (Lcom/example/model/SomeCmd;)I
 */
JNIEXPORT jint JNICALL
Java_com_example_engine_JniEngine_cmd(JNIEnv *env, jobject, jobject jCmd, jboolean useSinglePreInit) {
    if (!jniInited) {
        initJni(env);
        jniInited = useSinglePreInit;
    }
    // mapping to cmd
    struct some_cmd cmd{};
    cmd.id = env->GetIntField(jCmd, jSomeCmdObject.idField);
    cmd.count = env->GetLongField(jCmd, jSomeCmdObject.countField);
    cmd.value = env->GetFloatField(jCmd, jSomeCmdObject.valueField);
    cmd.valueD = env->GetDoubleField(jCmd, jSomeCmdObject.valueDField);

    jstring jName = (jstring) env->GetObjectField(jCmd, jSomeCmdObject.nameField);
    const char *name = jName != NULL ? env->GetStringUTFChars(jName, NULL) : NULL;
    cmd.name = std::string(name ?: "");
    if (name != NULL) env->ReleaseStringUTFChars(jName, name);

    auto jMetas = (jobjectArray) env->GetObjectField(jCmd, jSomeCmdObject.metaField);
    auto len = jMetas ? env->GetArrayLength(jMetas) : 0;

    for (int i = 0; i < len; i++) {
        struct meta m{};
        jobject jItem = env->GetObjectArrayElement(jMetas, i);

        jstring jTag = (jstring) env->GetObjectField(jItem, jMetaObject.tagField);
        const char *tag = jName != NULL ? env->GetStringUTFChars(jTag, NULL) : NULL;
        m.tag = std::string(tag ?: "");
        if (name != NULL) env->ReleaseStringUTFChars(jTag, tag);

        jstring jValue = (jstring) env->GetObjectField(jItem, jMetaObject.valueField);
        const char *value = jName != NULL ? env->GetStringUTFChars(jValue, NULL) : NULL;
        m.value = std::string(value ?: "");
        if (name != NULL) env->ReleaseStringUTFChars(jValue, value);

        cmd.metas.push_back(m);
    }

    struct some_event event{};
    event.id = cmd.id;
    event.count = cmd.count;
    event.value = cmd.value;
    event.valueD = cmd.valueD;
    event.name = cmd.name;
    event.metas = cmd.metas;

    {
        jstring jEventName = env->NewStringUTF((const char *) event.name.c_str());

        jobjectArray jMetaArray = env->NewObjectArray(event.metas.size(), jMetaObject.jClass, 0);
        auto indx = 0;
        for (const auto &item: event.metas) {
            jobject jMeta = env->NewObject(jMetaObject.jClass, jMetaObject.initMethod);

            jstring jTag = env->NewStringUTF((const char *) item.tag.c_str());
            jstring jValue = env->NewStringUTF((const char *) item.value.c_str());
            env->SetObjectField(jMeta, jMetaObject.tagField, jTag);
            env->SetObjectField(jMeta, jMetaObject.valueField, jValue);
            env->DeleteLocalRef(jTag);
            env->DeleteLocalRef(jValue);

            env->SetObjectArrayElement(jMetaArray, indx++, jMeta);
            env->DeleteLocalRef(jMeta);
        }

        jobject jEvent = env->NewObject(jSomeEventObject.jClass, jSomeEventObject.initMethod,
                                        event.id, event.count, event.value, event.valueD, jEventName, jMetaArray
        );
        for (const auto &item: listeners) {
            env->CallObjectMethod((jobject) item, jListenerObject.onEventMethod, jEvent);
        }

        env->DeleteLocalRef(jEventName);
        env->DeleteLocalRef(jEvent);
        env->DeleteLocalRef(jMetaArray);
    }


    return 1;
}


/*
 * Class:     com_example_engine_JniEngine
 * Method:    subscribe
 * Signature: (Lcom/example/engine/JniListener;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_engine_JniEngine_subscribe(JNIEnv *env, jobject, jobject jListener) {
    jobject ref = env->NewGlobalRef(jListener);
    listeners.push_back(ref);
    return 1;
}

/*
 * Class:     com_example_engine_JniEngine
 * Method:    unsubscribe
 * Signature: (Lcom/example/engine/JniListener;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_engine_JniEngine_unsubscribe(JNIEnv *env, jobject, jobject jListener) {
    auto it = listeners.begin();
    while (it != listeners.end()) {
        if (env->IsSameObject(*it, jListener)) {
            env->DeleteGlobalRef(*it);
            listeners.erase(it);
            return 1;
        }
    }
    return 0;
}