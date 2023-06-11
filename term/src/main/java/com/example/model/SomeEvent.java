package com.example.model;

import java.util.Arrays;
import java.util.Objects;

public class SomeEvent {
    public int id;
    public long count;
    public float value;
    public double valueD;
    public String name;
    public Meta[] meta;


    public SomeEvent(int id, long count, float value, double valueD, String name) {
        this.id = id;
        this.count = count;
        this.value = value;
        this.valueD = valueD;
        this.name = name;
    }


    public SomeEvent(int id, long count, float value, double valueD, String name, Meta[] meta) {
        this.id = id;
        this.count = count;
        this.value = value;
        this.valueD = valueD;
        this.name = name;
        this.meta = meta;
    }


    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        SomeEvent someEvent = (SomeEvent) o;
        return id == someEvent.id && count == someEvent.count && Float.compare(someEvent.value, value) == 0 && Double.compare(someEvent.valueD, valueD) == 0 && Objects.equals(name, someEvent.name) && Arrays.equals(meta, someEvent.meta);
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(id, count, value, valueD, name);
        result = 31 * result + Arrays.hashCode(meta);
        return result;
    }
}
