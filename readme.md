# Attempt Count

Edit the current or total level attempt counts in the game geometry dash. You can also edit the jumps and time.

There is no undo button! I'm not responsible for losing attempts.

<img src="https://github.com/Moebits/Attempt-Count/blob/main/image.png?raw=true" height="300">

### Advanced Note

Because I couldn't figure out what corresponds to the time label, this mod actually tracks its own level playtime and replaces it on the end screen. 

### Installation

1. Install [Geode](https://geode-sdk.org/).
2. Place the `.geode` file from [releases](https://github.com/Moebits/Attempt-Count/releases) in the geode/mods folder located where you installed Geometry Dash.

### Building

Install [Geode CLI](https://docs.geode-sdk.org/getting-started/geode-cli) and the sdk.

Release build - Run `geode build`. It is automatically installed to Geometry Dash if set up correctly.

Debug build - I run with the debugger in VSCode. You may have to edit the launch.json with the path to Geometry Dash on your device.

### Credits

- [Geode](https://geode-sdk.org/)
