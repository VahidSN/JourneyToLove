# Journey to Love (with C++ and Raylib)

**Journey to Love** is a 2D platformer built with modern C++ and the Raylib library. The player guides the main character through a challenging level filled with obstacles and checkpoints, with the ultimate goal of reaching the endpoint and reuniting with their love.

https://github.com/user-attachments/assets/47732319-eac5-4d48-a9ac-3f8805c4a61c

-----

## About This Project

This project was developed as a personal exploration to strengthen my understanding of game development fundamentals outside of Unity and to practice modern C++ programming. With over 10 years of professional experience in Unity and C#, I wanted to step closer to the low-level side of game development by working directly with Raylib.
The game makes use of Raylib’s systems for input handling, math structures (e.g., Vector2), rendering, and audio features.

Building everything from scratch allowed me to focus on fundamentals such as game loop structure, collision detection, and state management without relying on a full engine.
I’m sharing this as both a record of learning and a demonstration of adaptability. Feedback on design, structure, or implementation is very welcome as I continue expanding my C++ and engine-level skills alongside my Unity expertise.

-----

## Features

  * **Classic Platforming**: Navigate obstacles and reach the goal using "W," "A," and "D" for movement.
  * **Checkpoints**: Activate checkpoints to save your progress. Dying returns you to the most recently activated checkpoint.
  * **Customizable Level:** The level is loaded from a CSV file, making it easy to modify or create your own layout by editing the file.
  * **Dynamic UI**: A user interface updates in real-time to show your distance to the end goal.
  * **Sound and Music**: Enjoy a simple, engaging audio experience with background music and sound effects for key actions.
  * **Simple Art Style**: Uses clean, straightforward textures that provide a clear and pleasant visual experience.

-----

## How to Customize It

This project is designed to be easily modifiable. You can customize the game's level, visuals, and sounds to create your own unique experience.

### 1\. **Editing the Level**

The game level is defined in the `assets/level.csv` file. You can open and edit this file with any CSV editor (free version of ModernCSV is recommended) or a simple text editor to create your own level.

Each number in the CSV corresponds to a specific tile type:
  * **1**: Block (platform)
  * **2**: Hazard
  * **3**: Checkpoint
  * **4**: Endpoint
 
  *Important: row 23 is considered for the level length, so keep it all 1 until the end.*

### 2\. **Replacing Textures and Sounds**

All game assets are stored in the `assets/Textures` and `assets/Audios` folders. You can replace these files with your own custom images and sound files. Just make sure to keep the same file names and types for the game to load them correctly.

-----

## How It Works

The game's structure is built around a few core classes that manage different aspects of the game:

  * `Scene`: This is the central class that orchestrates the entire game. It manages the `Player` and `Level` objects, handles the main game loop, and controls the camera.
  * `Player`: This class represents the main character, handling all player-related logic, including movement, physics, and health. It uses the `CollisionUtils` namespace to detect and react to different parts of the level.
  * `Level`: This class is responsible for loading and rendering the game world. It reads the level data from a **CSV file**, which defines the placement of all platforms, hazards, and goals.
  * `UIController`: This class manages all the user interface elements, from the distance display to the end panel.

-----

## How to Set Up (Visual Studio)

This project is configured as a CMake project, which Visual Studio has native support for. This means you don't need to manually create a solution file.

### Prerequisites
  * **Visual Studio**: Make sure you have a recent version of Visual Studio installed that includes the **Desktop development with C++** workload.
  * **CMake**: You'll need CMake installed on your system.
  * **Raylib**: The Raylib library must be configured and available for CMake to find.

### Instructions
1.  **Clone the Repository and Open Folder in Visual Studio**:
      * Open Visual Studio.
      * Go to **File \> Open \> Folder...** and select the directory you just cloned.
2.  **CMake Configuration**:
      * Visual Studio will automatically detect the `CMakeLists.txt` file and start configuring the project. This will create a build directory (e.g., `out/build/x64-Debug`) for your build files. This folder should be ignored by Git by adding `out/` to your `.gitignore` file.
3.  **Build and Run**:
      * Once the configuration is complete, you can build and run the project directly from the Visual Studio toolbar.
      * Select the `JourneyToLove` executable from the startup item dropdown.
      * Press the green "play" button or press `F5` to build and run the game.

-----

## License

### Code

The code in this repository is shared for educational purposes.  
You are free to use, modify, and distribute the code for non-commercial or personal projects.

### Assets

This project uses royalty-free assets from the following sources:

**Audio (Pixabay):**
- [Jump](https://pixabay.com/sound-effects/jump-sound-effect-145285/) — freesound_community  
- [Music](https://pixabay.com/sound-effects/arcade-music-loop-6462/) — Gaston A-P  
- [Die](https://pixabay.com/sound-effects/video-game-death-sound-66829/) — freesound_community  
- [Checkpoint](https://pixabay.com/sound-effects/arcade-8-bit-video-game-level-up-98861/) — freesound_community  
- [End](https://pixabay.com/sound-effects/8-bit-video-game-level-completed-82807/) — freesound_community  

**Textures:**
- [Scribble Platformer Pack](https://kenney.nl/assets/scribble-platformer) — by Kenney  

> ⚠️ **Important:**  
> Third-party assets are subject to their own licenses and terms of use.  
> Please review each asset’s license if you plan to use it in your own projects, especially for commercial purposes.

---
