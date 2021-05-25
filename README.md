# ofxSampleRate

openFrameworks addon for changing the speed or samplerate of ofSoundBuffer instances.

Uses [libsamplerate](https://github.com/libsndfile/libsamplerate) to perform the resampling.


## Compatibility
Currently only the macos compiled library of libsamplerate is included with this addon. If you want to use on another OS, please add the appropriate pre compiled static library to the `libs/libsamplerate/lib/<your system type>` folder.