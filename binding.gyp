{
  "targets": [
    {
      "target_name": "java_io",
      "sources": [ "src/cpp/java_io_addon.cpp" ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")",
        "/usr/lib/jvm/java-17-temurin/include",
        "/usr/lib/jvm/java-17-temurin/include/linux"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cxxflags!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    },
    {
      "target_name": "java_lang",
      "sources": [ "src/cpp/java_lang_addon.cpp" ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")",
        "/usr/lib/jvm/java-17-temurin/include",
        "/usr/lib/jvm/java-17-temurin/include/linux"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cxxflags!": ["-fno-exceptions"],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    }
  ]
}
