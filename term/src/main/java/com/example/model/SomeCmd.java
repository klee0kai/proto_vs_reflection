package com.example.model;

import java.util.Arrays;
import java.util.Objects;

public class SomeCmd {
    public int id;
    public long count;
    public float value;
    public double valueD;
    public String name;
    public Meta[] meta;

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        SomeCmd cmd = (SomeCmd) o;
        return id == cmd.id && count == cmd.count && Float.compare(cmd.value, value) == 0 && Double.compare(cmd.valueD, valueD) == 0 && Objects.equals(name, cmd.name) && Arrays.equals(meta, cmd.meta);
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(id, count, value, valueD, name);
        result = 31 * result + Arrays.hashCode(meta);
        return result;
    }
}
