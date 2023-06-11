package com.example.model;

import java.util.Objects;

public class Meta {
    public String tag;
    public String value;

    public Meta() {
    }

    public Meta(String tag, String value) {
        this.tag = tag;
        this.value = value;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Meta meta = (Meta) o;
        return Objects.equals(tag, meta.tag) && Objects.equals(value, meta.value);
    }

    @Override
    public int hashCode() {
        return Objects.hash(tag, value);
    }
}
