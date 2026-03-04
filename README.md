# Racing Skeleton (SFML 3.x + vcpkg)

Кроссплатформенный скелет (macOS + Windows) для вашей игры.
`src/core` — логика (без SFML), `src/app` — окно/ввод/рендер через SFML 3.

## Требования
- CMake 3.16+
- Компилятор C++17
- Ninja (рекомендуется)
- vcpkg

---

## 1) Установка vcpkg (один раз)

### macOS
```bash
git clone https://github.com/microsoft/vcpkg ~/vcpkg
cd ~/vcpkg
./bootstrap-vcpkg.sh
echo 'export VCPKG_ROOT="$HOME/vcpkg"' >> ~/.zshrc
source ~/.zshrc
```

### Windows (PowerShell)
```powershell
git clone https://github.com/microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
setx VCPKG_ROOT "$PWD"
```
Перезапусти терминал после `setx`.

---

## 2) Сборка и запуск (Ninja)

Из корня проекта:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

Запуск:
- macOS/Linux:
```bash
./build/racing
```
- Windows (PowerShell):
```powershell
.\build\racing.exe
```

---

## 3) Visual Studio (Windows)
В "Developer PowerShell for VS 2022":
```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Debug
.\build\Debug\racing.exe
```

---

## Управление
- Space — газ
- Esc — выход
