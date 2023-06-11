import com.android.build.gradle.internal.tasks.factory.dependsOn

plugins {
    id("java")
    application
    id("com.github.gmazzo.buildconfig") version ("3.1.0")
    id("io.github.tomtzook.gradle-cmake") version ("1.2.2")
    id("com.google.protobuf") version ("0.9.3")
}

val nativeLibPath = File(buildDir, "cmakeLibs")
val nativeLibFullName = File(nativeLibPath, "myapplication/linux-amd64/libmyapplication.so")

buildConfig {
    buildConfigField("String", "NATIVE_LIB_PATH", "\"${nativeLibFullName.absolutePath}\"")
}

cmake {
    targets {
        register("myapplication") {
            cmakeLists.set(file("src/main/cpp/CMakeLists.txt"))
            targetMachines.add(machines.host)
            outputDir.set(nativeLibPath)
        }
    }
}

protobuf {
    protoc {
        // Download from repositories
        artifact = "com.google.protobuf:protoc:3.0.0"
    }
    generateProtoTasks {
        all().forEach { task ->
            task.builtins {
                create("cpp")
            }
        }
    }
}


tasks.clean.dependsOn(tasks.cmakeClean)
tasks.assemble.dependsOn(tasks.cmakeBuild)
tasks.cmakeBuild.dependsOn(tasks.jar)

dependencies {
    implementation("org.junit.jupiter:junit-jupiter-api:5.9.1")

    implementation("com.google.protobuf:protobuf-java:3.19.6")
    implementation("io.grpc:grpc-stub:1.45.1")
    implementation("io.grpc:grpc-protobuf:1.45.1")
    if (JavaVersion.current().isJava9Compatible()) {
        // Workaround for @javax.annotation.Generated
        // see: https://github.com/grpc/grpc-java/issues/3633
        implementation("javax.annotation:javax.annotation-api:1.3.1")
    }
}

