# MO2 Notes Plugin

A Mod Organizer 2 plugin that adds a notes panel with Markdown support.

## Requirements

- Visual Studio 2022
- CMake 3.30+
- Qt 6.7.x (MSVC 2022 build)
- Python 3.12
- Mod Organizer 2 source tree (for `uibase` headers and `cmake_common`)
- vcpkg (for Boost dependencies)

## Building

### 1. Create `CMakeUserPresets.json`

This file is gitignored and contains your local paths. Create it in the repository root:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "vs2022-windows",
      "inherits": "vs2022-windows-base",
      "cacheVariables": {
        "DEPENDENCIES_DIR": {
          "type": "PATH",
          "value": "D:/path/to/Mod.Organizer-2.5.2-src"
        },
        "QT_ROOT": {
          "type": "PATH",
          "value": "C:/Qt/6.7.3/msvc2022_64"
        },
        "PYTHON_ROOT": {
          "type": "PATH",
          "value": "C:/Users/YOU/AppData/Local/Programs/Python/Python312"
        }
      }
    }
  ],
  "buildPresets": [
    {
      "name": "vs2022-windows",
      "configurePreset": "vs2022-windows",
      "configuration": "RelWithDebInfo"
    }
  ]
}
```

Adjust the paths to match your system:

| Variable | Description |
|----------|-------------|
| `DEPENDENCIES_DIR` | Path to MO2 source tree (must contain `cmake_common/mo2.cmake`) |
| `QT_ROOT` | Path to Qt MSVC installation (e.g., `C:/Qt/6.7.3/msvc2022_64`) |
| `PYTHON_ROOT` | Path to Python 3.12 installation |

### 2. Configure and Build

```bash
cmake --preset vs2022-windows
cmake --build --preset vs2022-windows
```

The plugin DLL will be output to `install/bin/plugins/mo2_notes.dll`.

### 3. Install

Copy `mo2_notes.dll` to your Mod Organizer 2 `plugins` folder.

## License

See [LICENSE](LICENSE).
