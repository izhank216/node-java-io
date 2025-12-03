package io;

import java.io.*;
import java.lang.reflect.*;

public class JavaIOWrapper {

    // Generic method to create any java.io class instance
    public static Object create(String className, Object... args) throws Exception {
        Class<?> cls = Class.forName("java.io." + className);
        for (Constructor<?> ctor : cls.getConstructors()) {
            if (ctor.getParameterCount() == args.length) {
                return ctor.newInstance(args);
            }
        }
        throw new RuntimeException("No matching constructor found for " + className);
    }

    // Generic method to call any method on java.io class
    public static Object call(Object obj, String methodName, Object... args) throws Exception {
        Class<?> cls = obj.getClass();
        for (Method m : cls.getMethods()) {
            if (m.getName().equals(methodName) && m.getParameterCount() == args.length) {
                return m.invoke(obj, args);
            }
        }
        throw new RuntimeException("No matching method " + methodName + " found");
    }
}
