# AmpStudio

## Images
![Alt text](git-resources/preview-1.png "Connected device and plugin window")

## About
Audio processing software host which is targeting guitar.

The goal is to be able to write plugins with lua, and be cross platform. To be able to build amps, effect pedals with modern looking gui from any device.

## Getting started
Get source by ```git clone --recursive https://github.com/blackout1471/AmpStudio.git```
Run the GenerateSolution.bat to generate solution for guitarblade.

## Lua Documentation
### Audio effects plugins
The audio buffers range goes from -1.0f -> 1.0f

Lua plugins has be placed in plugin folder in root of exe to be loaded.
The lua files has to have the functions
```lua
function OnInit()
end

-- The sample argument is the buffer for the audio
-- and works like a normal array object in lua
function OnBufferUpdate(sample)
end
```

### Gui
All gui related functions should be called only in OnInit.
```lua
param = parameter.add(name, max, min, step) -- Adds a effect parameter.

parameter.getValue(param) -- Get the current value of the effect parameter.
```
### Audio
[TBC]

## Supported platforms
As of right now the application supports: Windows
The plan is to get it to build for Windows, Linux, ARM and mac os.

## Libraries
* Lua
* ImGui
* Asio (Window & Mac OS)
* Spdlog
* FFTConvolver -- plans to replace with own.

## Future
In the future i would like the application to be able to have a browse section where users can upload their effect plugins and be able download other created plugins.

## Contribute
We welcome contributions to AmpStudio! If you'd like to contribute, please follow these steps:

Fork the repository and create a new branch for your feature or bug fix.
Make your changes and test them thoroughly.
Create a pull request to merge your changes into the main repository.
Please make sure to follow the existing code style and conventions, and to thoroughly test your changes before submitting a pull request.

If you're unsure about anything, feel free to open an issue to discuss it.

## Reporting Bugs
If you encounter a bug in AmpStudio, please open an issue on the GitHub repository with a detailed description of the bug and steps to reproduce it. Please include any relevant error messages or logs.

## Feature Requests
If you have an idea for a new feature or improvement to AmpStudio, please open an issue on the GitHub repository with a detailed description of the feature and its use case. We'll review the request and let you know if it's something we can add to the project.
