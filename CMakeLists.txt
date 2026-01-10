cmake_minimum_required(VERSION 4.0)
project(Synth  VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

# Chemin vers JUCE - ajustez selon votre installation
set(JUCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/JUCE")

# Ajouter JUCE au projet
add_subdirectory(${JUCE_DIR} JUCE)

juce_add_gui_app(Synth
        PRODUCT_NAME "Basic Synth"
)

juce_generate_juce_header(Synth)

target_sources(
        Synth
        PRIVATE
        src/main.cpp
        src/MainComponent.cpp
        src/MainComponent.h
        #src/SynthEngine.cpp
        #src/SynthEngine.h
)

target_compile_definitions(Synth
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:Synth,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:Synth,JUCE_VERSION>"
)

target_link_libraries(Synth
        PRIVATE
        juce::juce_gui_extra
        juce::juce_audio_utils
        juce::juce_core
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)


#add_executable(Synth src/main.cpp)
