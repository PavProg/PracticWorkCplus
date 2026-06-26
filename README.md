# MyGameEngine

Учебный игровой движок на C++17 и OpenGL: ECS, загрузка моделей (.obj), текстур
(PNG/JPG) и шейдеров, менеджер ресурсов с кэшированием и горячей заменой.

Платформа: Windows (MSVC + vcpkg). Графика: OpenGL 3.3.

---

## Что нужно установить

1. **Git** — https://git-scm.com
2. **Visual Studio 2022** или **Build Tools for Visual Studio 2022**
   с компонентом **«Desktop development with C++»**.
   В нём сразу идут компилятор MSVC, CMake и Ninja — отдельно ставить их не нужно.
3. **vcpkg** — менеджер C++-зависимостей. Установка описана ниже.

> Все команды ниже выполняются в **«Developer PowerShell for VS 2022»**
> (Пуск → начните печатать «Developer PowerShell»). В обычном PowerShell
> команды `cmake`, `ninja`, `cl` могут быть не найдены.

---

## Установка vcpkg (один раз)

Нужен только сам vcpkg — библиотеки (`glfw3`, `glm`) вручную ставить не нужно.
Они перечислены в `vcpkg.json` и установятся автоматически при конфигурации.

Путь `C:\vcpkg` важен — он прописан в `CMakePresets.json`.

```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

Если vcpkg уже стоит в другом месте — см. раздел «Если vcpkg не в C:\vcpkg».

---

## Сборка и запуск

```powershell
# 1. Клонировать репозиторий
git clone <URL-репозитория>
cd Куда клонировали

# 2. Сконфигурировать (vcpkg сам подтянет glfw3 и glm по vcpkg.json)
cmake --preset windows-x64-debug

# 3. Собрать
cmake --build --preset windows-x64-debug

# 4. Запустить
.\build\windows-x64-debug\MyGameEngine.exe
```

Первая конфигурация идёт дольше — vcpkg скачивает и собирает `glfw3` и `glm`.
После сборки рядом с `.exe` автоматически копируется папка `assets`
и кладётся `glfw3.dll`. Лог работы пишется в `game.log` в той же папке.

---

## Возможные проблемы

| Проблема | Решение |
|---|---|
| `cmake` / `ninja` / `cl` не распознаётся | Запускайте из «Developer PowerShell for VS 2022». |
| `Could not read CMAKE_TOOLCHAIN_FILE … C:/vcpkg/...` | vcpkg не в `C:\vcpkg`. См. раздел ниже. |
| `find_package(glm/glfw3) … not found` | Проверьте, что в корне есть `vcpkg.json` и сборка идёт через пресет `windows-x64-debug` (vcpkg-toolchain). |
| При запуске: `glfw3.dll not found` | Скопируйте `glfw3.dll` из `build\windows-x64-debug\vcpkg_installed\x64-windows\bin` рядом с `.exe`. |
| Не находятся `assets` / текстуры | Запускайте `.exe` из его папки `build\windows-x64-debug\`. |

---

## Если vcpkg не в C:\vcpkg

Не редактируйте `CMakePresets.json` (он в репозитории). Создайте рядом
локальный `CMakeUserPresets.json` (он в `.gitignore`) и укажите свой путь:

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "my-debug",
      "inherits": "windows-x64-debug",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "D:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
      }
    }
  ]
}
```

И собирайте своим пресетом: `cmake --preset my-debug`.

---

## Рекомендации

- **Чистая пересборка.** Если после изменения структур или заголовков
  появляются странные ошибки (например, переполнение стека на ровном месте) —
  удалите папку `build\` и сконфигурируйте заново. Инкрементальная сборка
  иногда оставляет устаревшие объектные файлы.

- **Зависимости.** Список библиотек из vcpkg хранится в `vcpkg.json` в корне
  проекта. Они ставятся автоматически при `cmake --preset ...` (локально, в
  `build\...\vcpkg_installed\`). Чтобы добавить новую библиотеку — допишите её
  в раздел `dependencies` этого файла.

---