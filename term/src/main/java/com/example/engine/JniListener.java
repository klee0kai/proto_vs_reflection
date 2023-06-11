package com.example.engine;

import com.example.model.SomeEvent;

public interface JniListener {

    void onEvent(SomeEvent event);

    void onEvent(byte[] event);

}
