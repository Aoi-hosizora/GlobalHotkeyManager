# GlobalHotkeyManager

+ A background tool built by Qt used to manage global hotkeys in Windows.

### Environment

+ Qt 5.15.2 (MinGW 8.1.0 64-bit)
+ Windows 10 21H2 x64

### Build and deploy

+ Build

```bash
# 1. Qmake and generate Makefile
mkdir build
cd build
qmake ..

# 2. Build in debug mode (with console)
make debug

# Or build in release mode (without console)
make release

# Or build in both debug and release modes
make all

# 3. Run the built executable
./build/debug/GlobalHotkeyManager.exe

# Or
./build/release/GlobalHotkeyManager.exe
```

+ For Multi-language supported (English, Chinese, Japanese)

```bash
# TODO
```

+ Deploy with Qt dependencies

```bash
# 1. Build GlobalHotkeyManager in release mode first
mkdir build
cd build
qmake ..
make release

# 2. Deploy to self-contained folder
cd ..
mkdir dist
cp ./build/release/GlobalHotkeyManager.exe ./dist/GlobalHotkeyManager.exe
windeployqt ./dist/GlobalHotkeyManager.exe

# 3. Run the deployed executable (which can be executed in no-qt-environment)
./dist/GlobalHotkeyManager.exe
```

### Screenshots

TODO

### Tips

TODO

### Reference

+ [ddqd/qxtglobalshortcut5](https://github.com/ddqd/qxtglobalshortcut5)
+ [Aoi-hosizora/OpenWithMenu](https://github.com/Aoi-hosizora/OpenWithMenu)
+ [ShellExecuteW function (shellapi.h)](https://docs.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shellexecutew)
+ [RegOpenKeyExW function (winreg.h)](https://docs.microsoft.com/en-us/windows/win32/api/winreg/nf-winreg-regopenkeyexw)
+ [List of Stylable Widgets - QListView](https://doc.qt.io/qt-5/stylesheet-reference.html#qlistview-widget)
+ [Deploying Translations](https://doc.qt.io/qt-5/linguist-programmers.html#deploying-translations)
+ [Undocumented QMake](https://wiki.qt.io/Undocumented_QMake)
