# Audio Processing and Sampling Tool

This C++ program allows you to perform various operations on audio files, such as upsampling, downsampling, applying a moving average filter, merging audio files, and more. It provides a menu-driven interface for easy interaction.

## How to Use

1. Run the program.
2. You will be prompted to enter the name of an audio file (in WAV format).
3. After loading the audio file, you can choose from the following operations:
   - **Up Sample the Audio**: Increases the sampling rate of the audio.
   - **Down Sample the Audio**: Decreases the sampling rate of the audio.
   - **Apply Moving Average Filter**: Smoothes the audio data using a moving average filter.
   - **Merge with Another Audio File**: Combines the loaded audio with another audio file.
   - **Load a New Audio File**: Load a different audio file.
   - **Play an Audio File**: Listen to the audio you have loaded.
   - **Exit**: Terminate the program.

## Features

### Memory Management

- Memory is allocated and deallocated properly, ensuring no memory leaks.
- The `cutArray` function is used to cut the array to the precise size of the audio file, optimizing memory usage.

### Audio Processing

- **Upsample Audio**: Increase the sampling rate of the audio file.
- **Downsample Audio**: Decrease the sampling rate of the audio file.
- **Apply Moving Average Filter**: Smooth the audio data using a moving average filter.
- **Merge Audio Files**: Combine the loaded audio with another audio file.
- **Load New Audio File**: Load a different audio file.

### Audio Playback

- You can play the loaded audio to listen to the processed or original audio.

## Author

This program was created by [Your Name].

Feel free to modify and use it as needed. If you have any questions or suggestions, please contact [Your Email].
