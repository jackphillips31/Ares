@page getting_started Getting Started
# Prerequisites
 - **Premake 5**: You'll need [Premake5](https://premake.github.io/download) to generate project files.
 - **Git**: To clone the repository.
# Cloning the Repository
```bash
git clone --recursive https://github.com/jackphillips31/ares.git
```
# Generating Project Files
The current `premake5.lua` is set up with the internal project named **Sandbox**. If you'd like to create a new blank project, such as **MyApplication**, follow these steps:
1. Download and install [Premake5](https://premake.github.io/download) if you haven't already.
2. Place `premake5.exe` inside the `/scripts` folder.
3. In the scripts folder of the cloned repository, run the `InitializeProject.bat` script.
	- It will ask for a project name and internal project name and will create a new `premake5.lua` based on that.
	- It will then ask if you'd like it to generate a script for running **Premake**. This will create a `GenerateProjectFiles.bat` in the `/scripts` folder.
	- Finally, it will ask you if you would like to run the generated script to create your project files.
4. With your project files generated, you are ready to start working on your blank project!

<div class="section_buttons">
|Previous|Next|
|:-|-:|
|[Home](#mainpage)|[Usage](#usage)|
</div>