package com.example.model;

import com.github.klee0kai.proto.Jni;

import java.util.Arrays;
import java.util.Objects;

public class ModelUtils {

    public static boolean eq(SomeCmd cmd, SomeEvent event) {
        return cmd.id == event.id
                && cmd.count == event.count
                && Objects.equals(cmd.name, event.name)
                && cmd.value == event.value
                && cmd.valueD == event.valueD
                && Arrays.equals(cmd.meta, event.meta);

    }

    public static byte[] toFlat(SomeCmd cmd) {
        Jni.SomeCmdModel.Builder builder = Jni.SomeCmdModel.newBuilder()
                .setId(cmd.id)
                .setCount(cmd.count)
                .setValue(cmd.value)
                .setValueD(cmd.valueD)
                .setName(cmd.name);
        for (Meta meta : cmd.meta) {
            builder.addMeta(
                    Jni.MetaModel.newBuilder()
                            .setTag(meta.tag)
                            .setValue(meta.value)
                            .build()
            );
        }
        return builder.build().toByteArray();
    }
}
