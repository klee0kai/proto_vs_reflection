package com.example;

import com.example.engine.JniEngine;
import com.example.engine.JniListener;
import com.example.model.Meta;
import com.example.model.ModelUtils;
import com.example.model.SomeCmd;
import com.example.model.SomeEvent;
import com.github.klee0kai.proto.Jni;
import com.google.protobuf.InvalidProtocolBufferException;

import java.util.Locale;
import java.util.function.Function;

import static org.junit.jupiter.api.Assertions.*;

public class Main {
    private static final JniEngine engine = new JniEngine();

    public static void main(String[] args) {
        System.out.print("test jni reflection test");
        runTest((cmd) -> {
            engine.cmd(cmd, false);
            return null;
        });


        System.out.print("test on indexed jni reflection");
        runTest((cmd) -> {
            engine.cmd(cmd, true);
            return null;
        });

        System.out.print("test proto serialize");
        runTest((cmd) -> {
            engine.cmdFlat(ModelUtils.toFlat(cmd));
            return null;
        });
    }

    private static void runTest(Function<SomeCmd, Void> bridge) {
        long startTime = System.currentTimeMillis();

        long testCount = 1000;
        long testProcessMark = testCount / 10;

        for (long i = testCount; i > 0; i--) {
            if (i % testProcessMark == 0) System.out.print(".");

            SomeEventListener listener = new SomeEventListener();

            engine.subscribe(listener);
            engine.subscribe(listener);

            SomeCmd cmd = new SomeCmd();
            cmd.id = 1;
            cmd.count = 2;
            cmd.name = "name";
            cmd.value = 1.3f;
            cmd.valueD = 1.5;
            cmd.meta = new Meta[]{
                    new Meta("1", "v1"),
                    new Meta("2", "v2")
            };
            bridge.apply(cmd);

            assertTrue(engine.unsubscribe(listener));
            assertTrue(engine.unsubscribe(listener));
            assertFalse(engine.unsubscribe(listener));

            assertEquals(2, listener.count);
            assertTrue(ModelUtils.eq(cmd, listener.event));
        }
        System.out.println(".");

        long endTime = System.currentTimeMillis();
        float spendTimeSec = (endTime - startTime) / 1e3f;
        System.out.printf(Locale.ROOT, "Test time %.3f%n", spendTimeSec);
    }

    static class SomeEventListener implements JniListener {

        int count = 0;
        SomeEvent event;

        @Override
        public void onEvent(SomeEvent event) {
            count++;
            this.event = event;
        }

        @Override
        public void onEvent(byte[] eventFlat) {

            try {
                Jni.SomeEventModel eventModel = Jni.SomeEventModel.parseFrom(eventFlat);
                this.event = new SomeEvent(
                        eventModel.getId(),
                        eventModel.getCount(),
                        eventModel.getValue(),
                        eventModel.getValueD(),
                        eventModel.getName()
                );

                event.meta = new Meta[eventModel.getMetaCount()];
                int i = 0;
                for (Jni.MetaModel metaModel : eventModel.getMetaList()) {
                    event.meta[i] = new Meta();
                    event.meta[i].tag = metaModel.getTag();
                    event.meta[i].value = metaModel.getValue();
                    i++;
                }

                count++;
            } catch (InvalidProtocolBufferException e) {
                throw new RuntimeException(e);
            }
        }
    }
}


