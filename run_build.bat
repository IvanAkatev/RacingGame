@echo off
set PROJECT_NAME=RacingGame

:: 1. Очистка и конфигурация
if exist build rmdir /s /q build
cmake -S . -B build -G "Visual Studio 18 2026"

:: 2. Сборка проекта
cmake --build build --config Debug

:: 3. Проверка: если сборка прошла успешно, запускаем игру
if %ERRORLEVEL% EQU 0 (
    echo.
    echo --- Zapusk programmy... ---
    .\build\Debug\racing.exe
) else (
    echo.
    echo [!] Sborka zavershilas s oshibkoy.
)

pause