@page asset_manager Asset Manager
@tableofcontents
# Overview
The **Asset Manager** in Ares is a centralized system for managing assets such as textures, models, or any other resources used by the engine. It provides functionality for staging, loading, unloading and accessing assets, as well as a listener system for handling asset-related events.

The [AssetManager](#Ares::AssetManager) class is the core of asset management within the Ares Engine. It provides static methods for:
 - Staging assets using the asset's metadata.
 - Loading staged assets.
 - Unloading and unstaging assets.
 - Retrieving assets based on name or ID.
 - Retrieving a complete list of all assets.
 - Adding listeners based on an asset's name.
 - Adding global listeners for all asset events.

The [Asset Manager](#Ares::AssetManager) is truly a tool for the developers. The [Application](#Ares::Application) itself has very little control over the [Asset Manager](#Ares::AssetManager) other than initializing, shutting down, and processing the [Asset](#Ares::Asset) [Listeners](#Ares::AssetListener). Everything else is up to you as a developer.
@note The [AssetManager](#Ares::AssetManager) itself does not store any data. If the [AssetManager](#Ares::AssetManager) loads a file, it will store the data in a [DataBuffer](#Ares::DataBuffer) in the [MemoryDataProvider](#Ares::MemoryDataProvider).
# Key Features
 - **Staging**: Prepare assets by registering them in the system before loading.
 - **Loading and Unloading**: Load assets asynchronously, unload them when no longer needed, and handle dependencies efficiently.
 - **Customizable Callbacks**: AssetManager::Load takes an optional callback that is run when the Asset is either loaded or failed to load.
 - **Event Listeners**: Respond to asset-related events with custom callbacks.
 - **Asset Lookup**: Retrieve assets by name or ID for easy access.
 - **Efficient Processing**: Assets are loaded asynchronously using the ThreadPool, providing Asset loading with no blocking.
# How to Use the Asset Manager
## Initialization and Shutdown
Initialization and shutdown is handled within the Application class. If you are creating your own application logic, you will need to call these methods yourself.
<br>
**Example**:

```cpp
#include <Engine/Data/AssetManager.h>

int main() {
	// Initialize the Asset Manager
	Ares::AssetManager::Init();

	// Your code here

	// Shutdown the Asset Manager
	Ares::AssetManager::Shutdown();
	return 0;
}
```
## Staging Assets
Before an asset can be loaded, it must be staged. You can stage an asset with various configurations.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

// Basic staging with a name and filepath
AssetManager::Stage<Texture>("MyTexture", "path/to/texture.png");

// Staging with dependencies
AssetManager::Stage<ShaderProgram>("MyShaderProgram", {
	AssetManager::Stage<VertexShader>("ShaderProgram_VertShader", "path/to/vertex.glsl"),
	AssetManager::Stage<FragmentShader>("ShaderProgram_FragShader", "path/to/fragment.glsl")
});

// Staging with a memory data key
MemoryDataKey textureDataKey = MemoryDataProvider::RegisterData(dataPointer, dataSize);
Ref<Asset> textureAsset2 = AssetManager::Stage<Texture>("MyTexture2", textureDataKey);
```
To remove a staged asset:
```cpp
#include <Ares.h>
using namespace Ares;

AssetManager::Unstage(textureAsset2);
```
## Loading and Unloading Assets
### Loading Assets
Load a single asset or a group of assets with an optional callback.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

AssetManager::Load(myAsset, [](Ref<Asset> asset) {
	// Handle post-load logic
});

AssetManager::Load({ asset1, asset2 }, [](Ref<Asset> asset) {
	// Handle post-load logic
});
```
### Unloading Assets
Unload an asset when it's no longer needed.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

AssetManager::Unload(myAsset);
```
## Accessing Assets
Retrieve assets by name or ID.
<br>
**Example**:

```cpp
#include <Ares.h>
using namespace Ares;

Ref<Asset> assetByName = AssetManager::GetAsset("MyAssetName");
Ref<Asset> assetById = AssetManager::GetAsset(3);
```
To get a complete list of all assets, staged and loaded:
```cpp
#include <Ares.h>
using namespace Ares;

std::vector<Ref<Asset>> allAssets = AssetManager::GetCompleteList();
```
## Asset Listener System
With the AssetManager's listener system, you are able to add listeners to respond to asset-related events. You can add listeners either to specific assets or globally to all asset-related events.
### Adding Listeners

```cpp
#include <Ares.h>
using namespace Ares;

AssetListener listener1 = AssetManager::AddListener("MyAssetName", [](Event& event) {
	// Handle event for a specific asset
});

AssetListener listener2 = AssetManager::AddListener([](Event& event) {
	// Hanlde global asset events
});
```
@note The [AddListener](#Ares::AssetManager::AddListener) method takes a **void** function as the callback. It does not take a boolean function like [EventQueue::AddListener](#Ares::EventQueue::AddListener). This method cannot mark events as "Handled".
<br>
[Documentation](#Ares::AssetManager::AssetListenerCallbackFn)

### Removing Listeners

```cpp
#include <Ares.h>
using namespace Ares;

AssetListener listener = AssetManager::AddListener("MyAssetName", [](Event& event) {
	// Handle event for a specific asset
});

AssetManager::RemoveListener(listener);
```
# Relevant Documentation
 - Ares::Asset
 - Ares::AssetManager
 - Ares::AssetListener
 - Ares::DataBuffer
 - Ares::MemoryDataProvider
 - Ares::MemoryDataKey
 - Ares::Ref

<div class="section_buttons">
|Previous|Next|
|:-|-:|
|[Event System](#event_system)|[Classes](annotated.html)|
</div>