## Proto Vs Reflection

Comparing the speed of reflection and proto serialization

This work compares between serialization and reflection for use with jni.
Protobuf is used as a serialization engine as a compact, fast and multi-platform tool.

Based on test results: 

[Run 1](https://github.com/klee0kai/proto_vs_reflection/actions/runs/6125603619),
```
> Task :term:run
test jni reflection test...........
Test time 1.272
test on indexed jni reflection...........
Test time 0.594
test proto serialize...........
Test time 1.034
```

[Run 2](https://github.com/klee0kai/proto_vs_reflection/actions/runs/6129867904),


```
> Task :term:run
test jni reflection test...........
Test time 1.266
test on indexed jni reflection...........
Test time 0.593
test proto serialize...........
Test time 1.013
```

[Run 3](https://github.com/klee0kai/proto_vs_reflection/actions/runs/6129871309)

```
> Task :term:run
test jni reflection test...........
Test time 1.303
test on indexed jni reflection...........
Test time 0.596
test proto serialize...........
Test time 1.052
```

Based on the results of the comparisons, we can conclude that reflection works longer than serialization.
However, for integration with jni it is better to use preliminary indexing of reflection - which gives greater speed


## License

```
Copyright (c) 2023 Andrey Kuzubov
```

