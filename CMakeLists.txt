
cmake_minimum_required(VERSION 3.25)

project(MyVST VERSION 0.1.0 LANGUAGES C CXX)

# Include JUCE's CMake targets and functions
add_subdirectory(external/JUCE)

set(PLUGIN_NAME "MyVST")
# Declare our audio plugin
juce_add_plugin(MyVST
    COMPANY_NAME "acidSounds"
    PLUGIN_MANUFACTURER_CODE Acid
    PLUGIN_CODE Mvst
    FORMATS VST3 Standalone
    PRODUCT_NAME "MyVST"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

target_compile_features(MyVST PRIVATE cxx_std_23)

target_compile_definitions(
    MyVST
    PRIVATE
    PLUGIN_NAME="${PLUGIN_NAME}"
)

# Files that belong to our plugin
target_sources(MyVST
    PRIVATE
        src/PluginProcessor.cpp
        src/MySynth.cpp
        # src/PluginEditor.cpp
)

# JUCE modules that our plugin depends on
target_link_libraries(MyVST
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(MyVST
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)
