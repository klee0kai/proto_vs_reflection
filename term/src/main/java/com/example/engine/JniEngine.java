package com.example.engine;

import ProtoVsReflection.term.BuildConfig;
import com.example.model.SomeCmd;

public class JniEngine {

    static {
        System.load(BuildConfig.NATIVE_LIB_PATH);
    }

    public native String helloWorld();

    public native int cmd(SomeCmd cmd, boolean singleInitJni);

    public native int cmdFlat(byte[] cmd);

    public native boolean subscribe(JniListener listener);

    public native boolean unsubscribe(JniListener listener);


}
