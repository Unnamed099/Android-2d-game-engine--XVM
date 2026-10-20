# 项目开发文档（中英对照） / Project Development Documentation (Bilingual)

> **项目 / Project**：`com.qianlan.cannon` — Android 原生 2D 游戏 + 自研脚本语言虚拟机 / Android native 2D game with a custom script-language VM
> **来源 / Sources**：由《开发文档.md》《Development_Documentation.md》《JSON与X语言开发参考.md》合成；全部事实已对照 `src/` 源码核实 / Merged from the Chinese & English docs, verified against the `src/` source tree.
> **格式 / Format**：每节标题“英文 / 中文”，正文先英文段、后中文段 / Each section: "English / 中文" heading, English text first, then Chinese.
> **约定 / Conventions**：废弃历史代码（`action` / `cmds` / `CommandNode` / `CmdFunction` / 未实现内建 / 跨模块 `goto`）勿动、勿推理；有效脚本路径只有 `vm_code` + `code_index.json`；模块间通信用 `persist`。Deprecated legacy code is intentionally left untouched; the only effective script path is `vm_code` + `code_index.json`; inter-module communication uses `persist`.

---

## 目录 / Table of Contents

**Part I 开发文档 / Development Documentation**

1. [Project Overview / 项目概述](#1-project-overview--项目概述)
2. [Tech Stack & Project Layout / 技术栈与工程形态](#2-tech-stack--project-layout--技术栈与工程形态)
3. [High-Level Architecture / 总体架构](#3-high-level-architecture--总体架构)
4. [Java Shell / Java 壳层](#4-java-shell--java-壳层)
5. [JNI Bridge & GL Backend / JNI 桥接与 GL 渲染后端](#5-jni-bridge--gl-backend--jni-桥接与-gl-渲染后端)
6. [Game Main Loop / 游戏主循环](#6-game-main-loop--游戏主循环)
7. [Global Data Bus / 全局数据总线](#7-global-data-bus--全局数据总线)
8. [X Language VM / X 语言虚拟机](#8-x-language-vm--x-语言虚拟机)
9. [Procedural World Generation / 程序化世界生成](#9-procedural-world-generation--程序化世界生成)
10. [Unit System / 单位系统](#10-unit-system--单位系统)
11. [Animation System / 动画系统](#11-animation-system--动画系统)
12. [JSON-Driven UI / JSON 驱动 UI](#12-json-driven-ui--json-驱动-ui)
13. [Input Controller / 输入控制](#13-input-controller--输入控制)
14. [Persistence / 持久化](#14-persistence--持久化)
15. [Utility Libraries / 基础工具库](#15-utility-libraries--基础工具库)
16. [Resource & Data Format Reference / 资源与数据格式参考](#16-resource--data-format-reference--资源与数据格式参考)
17. [X Language Quick Reference / X 语言速查](#17-x-language-quick-reference--x-语言速查)
18. [Built-in Function Groups / 内建函数分组](#18-built-in-function-groups--内建函数分组)
19. [Build & Run / 构建与运行](#19-build--run--构建与运行)
20. [Key Design Notes / 关键设计说明](#20-key-design-notes--关键设计说明)
21. [Current Status & Conventions / 当前状态与开发约定](#21-current-status--conventions--当前状态与开发约定)

**Part II JSON 与 X 语言参考 / JSON & X Language Reference**

1. [Assets Index System / Assets 索引体系](#1-assets-index-system--assets-索引体系)
2. [Common JSON Conventions / 通用 JSON 约定](#2-common-json-conventions--通用-json-约定)
3. [Unit Definition JSON / 单位定义 JSON](#3-unit-definition-json--单位定义-json)
4. [Animation JSON / 动画 JSON](#4-animation-json--动画-json)
5. [Skeleton Animation JSON / 骨骼动画 JSON](#5-skeleton-animation-json--骨骼动画-json)
6. [UI JSON / UI JSON](#6-ui-json--ui-json)
7. [Map / Biome JSON / 地图与生物群系 JSON](#7-map--biome-json--地图与生物群系-json)
8. [Tile Index JSON / 图块索引 JSON](#8-tile-index-json--图块索引-json)
9. [Structure JSON / 结构贴图 JSON](#9-structure-json--结构贴图-json)
10. [Command Templates / 指令模板 JSON](#10-command-templates--指令模板-json)
11. [X Language: File Organization & Run Chain / X 语言：文件组织与运行链路](#11-x-language-file-organization--run-chain--x-语言文件组织与运行链路)
12. [X Language: Lexical / X 语言：词法](#12-x-language-lexical--x-语言词法)
13. [X Language: Type System / X 语言：类型系统](#13-x-language-type-system--x-语言类型系统)
14. [X Language: Statements & Expressions / X 语言：语句与表达式](#14-x-language-statements--expressions--x-语言语句与表达式)
15. [X Language: Custom Functions / X 语言：自定义函数](#15-x-language-custom-functions--x-语言自定义函数)
16. [Built-in Function Groups Overview / 内建函数分组总览](#16-built-in-function-groups-overview--内建函数分组总览)
17. [engine Group / engine 组](#17-engine-group--engine-组)
18. [anim Group / anim 组](#18-anim-group--anim-组)
19. [noise Group / noise 组](#19-noise-group--noise-组)
20. [persist Group / persist 组](#20-persist-group--persist-组)
21. [gl_draw Group / gl_draw 组](#21-gl_draw-group--gl_draw-组)
22. [io / math / string / mem Groups / io-math-string-mem 组](#22-io--math--string--mem-groups--iomathstringmem-组)
23. [Unit Conventions & Deprecated Code / 量纲速查与废弃代码约定](#23-unit-conventions--deprecated-code--量纲速查与废弃代码约定)

---

# Part I / 第一部分：项目开发文档 / Development Documentation

## 1. Project Overview / 项目概述

`com.qianlan.cannon` is an **Android native 2D game**. The core idea:

* **Java is only a thin shell**: `MainActivity` creates a `GLSurfaceView` (OpenGL ES 2.0) and forwards the render loop and touch events entirely to C++.
* **A self-built C++ engine** handles: procedurally generated map (chunked world + Perlin noise + biome configuration), a unit system, A\* pathfinding, skeleton / sprite-sheet animation, JSON-driven UI, and a virtual joystick.
* **A custom scripting language + VM ("X language" / XVM)**: gameplay logic (unit behavior, UI callbacks, map correction) is written in `.x` scripts, compiled to bytecode, and executed by a built-in CPU VM. A command-line toolchain `xtool` compiles / runs / debugs scripts standalone.

本项目是一个 **Android 原生 2D 游戏**（包名 `com.qianlan.cannon`，Native 库名 `gamecore`），核心思路是：

* **Java 层只做极薄壳**：`MainActivity` 创建 `GLSurfaceView`（OpenGL ES 2.0），把渲染循环、触摸事件全部转发给 C++。
* **C++ 层自研一套游戏引擎**：程序化地图（分块 chunk + Perlin 噪声 + 生物群系配置）、单位（Unit）系统、A\* 寻路、骨骼 / 序列帧动画、JSON 驱动 UI、虚拟摇杆。
* **自定义脚本语言与虚拟机（X 语言 / XVM）**：游戏逻辑（单位行为、UI 回调、地图修正）以 `.x` 脚本编写，经编译成字节码后由内置 CPU 虚拟机执行；并附带命令行工具链 `xtool` 用于独立编译 / 运行 / 调试脚本。

### 1.1 Scale / 规模概览（`src` 目录）

| Category / 类别 | Count / 数量 | Notes / 说明 |
| --- | --- | --- |
| C++ sources / headers / C++ 源与头文件 | 172 files / 172 个（约 3.8 万行） | Engine core, VM, rendering backend / 引擎核心、虚拟机、渲染后端 |
| Java sources / Java 源文件 | 3 | Thin Android shell / 极薄 Android 壳层 |
| JSON assets / JSON 资源 | 31 | Maps / units / animations / UI / index configs / 地图、单位、动画、UI、索引配置 |
| Image / font assets / 图片与字体资源 | 43 | Tiles, unit sprites, UI, fonts / 瓦片、单位图、UI、字体 |
| X scripts / X 语言脚本 | 5 | Gameplay scripts (`code/*.x`) / 游戏逻辑脚本 |

### 1.2 Repository completeness / 仓库完整性

⚠️ Only the **`src/`** source tree is provided. Top-level Gradle files (`build.gradle`, `settings.gradle`, `gradle/wrapper/`) are **not** included and must be supplied when integrating the project.

⚠️ 当前提供的目录只有 `src`（Android 应用的源码目录），不含顶层的 Gradle 构建脚本（`build.gradle`、`settings.gradle`、`gradle/wrapper` 等），接入工程时需补齐。

---

## 2. Tech Stack & Project Layout / 技术栈与工程形态

| Item / 项 | Content / 内容 |
| --- | --- |
| Platform / 平台 | Android, landscape (`screenOrientation="landscape"`), hardware accelerated / 横屏、硬件加速 |
| Languages / 语言 | Java (shell), C++11 (engine), custom X language (gameplay) / Java（壳层）、C++11（引擎）、自研 X 语言（游戏逻辑） |
| Rendering / 渲染 | OpenGL ES 2.0 (`GLSurfaceView` + EGL + GLESv2); C++ side re-implements an `android.graphics`-like Canvas/Paint/Bitmap/Matrix/Path abstraction / C++ 侧实现类 `android.graphics` 的 Canvas/Paint/Bitmap/Matrix/Path 抽象 |
| Build / 构建 | CMake (`src/cpp/CMakeLists.txt`) → `libgamecore.so`; auto `GLOB_RECURSE` over all `.cpp` / 动态库，自动 GLOB_RECURSE 扫描全部 .cpp |
| Linked libs / 链接库 | `log`, `android`, `jnigraphics`, `EGL`, `GLESv2`, `z` |
| Compile flags / 编译选项 | `-std=c++11 -Wall -fexceptions -frtti`; excludes `native-lib.cpp` and `_backup`/`_old` directories / 剔除 native-lib.cpp 与 _backup/_old 目录 |
| Asset access / 资源加载 | `AAssetManager` reads assets directly / 直接读 assets（不再走 Java 反射） |
| Persistence / 持久化 | Binary file at `/storage/emulated/0/log/l/gamestate.dat` (global save) + per-Region GZIP files / 全局存档 + 分区域（Region）GZIP 文件存单位数据 |

### 2.1 Directory tree / 目录结构

```
agent/src
├── AndroidManifest.xml          # MainActivity (landscape fullscreen) + 3 editor Activity declarations / 应用清单 + 3 个编辑器 Activity 声明
├── assets/                      # All game resources / 全部游戏资源
│   ├── animation/               # Skeleton animation JSON (a.json / b.json / test_unit.json ...)
│   ├── code/                    # X scripts (main.x / updata.x / canvas_ui.x / test0.x ...)
│   ├── fonts/main.ttf           # Game font (stb_truetype) / 游戏字体
│   ├── images/                  # tiles/ ui/ units/ explosion & projectile art / 瓦片、UI、单位、爆炸/弹道图
│   ├── maps/                    # Map/biome configs (mp.json / mp2.json) / 地图/生物群系配置
│   ├── structure/               # Ground structures (village_road.json) / 地表结构
│   ├── ui/gameui/               # In-game UI (health bar / backpack) / 游戏内 UI
│   ├── unitdata/                # Unit definition JSON / 单位定义 JSON
│   └── *.json                   # Index files (see Part II §1) / 索引文件
├── cpp/                         # C++ engine (CMake project "gamecore")
│   ├── Main.cpp/.h              # Game main loop & load flow / 游戏主循环、加载流程
│   ├── DataBus.*                # Global data bus / 全局数据总线
│   ├── DataBusPersistence.*     # Global save/load / 全局存档读写
│   ├── StartGame.cpp            # Starts the game after GL is ready / GL 就绪后启动游戏
│   ├── GameCanvasJNI.cpp        # Java native method implementations / native 方法实现
│   ├── GameGlobal.*             # Global statics (databus/paint/context/ui/main) / 全局静态
│   ├── CmdFunction.*            # Unit command entry / 单位指令函数入口（废弃体系）
│   ├── LambdaCallback.h         # std::function callback wrapper / 回调包装
│   ├── base/                    # Animation, Sprite, Pool, Cache, Emitter, WhileFrameTask, Expression
│   ├── file/AssetsReader.*      # assets reading / assets 资源读取
│   ├── jni/                     # GL backend + JNIBridge(~80KB) + SimpleJSON + stb_image/stb_truetype
│   ├── map/MapDataLoad.*        # Map data loading / 地图数据加载
│   ├── math/                    # Calculator expression eval, Function, QuickSort
│   ├── operation/Controller.*   # Touch control, virtual joysticks, unit selection / 触摸控制、虚拟摇杆、单位选择
│   ├── runtime/                 # BiomeConfigLoader / PerlinNoise / ProceduralChunkGenerator / ChunkManager / ChunkRenderer / ChunkTileLoader / ChunkUnitPersistence / CliffManager / TileArcManager / StructureRes
│   ├── shader/DynamicShadow.*   # Dynamic shadows / 动态阴影
│   ├── skeleton_animation/      # ComponentAnimation / SkeletonLoader / SkeletonAnimationResource
│   ├── units/                   # Units / UnitData / LoadUnits / ReadUnits / AStar / Move / JsonToMap
│   ├── vm/                      # X language VM (see §8) / X 语言虚拟机
│   └── widget/                  # JsonUIParser (JSON UI), LogCat on-screen log
├── java/                        # Java shell (com.qianlan.cannon.jni, 3 classes) / Java 壳层
├── keepRules/rules.keep         # ProGuard keep rules / ProGuard keep 规则
└── res/                         # Android resources / Android 资源
```

---

## 3. High-Level Architecture / 总体架构

```
┌─────────────────────────── Java shell / Java 层 ──────────────────────────┐
│ MainActivity (loadLibrary + assets + startGame)                          │
│   → GameCanvas (GLSurfaceView, ES2.0, continuous render)                 │
│   → JNIData (screen size + Controller_onTouchEvent)                      │
└───────────────────────────────┬──────────────────────────────────────────┘
                                │ JNI
┌───────────────────────────────▼──────────────────────────────────────────┐
│ JNI bridge / JNI 桥                                                       │
│ GameCanvasJNI (surfaceCreated/Changed, update, render, touch)            │
│ StartGame (defer new Main() until GL thread is ready)                    │
│ JNIBridge (~80KB): Canvas/Paint/Bitmap/Matrix/Path/GLFont/tex LRU        │
└───────────────────────────────┬──────────────────────────────────────────┘
                                │ create Main + load_game
┌───────────────────────────────▼──────────────────────────────────────────┐
│ Engine core / 引擎核心 (Main loop: update + renderToCanvas)               │
│ DataBus (global bus: units/frame/offset/pool/UUID/command trees)         │
│ XVM (compile code_index.json → execute bytecode)                         │
│ Controller (virtual joystick) · JsonUIParser (JSON UI)                   │
├───────────────────────────────────────────────────────────────────────────┤
│ World generation / 世界生成 (runtime/)                                    │
│ ChunkTileLoader → BiomeConfigLoader → PerlinNoise →                      │
│ ProceduralChunkGenerator (async) → ChunkManager (3×3, LRU) →             │
│ CliffManager / TileArcManager → ChunkRenderer → ChunkUnitPersistence     │
├───────────────────────────────────────────────────────────────────────────┤
│ Units & animation / 单位与动画                                            │
│ Units/UnitData · LoadUnits/ReadUnits · AStar pathfinding · Move          │
│ Animation (sprite sheet) · Skeleton (ComponentAnimation) · DynamicShadow │
└───────────────────────────────────────────────────────────────────────────┘
```

**Flow in one sentence**: Java shell → JNI → GL thread init → async load of tiles / biomes / unit definitions → create `ChunkManager` → per frame `update` + `renderToCanvas`, while XVM executes `.x` scripts to drive units and UI; chunks are generated / recycled around the player.

**运行流程一句话**：Java 壳 → JNI → GL 线程初始化 → 异步加载瓦片 / 生物群系 / 单位定义 → 创建 ChunkManager → 每帧 update + renderToCanvas，期间以 XVM 执行 `.x` 脚本、更新单位与 UI，Chunk 按玩家位置动态生成 / 回收。

### 3.1 Per-frame pipeline / 每帧流程（`Main::update`）

* First frame: `first_load()` — summon test units, dispatch `start` event, run `XVM::run("main")`. / 首帧先 `first_load()`（召唤测试单位、派发 `start` 事件、执行 `XVM::run("main")`）。
* Every frame: `XVM::run("updata")` → sort units by draw order → swap global events → advance time/frame counters → per-unit `update()` (units holding `load_chunk` drive `ChunkManager::updata` to follow the player) → joystick movement. / 此后每帧：`XVM::run("updata")` → 单位按绘制序排序 → 全局事件交换 → 时间 / 帧计数 → 逐单位 `update()`（持有 `load_chunk` 的单位驱动 `ChunkManager::updata` 跟随玩家）→ 摇杆移动。

### 3.2 Per-frame render order / 每帧渲染顺序（`Main::renderToCanvas`）

1. `ChunkRenderer::render` (terrain + cliff / arc transition decorations) / 地表 + 悬崖 / 弧过渡装饰
2. `LogCat::render` (log overlay) / 日志浮层
3. `XVM::run("canvas_background")` → render each visible unit → `XVM::run("canvas_sky")` / 视口内单位逐个渲染
4. `Controller::render` (joystick, selection box) / 摇杆、选择框
5. `XVM::run("canvas_ui_background")` → `JsonUIParser::draw` (all UI) → `XVM::run("canvas_ui")` / 全部 UI 绘制

---

## 4. Java Shell / Java 壳层

Package `com.qianlan.cannon.jni` — only 3 classes / 包 `com.qianlan.cannon.jni`，仅 3 个类：

| Class / 类 | Responsibility / 职责 |
| --- | --- |
| `MainActivity` | `System.loadLibrary("gamecore")`; `nativeSetAssetManager(getAssets())`; create `GameCanvas`, `nativeStartGame("maps/mp.json", 0)`; lifecycle sync / 注入 assets、创建 GameCanvas、启动游戏、生命周期同步 |
| `GameCanvas` | `GLSurfaceView`, EGL 8/8/8/8, ES 2.0, `RENDERMODE_CONTINUOUSLY`; `onDrawFrame` calls `nativeUpdate` + `nativeRender`; multi-touch forwarded / 渲染循环 + 触摸转发 |
| `JNIData` | Compatibility layer: screen size + `Controller_onTouchEvent`. ⚠️ Keeps historic misspelling `Render_SCREEN_HEIGTH()` / 兼容层：屏幕尺寸 + 触摸入口；保留历史拼写 |

---

## 5. JNI Bridge & GL Backend / JNI 桥接与 GL 渲染后端

* `GameCanvasJNI.cpp` implements 4 native method groups / 实现 4 组 native 方法：`nativeSurfaceCreated`（`AAssetManager_fromJava` → `JNIBridge::glInit` → `startGameAfterGLInit`）、`nativeSurfaceChanged`（`glResize`）、`nativeUpdate`（调 `Main::update`）、`nativeRender`（`glBeginFrame` → 创建 `Canvas` → `renderToCanvas` → `glEndFrame`）。
* `StartGame.cpp`: `JNIBridge_setPendingGame` only records the map path + id; once the GL thread is ready, `new Main()` + `load_game` run — GL context valid before startup / 只记录地图路径与 id，等 GL 线程就绪后再启动，保证 GL 上下文可用。
* `JNIBridge` (~80KB) re-implements **Android graphics API semantics** in C++ with GLES2 / 在 C++ 侧用 GLES2 重新实现 Android 图形 API 语义：
  * Canvas: drawBitmap / drawText / drawRect / drawCircle / drawPath / save / restore / clipRect etc.
  * Bitmap: create / createScaledBitmap / createSubBitmap / getPixel / setPixel (dirty-region upload), GL texture handle, FBO bitmap cache / 脏区显式上传、GL 纹理句柄、FBO 绘制型位图缓存
  * Paint: color, text size, alignment, anti-alias, shadow, shader (LinearGradient)
  * Matrix: `android.graphics.Matrix`-compatible set/post/pre + inverse + mapPoints
  * Fonts: `stb_truetype` renders `main.ttf` (`GLFont.cpp`); Images: `stb_image` decode; LRU content cache (`getCachedBitmap`) / 字体与图片解码、LRU 内容缓存

---

## 6. Game Main Loop / 游戏主循环

### 6.1 `load_game(mapPath, id)`

1. `XVM::init()` — compile & load all X scripts (`code_index.json`). / 编译加载全部 X 脚本
2. `LoadUnits` — load unit definitions (`index.json` → each `unitdata/*.json`). / 加载单位定义
3. First load / 首次加载：`DataBusPersistence::load()` → `AnimationResource/AnimationLoad/SkeletonAnimationResource/StructureRes::init()` → `Controller::initTouchControl` → `ChunkTileLoader` 读 `tile_index.json` → **同步**加载全部瓦片图并缩放为 `CELL_WIDTH × CELL_HEIGHT` → `BiomeConfigLoader` 读 `maps/*.json` → 构建 `ProceduralChunkGenerator` → `ChunkManager` → `AStar` → `ChunkRenderer`，置 `FL=true`。
4. Subsequent loads (map switch) / 非首次加载（换图）：`map_swap` 全局事件 + 重新构建生成器与管理器。

### 6.2 `update()` / `renderToCanvas()`

See §3.1 / §3.2 / 见 §3.1 / §3.2。

---

## 7. Global Data Bus / 全局数据总线

Static global data center / 静态全局数据中心，主要成员：

| Member / 成员 | Purpose / 职责 |
| --- | --- |
| `CHUNK_SIZE=12`、`GRID_COLS/ROWS=3×3` | chunk 边长与 3×3 活跃区 |
| `units` / `frame` / `time` | 单位列表、帧计数、时间 |
| `offsetX/Y`、`startX/Y` | 屏幕偏移与视口原点 |
| `plot_index`、`CELL_WIDTH/HEIGHT` | 地块索引与格子像素尺寸 |
| `find_way (AStar*)` | 全局寻路实例 |
| `tileBitmaps` | tile id → 缩放后的瓦片图 |
| `final_data/map_data/list_data (CommandNode*)` | 单位指令树数据（废弃体系） |
| `pool (Pool)` | 全局对象池 |
| `UUID` | 单位唯一 ID 自增器 |
| `log` | 日志队列 |
| `summon()` | 按单位名在世界坐标召唤单位 |
| Query family / 查找族 | `findUnitAt / findUnitsXY / findUnitByRect / findUnitByCirCle / findUnitBySector` 等 |
| Utility family / 工具族 | 碰撞、距离、矩阵转置、求交 |

---

## 8. X Language VM / X 语言虚拟机

**Design idea / 设计思路**：游戏逻辑不写死在 C++，而是用类 C 语言（X 语言）编写 `.x` 脚本 → 编译成字节码 → 内置 CPU 执行。

### 8.1 Module layout / 模块划分

| Directory / 目录 | Responsibility / 职责 |
| --- | --- |
| `vm/compiler/` | `Lexer` 词法 → `Parser` 语法 → `AST` → `CodeGenerator`（65KB，生成 code/data 段字节码） |
| `vm/assembler/` | `.asm` 汇编文本 → `BinaryWriter` 二进制 |
| `vm/vm/` | `CPU`（256 个 16 位寄存器、`pc/sp/bp/si/di/ds/es`、标志位、255 个操作码槽位）、`SegMemory` 分段内存、`Instructions`（33KB）、`BinaryLoader`、`builtins/*` |
| `vm/runtime/` | `GotoRuntime`（跨模块 goto）、`ModuleLoader` 模块加载 |
| `vm/toolchain/` | `XToolchain`（自动识别 `.x/.asm/.bin` 编译运行）、`ModuleManager`（`--module name=xxx.x` 预加载模块） |
| `vm/use/` | 与引擎的绑定层：`engin.cpp`（单位属性 get/set、寻敌、UI 指令）、`anim.cpp`（骨骼节点操作）、`builtin_noise.cpp`、`builtin_persist.cpp`、`gl_draw.cpp`（绘制指令）、`XVM` 入口 |
| `vm/main/xtool_main.cpp` | 命令行入口（见 §19） |

### 8.2 Compile & run chain / 编译与运行链路

```
code_index.json (path array / 路径数组)
  → XVM::init(): read each .x → Lexer → Parser → CodeGenerator (.asm text)
  → Assembler::assembleText → code.bin + data.bin
  → compiledMap[module name = filename without extension / 模块名 = 文件名去扩展名]
```

* Module name = `.x` filename without extension (`code/test.x` → module `test`) / 模块名 = `.x` 文件名去掉扩展名。
* Each module must define `int main()`; `XVM::run("module")` executes from `main` until `hlt`/`return 0` / 每个模块必须包含 `int main()` 入口。
* Compile failures print `编译出错：...` to LogCat; the failing module is simply not registered (no crash) / 编译失败只打日志，该模块不会被注册，但不中断其他模块。

### 8.3 Script mount points / 脚本挂载点（谁在什么时候 run 谁的脚本）

| Mount point / 挂载点 | When / 触发时机 | VM context / 上下文 (CPUDATA) |
| --- | --- | --- |
| Unit `vm_code` / 单位 | **every frame** / 每帧（`UnitData::update`） | `unit` = 所属 Units；事件表 = 该单位 events |
| UI button `vm_code` / UI 按钮 | click release (ACTION_UP) / 点击抬起 | 无单位上下文（勿调单位函数） |
| Skeleton component `vm_code` / 骨骼组件 | **every frame** / 每帧（`ComponentAnimation`） | `anim` = 当前 SonComponent |
| Noise correction `vm_code` / 噪声修正 | first region sample / 区域首次采样 | `noise` = 该区域 RD |
| `goto 模块(实参)` | script jump / 脚本内跳转 | 废弃路径，勿用于模块间通信 |

### 8.4 Built-in groups summary / 内建函数分组总览

| Group / 组 | Typical functions / 典型函数 | Purpose / 用途 |
| --- | --- | --- |
| `io` | print / print_str / print_long | 输出 |
| `math` | add / sub / mul / max / min / abs / clamp | 运算 |
| `string` | strlen / strcmp / strcpy | 字符串（数据段指针） |
| `mem` | heap_start | 堆起点 |
| `engine` | setRunning / setHp / setSpeed / setAngle / setXY / setWH / move / summon / searchUnit / findUnitByRect/CirCle/Sector / addUi / setUiXY / addEvent / getX / getY / getCanvasX / getRandom … | 单位与游戏控制 |
| `persist` | setVar / getVar / hasVar / delVar / clearVar | 全局持久变量 |
| `anim` | setFatherNodeX … getCanvasX / sin / cos | 骨骼动画节点操作 |
| `noise` | setNoise / getNoise / setWHNoise / getCenterX / getCenterY / setStep | 噪声区域（RD）读写 |
| `gl_draw` | createPaint / loadBitmap / canvasDrawColor/Rect/Circle/Text/Bitmap / canvasSave / canvasRestore / measureText / getScreenWidth/Height … | 直接绘制 |

**Deprecated builtins / 废弃内建**（已登记但未实现，启动时打警告；调用会失败，**勿调用** / registered but unimplemented; startup warning; calls fail — do not call）：`setMainControl`、`addUiCmd`、`clearUiCmd`、`getAllEvents`、`getWalkAbleId`。

**X language script sample / X 语言脚本示例**（`assets/code/test0.x`，坦克炮管角度摆动逻辑 / tank barrel swing logic）：

```
#include <anim>
#include <persist>
#include <engine>

int main() {
    if(getVar("tick")==3){
        setVar("tick",0);
        if (getVar("add2") != -1 && getVar("add2") != 1) {
            setVar("add2", 1);
        }
        if (getAngle() <= 1100 || getAngle() >= 1150) {
            setVar("add2", - getVar("add2"));
        }
        if (getAngle() <= 1100 || getAngle() >= 1150) {
            setAngle(getAngle() + getVar("add2"));
        }
        setVar("bx", (getAngle() - 1100) / 200);
        int cos_bx = cos(getVar("bx"));
        setAngle(getAngle() + (cos_bx+1) * getVar("add2"));
        return 0;
    }
    setVar("tick", getVar("tick")+1);
    return 0;
}
```

**Fixed engine mounts / 引擎内固定挂载点**（`Main` 按名字 `XVM::run(...)` 执行 / executed by name）：`main`（首帧）、`updata`（每帧）、`canvas_background` / `canvas_sky` / `canvas_ui_background` / `canvas_ui`（渲染各层）。索引文件为 `assets/code_index.json`。
---

## 9. Procedural World Generation / 程序化世界生成（`runtime/`）

Core pattern: **config-driven + async frame-sliced generation + chunk caching** / 核心是「配置驱动 + 分帧异步生成 + 分块缓存」：

1. `BiomeConfigLoader`: parses `maps/*.json` into `BiomeConfig` — master noise + sub-biomes (`sub_noises` with thresholds, tile noise, layered tiles `tile_layers`, item spawn rules, unit spawn rules, correction noise `correction_noise_value`, patterns `patterns`) + global patterns + `excessive_waves` + `excessive_cliff`. Full field reference in Part II §7 / 解析 `maps/*.json` 为 `BiomeConfig`：主噪声 + 若干子群系 + 全局图案 + 浪过渡 + 悬崖过渡；完整字段见第二部分 §7。
2. `PerlinNoise`: 256-permutation-table Perlin / fBm noise; wave superposition (sin/cos/random/perlin); `RD` region cache (one cache + one X script per region); correction noise (`CorrectionNoise`) / 256 置换表的基础 Perlin / 分形噪声；支持波叠加；RD 区域缓存（每区域一块缓存 + 一份 X 脚本）；修正噪声。
3. `ProceduralChunkGenerator`: pick sub-biome by master noise → layered tiles (`TileChance` weighted random) → patterns → items → units → boundary tile cache; multi-stage `WhileFrameTask` async (max 3 parallel tasks) / 按主噪声选子群系 → 分层取瓦片 → 应用图案 → 生成物品 → 生成单位 → 边界缓存；分帧异步执行避免卡顿。
4. `ChunkManager`: 3×3=9 chunk active area around the player; chunk cache (`MAX_CACHE_SIZE=50`), LRU eviction; `PRELOAD_RADIUS=2` preload; `updata(x,y)` follows the `load_chunk` unit (player) / 3×3 块活跃区围绕玩家；世界坐标 chunk 缓存（上限 50），LRU 淘汰；预加载半径 2；随玩家移动刷新。
5. `CliffManager` (cliff faces, 10 types) / `TileArcManager` (transition arcs, 12 types + curvature): runtime-generated transition bitmaps, LRU-cached (renderer `MAX_CACHE_SIZE=2048`) / 悬崖面（10 种）/ 过渡弧（12 种 + 曲率），运行时生成并 LRU 缓存。
6. `ChunkRenderer`: draws 3×3 active chunks into the viewport, auto-adding cliff/arc transitions; `buildTileArray` expands neighborhoods / 把 3×3 活跃 chunk 按视口绘制，瓦片间自动补齐悬崖 / 弧过渡装饰。
7. `ChunkUnitPersistence`: saves/loads per-chunk unit state as Region (10×10 chunks) files (GZIP + binary blocks); typed binary objects; periodic dirty flush; two background threads / 按 Region（10×10 chunk）分文件保存 / 加载单位状态；类型化二进制对象；脏区域定期自动 flush；双后台线程。

---

## 10. Unit System / 单位系统（`units/`）

* `UnitData`: unit definition data — `name/hp/maxHp/speed/attackR/radius/angle/w/h/team/hit box/walkAbleTileId/events/animation_name/skeleton_name/vm_code` + command trees. `vm_code` names the bound X script / 单位定义数据；`vm_code` 指定单位绑定的 X 脚本名。
* `Units` (extends `Animation`): `update()` driven by `UnitData.events/add_events`; `data` owned by `unique_ptr`; non-copyable, movable / 按事件驱动行为；data 用 unique_ptr 管理；禁止拷贝、支持移动。
* `LoadUnits / ReadUnits`: read `assets/index.json`, parse `code` (attributes) and `action` blocks; `index.json` is a quoted-path array; 5-thread parallel read; missing files skipped / 读取单位文件列表，解析 `code` 与 `action`；5 线程并行读；文件缺失不中断。
* **Command system / 指令系统**（`CmdFunction.cpp` + `DataBus` `CommandNode`）: ⚠️ **deprecated / 废弃** — the `action` block (`condition/operate`) has **no executor** in the current C++ code. Do not port or extend it; unit behavior goes in X scripts (`vm_code`). / `action` 块（条件-操作）当前无执行器；不要补执行器、不要迁移；单位行为写 X 脚本（`vm_code`）。
* `AStar`: pathfinding on `walk_able_tile_id` using `ChunkManager` tile data; heuristics, neighbor expansion, `optimizePath`, `getLinePoints` / 以瓦片数据为地图按可走瓦片寻路；支持启发式、邻居扩展、路径优化、取直线格点。
* `Move`: unit movement (target point + speed) / 单位移动。
* `JsonToMap`: generic JSON → `std::map` utility / JSON 转 map 工具。

---

## 11. Animation System / 动画系统

| Module / 模块 | Notes / 说明 |
| --- | --- |
| `base/Animation` + `AnimationResource` + `AnimationLoad` | Sprite-sheet animation: `animation/*.json` (`type=big`, `single_frame_tick=5`, `figure_w/h=3`, `event_frame=9`, `animation_img=images/man.png`) — slices frames from one big image; supports frame index switching, horizontal / vertical mirroring. Indexes: `unit_animation_index.json` / `animation_index.json` / 序列帧动画：从整张大图上按 `figure_w×figure_h` 分格取帧，支持帧索引切换、水平 / 垂直镜像 |
| `skeleton_animation/` (`ComponentAnimation` / `SkeletonLoader` / `SkeletonAnimationResource`) | Skeleton animation: `SonComponent` parent/child nodes (joints `father_nodeX/Y`, `son_nodeX/Y`, `trust_nodeX/Y`) + bitmap + angle, per-node matrix; `MainComponent` supports mirroring & data variables. Index: `skeleton_index.json` (`animation/a.json`, `animation/b.json`). Each component may run its own `vm_code` every frame (context `anim`) / 骨骼动画：父子节点 + 位图 + 角度，逐节点矩阵计算并绘制；每个组件可每帧运行自己的 `vm_code`（上下文 `anim`） |
| `shader/DynamicShadow` | Matrix skew + scale + translucent Paint for dynamic projections / 用 Matrix 斜切 + 缩放 + 半透明 Paint 绘制动态投影 |

---

## 12. JSON-Driven UI / JSON 驱动 UI（`widget/JsonUIParser`）

Declarative JSON UI, loaded via `loadFromAssets("menu.json")` or at runtime `addUi("ui/gameui/xxx.json")` / JSON 声明式 UI，`loadFromAssets("menu.json")` 或运行期 `addUi(...)` 加载。

* Control types / 控件类型：`textview` / `image` / `button` / `rectangle` / `grid`.
* Coordinates / 坐标：**relative** (`x/y/width/height` 0–1 ratios) or **fixed pixels** (`fixed_x/fixed_y/fixed_width/fixed_height`); `center_anchor` (center vs top-left) / 相对坐标或固定像素，`center_anchor` 控制锚点。
* Text: color, size, gravity alignment, `maxWidth` auto-wrap, custom font path / 文本颜色、字号、对齐、自动换行、自定义字体。
* Each element may carry `vm_code` (**the only effective click script** — buttons run it on ACTION_UP). The legacy `cmds` array is deprecated and ignored / 元素可挂 `vm_code`（唯一有效的点击脚本）；遗留 `cmds` 数组废弃、被忽略。
* Runtime API / 运行期 API：`setXY / setWH / setId / setVisible / clearUi`（废弃勿调用：`addUiCmd`、`getUiVisible` — 后者无返回值）。

---

## 13. Input Controller / 输入控制（`operation/`）

* Multi-touch / 多指触摸：`JNIData.Controller_onTouchEvent` → `onTouchDown/Move/Up/Cancel`.
* **Move joystick** (outer 120px / inner 50px) and **attack joystick** (100/45px); displacement maps to direction vector / 移动虚拟摇杆（外圈 120px / 内圈 50px）与攻击摇杆（100/45px）。
* Unit selection / 单位选择：`select_item` stores the selected unit and renders a selection box; `updateJoystickMove` applies joystick direction with `ChunkManager` refresh.
* Rendering: joystick base ring / inner ring / direction arrow / attack indicator with pre-initialized Paints.

---

## 14. Persistence / 持久化

* `DataBusPersistence`: global save, binary file at `/storage/emulated/0/log/l/gamestate.dat`; API `save/load/exists/deleteFile/getLastModified/getFileSize`; internal `BinaryWriter/Reader` / 全局存档，二进制写入固定绝对路径；提供 save/load/exists 等 API。
* `ChunkUnitPersistence` (§9): chunk-level unit save, Region files + typed objects + GZIP + auto flush / chunk 级单位存档，区域文件 + 类型化对象 + GZIP + 自动 flush。

---

## 15. Utility Libraries / 基础工具库

| Module / 模块 | Notes / 说明 |
| --- | --- |
| `Pool` | Name-keyed object pool (factory create / recycle / precreate) to reduce GC & allocation / 按名称分类的对象池 |
| `Cache` | Simple caching utility / 简单缓存工具 |
| `Emitter` | Event subscribe/publish (`on/once/off/emit`), deferred release & safe nested emit / 事件订阅 / 发布 |
| `WhileFrameTask` | Frame-sliced task: `totalSteps` + `onExecute/onComplete/onProgress`, `doFrame(maxSteps)` / 分帧任务，把耗时逻辑分摊到多帧 |
| `Sprite` | Sprite drawing / 精灵绘制 |
| `Expression` / `math/Calculator` / `Function` | Expression parsing & evaluation (for `condition` conditions & `[[...]]` value extraction) / 表达式解析与计算 |
| `math/QuickSort` | Sort units by y / draw order / 单位按 y / 绘制序排序 |
| `SimpleJSON` (19KB) | In-house JSON parser (objects/arrays/typed access/debug output) / 自研 JSON 解析器 |
| `file/AssetsReader` | assets stream reading / assets 流读取 |
| `widget/LogCat` | On-screen log overlay (dev debugging) / 屏幕日志浮层 |

---

## 16. Resource & Data Format Reference / 资源与数据格式参考

> 完整字段级参考见第二部分（Part II）。此处为速览 / Full field-level reference is in Part II; quick overview here.

### 16.1 Index files / 索引文件（assets 根目录）

All are **arrays of double-quoted paths**; missing files are skipped, never fatal / 均为双引号包裹路径数组；文件缺失不中断启动。

| File / 文件 | Content / 内容 | Consumer / 消费方 |
| --- | --- | --- |
| `index.json` | Unit definition file list / 单位定义文件列表 | `LoadUnits` |
| `unit_animation_index.json` | Sprite-sheet animation file list / 单位序列帧动画文件列表 | `AnimationResource` |
| `animation_index.json` | Animation resource file list / 动画资源文件列表 | `AnimationLoad` |
| `skeleton_index.json` | Skeleton animation file list / 骨骼动画文件列表 | `SkeletonAnimationResource` |
| `structure_index.json` | Ground structure file list / 地表结构文件列表 | `StructureRes` |
| `code_index.json` | X script file list (compile mounts) / X 脚本文件列表（编译挂载点） | `XVM` |
| `tile_index.json` | Tile config `{id, path, special}` / 瓦片配置 | `ChunkTileLoader` |
| `function/function.json` | Command templates / 指令模板 | `CmdFunction` — 废弃 / deprecated, no consumer |

### 16.2 Map / biome / 地图与生物群系（`maps/*.json`）

Full field table (with defaults) in Part II §7 / 完整字段表（含默认值）见第二部分 §7。要点：`horizontal_plane_index`、`biome.name`、`master_noise`（NoiseConfig）、`sub_noises[]`（阈值 / 瓦片分层 / 物品 / 单位 / 修正噪声 / 图案）、`global_patterns[]`、`excessive_cliff`、`excessive_waves`。

⚠️ 示例 `maps/mp.json` 的 `correction_noise_value.cmds` 是**废弃写法**；新配置只写 `"vm_code": "<模块名>"` 并注册脚本 / the sample's `cmds` is legacy; new configs use `"vm_code"`.

### 16.3 Unit definition / 单位定义（`unitdata/*.json`）

Full field table (with defaults) in Part II §3 / 完整字段表见第二部分 §3。要点：`code` 对象内 `name / maxHp / speed / attack_r / radius / w / h / is_building / hit_box_* / animation_name / skeleton_name / vm_code`；数值字段按字符串解析；`action` 块废弃；`walkable` 固定 `[0]`（既定现状）。

### 16.4 Animation / 动画（`animation/*.json`）

`{ "type": "big", "name": "test", "single_frame_tick": 5, "figure_w": 3, "figure_h": 3, "event_frame": 9, "animation_img": "images/man.png" }` — 序列帧大图按 `figure_w × figure_h` 均分，`single_frame_tick` 控制帧停留，`event_frame` 标记事件帧 / sprite-sheet sliced by figure_w×figure_h.

### 16.5 UI / UI JSON（`assets/ui/**/*.json`）

Root array of controls; common fields: `type`、`id`、`x/y/width/height`（相对）或 `fixed_*`（像素）、`center_anchor`、`visible`、`vm_code`；`cmds` 废弃。控件专属字段见第二部分 §6 / control-specific fields in Part II §6。

### 16.6 X scripts / X 脚本（`assets/code/*.x`）

C-like syntax (`int main()` entry, `return 0`, `#include <group>`); engine mounts by name (`main/updata/canvas_*`) / 类 C 语法，引擎按名字挂载执行。

---

## 17. X Language Quick Reference / X 语言速查

> 完整参考见第二部分（Part II）§11–§22 / Full reference in Part II §11–§22. Summary:

* Lexical / 词法：`//` 注释；`"..."` 字符串（转义 `\n \t \" \\ \' \0`）；`'x'` → ASCII；`0x1F` 十六进制；`true/false` → `1/0`；关键字 `int void if else while for return break continue string import byte char short long struct do phys goto bool`（`import` 已移除，用 `#include <组名>`）；运算符 `+ - * / % == != < > <= >= && || ! & | -> =`。
* Types / 类型：`byte/char`（1 槽）、`short/int`（16 位 1 槽）、`long`（2 槽 32 位）、`bool`、`string`（数据段指针）、`struct`、指针 `ptrLevel`、函数指针。
* Statements / 语句：声明、if-else、while、do-while、for、break、continue、return、`goto 模块(实参)`（废弃，勿用）、表达式语句。
* Pointers / 指针：`int* p = &a; *p = 6; p->field; arr[i]` ≡ `*(arr+i)`；强转 `(int*)0x1000`。
* Calling convention / 调用约定：返回值在 **R0**；`long` 返回 R0 低字 + R1 高字；数组返回 R0=地址、R1=长度。
* **Convention / 约定**：模块间通信只用 `persist`；跨模块 `goto` 是废弃路径，不要用于数据交换或模块切换。

---

## 18. Built-in Function Groups / 内建函数分组

> 完整签名见第二部分（Part II）§16–§22 / Full signatures in Part II §16–§22. Quick summary:

* `engine`：单位数值 / 变换 / 移动 / 事件 / 环境量 / 查询（`searchUnit`、`findUnitByRect/CirCle/Sector`、`summon`）/ UI。`getX/getY` 32 位；`getMoveDx/Dy` ×100；`summon` 按格传参；`setAngle` 度、`getAngle` ×10（推断）。
* `anim`：骨骼节点 get/set（×10 定点）；`setAngle(度×10)`；`sin/cos` 输入输出 ×10。
* `noise`：区域噪声读写（×10）；`setWHNoise` 贴结构；⚠️ `sin/cos` 输入为**圈×10**（与 anim 不同）。
* `persist`：`setVar/getVar/hasVar/delVar/clearVar`，进程内全局共享，缺失 key 返回 0。
* `gl_draw`：Paint/Path/Matrix/Bitmap/Canvas/Shader 均以整数 id 管理；`canvasDraw*` 参数 `(canvas, paint, 几何...)`。
* `io / math / string / mem`：print 系列、add/sub/mul/max/min/abs/clamp、strlen/strcmp/strcpy、heap_start。
* 废弃勿调用：`setMainControl / addUiCmd / clearUiCmd / getAllEvents / getWalkAbleId`、`getUiVisible`（无返回值）。

---

## 19. Build & Run / 构建与运行

### 19.1 Android side / Android 侧

1. 补齐 Gradle 工程（当前只提供了 `src`）：`applicationId = com.qianlan.cannon`，`externalNativeBuild` 指向 `src/cpp/CMakeLists.txt`（产物 `libgamecore.so`）。/ Provide the Gradle project.
2. `MainActivity` 为入口（横屏全屏），`assets/` 随 APK 打包。
3. 运行期权限：`READ/WRITE_EXTERNAL_STORAGE`（存档写 `/storage/emulated/0/log/l/`）。

### 19.2 X toolchain / X 语言工具链（`xtool`）

Standalone CLI (`src/cpp/vm/main/xtool_main.cpp`), debug scripts on desktop without Android / 独立命令行，可脱离 Android 在桌面调试脚本：

```
xtool <输入> [--module name=xxx.x | name=xxx.asm | name=xxx_code.bin,xxx_data.bin ...]
```

* `<file>.x`：编译 + 运行；`<file>.asm`：汇编 + 运行；`<file>_code.bin`：加载二进制运行（自动配对 `_data.bin`）；
* `"<源码文本>"`：字符串源码直接运行；
* 显式模式：`xtool run <file>` / `run-str <源码>` / `run-asm <asm>` / `run-bin <code> <data>`；
* `--module`：预加载跨模块跳转（`goto`）所需模块（废弃路径，见 §21）。

---

## 20. Key Design Notes / 关键设计说明

1. **Logic is scripted / 逻辑脚本化**：单位行为、UI 命令、地图修正都走 X 脚本，改动玩法不需要改 C++ 重新编译，天然支持热迭代与调试（`LogCat` 屏幕日志）。
2. **Async asset loading + frame-sliced generation / 资源异步加载 + 分帧生成**：瓦片加载回调链 + `WhileFrameTask` 多阶段 chunk 生成（并行上限 3 任务），把大世界生成成本分摊到多帧，避免阻塞渲染线程。
3. **Chunked world & caching / 分块世界与缓存**：3×3 chunk 活跃区跟随玩家，LRU 淘汰非活跃 chunk（上限 50），悬崖 / 弧装饰位图运行时生成并 LRU 缓存（上限 2048），单位存档按 Region 分文件、GZIP 压缩、脏区定时 flush。
4. **C++ re-implements Android graphics semantics / C++ 复刻 Android 图形语义**：`JNIBridge` 用 GLES2 实现 Canvas/Paint/Bitmap/Matrix 等 API，历史代码可低改动迁移，且规避 Java 层每帧 JNI 反射开销。
5. **Event-driven / 事件驱动**：`Emitter` 订阅 / 发布贯穿加载流程（如 `allUnitsLoad`）；单位行为由 `events/add_events` 驱动；全局事件每帧在 `Main::update` 中交换结算。

---

## 21. Current Status & Conventions / 当前状态与开发约定

### 21.1 General status / 一般状态

* **Incomplete project / 工程不完整**：仅提供 `src` 源码树，缺少 Gradle 工程文件与 `src/java` 之外的编辑器代码。
* **Manifest references missing sources / 清单引用但源码缺失**：`AndroidManifest.xml` 声明了 `MapEditorActivity`、`SkeletonEditorActivity`、`ImageZoomActivity`，但当前 `src/java` 下只有 `jni` 包，需补充。
* `JNIData` **spelling compatibility / 拼写兼容**：`Render_SCREEN_HEIGTH()` 保留历史错误拼写。
* **Dev/sample content / 示例测试性质内容**：`menu.json`、`test_unit.json`、`first_load()` 中的测试单位召唤、"测试" 单位及 `test0.x` 炮管摆动脚本等属于开发期调试内容；正式玩法需替换。
* **Absolute save path / 存档路径为绝对路径**：`DataBusPersistence` 写死 `/storage/emulated/0/log/l/gamestate.dat`，多用户 / 多地图场景建议改为按应用私有目录与 mapId 分文件。

### 21.2 Deprecated / legacy code / 废弃历史代码（勿动、勿推理）

The items below are **deprecated legacy code / legacy conventions**, not gaps or TODO features. Do not build executors, do not migrate `cmds`, do not align samples — ignore them / 以下内容属于历史遗留的废弃代码 / 废弃写法，**不是缺口、不是待修功能**；看到就当没看见，不补、不改、不对齐：

1. **`action` blocks / `action` 块** — the `condition/operate` command system has no executor; it is legacy. Unit behavior lives in X scripts (`vm_code`) / 无执行器，属废弃；行为逻辑一律写 X 脚本。
2. **`cmds` arrays / `cmds` 数组** — deprecated everywhere; buttons execute `vm_code` only; menu buttons without `vm_code` do nothing on click / 废弃写法，无任何效果；按钮点击只走 `vm_code`。
3. **`cmds` in map correction / 地图 correction 里的 `cmds`** — `maps/mp.json`'s `correction_noise_value.cmds` is a legacy write; new configs use `"vm_code"` + register the script / 废弃写法；新配置只写 `"vm_code"` 并注册脚本。
4. **`walkable` fixed to `[0]` / `walkable` 硬编码 `[0]`** — current state by design, not a bug; no JSON field, no configuration needed / 既定现状，不是 bug，无需 JSON 配置。
5. **Cross-module `goto` / 跨模块 `goto`** — not used for module communication; inter-module data uses `persist` only / 不用于模块间通信；模块间数据交换只用 `persist`。
6. **Unimplemented builtins / 未实现内建** — `setMainControl / addUiCmd / clearUiCmd / getAllEvents / getWalkAbleId` and `getUiVisible` (no return value) — do not call / 都不要调用。

### 21.3 Development notes / 开发注意事项

1. **Strings are data-segment pointers / 字符串是数据段指针** — compare with `strcmp`, print with `print_str`, copy with `strcpy`; never treat a string variable as inline text / 要用 strcmp 比较、print_str 打印、strcpy 复制。
2. **Unit scripts run every frame / 单位脚本每帧执行** — no heavy loops; use `getVar/setVar` for cross-frame state / 别在脚本里做耗时循环。
3. `import` **is removed / 已移除** — use `#include <group>` / 写 import 直接报错。
4. **Module name = filename stem / module 名 = 文件名去扩展名** — `vm_code` / `correction.vm_code` / UI `vm_code` reference the stem; renaming a file requires syncing `code_index.json` and JSON references / 改文件名记得同步 code_index.json 与 JSON 引用。
5. **Compile failures are silent / 编译失败静默跳过** — a broken `.x` just logs and skips; `XVM::run(missing)` returns immediately without error.
6. **Multiple mount points run scripts per frame / 挂载点多** — don't assume exclusive VM ownership; `CPUDATA::unit/anim/noise` are set by the caller before `run`.

### 21.4 Unit-convention table / 量纲速查

| Value / 数值 | Convention / 量纲 | Source / 来源 |
| --- | --- | --- |
| `anim` coords / w/h | ×10 fixed-point / ×10 定点 | `anim.cpp` |
| `anim` angle | degrees×10 / 度×10 | fmod 360 |
| `anim` sin/cos | input degrees×10, output ×10 / 输入度×10、输出×10 | `v*2π/360` |
| `noise` values / 噪声值 | ×10 fixed-point / ×10 定点 | `getNoise/setNoise` |
| `noise` sin/cos | **turns×10** input (v/10 = turn), output ×10 / 输入圈×10、输出×10（与 anim 不一致） | `v*2π` |
| `engine getAngle()` | returns `data->angle`; inferred degrees×10 (test0.x uses 1100≈110°) / 推断量纲 ×10 度 | `engin.cpp` |
| `engine setAngle()` | direct **degrees** / 直接度 | `Animation::setRotation(angle)` |
| `getMoveDx/Dy` | ×100 (-100..100) | `move->dx*100` |
| `getX/getY` | 32-bit (R0 low / R1 high) / 32 位 | `returnL` |
| `summon` | x/y are **grid cells**, internally ×CELL / 按格传参，内部 ×CELL | `AL*CELL_WIDTH` |
| `getRandom` | 0..65535 | all groups / 各实现一致 |
---

# Part II / 第二部分：JSON 与 X 语言参考 / JSON & X Language Reference

> 本部分面向“依赖 JSON 配置 + X 脚本做后续开发”的场景；字段、默认值、函数签名均以当前代码实现为准 / This part targets development that depends on JSON configs + X scripts; all fields, defaults and signatures follow the current code.

## 1. Assets Index System / Assets 索引体系

All index JSON files sit at the **assets root** and contain an **array of double-quoted paths** (`["path1","path2",...]`). Loaders extract the `"..."` entries and load each file; **missing files do not abort startup** (they log and skip) / 所有索引 JSON 都位于 **assets 根目录**，内容是一个**双引号包裹的路径字符串数组**。加载器通过正则式抓取所有 `"..."` 得到文件列表，再按路径加载；**文件缺失不会中断启动**。

| Index file / 索引文件 | Loader / 加载器 | Referenced dir / 被引用目录 | Purpose / 用途 |
| --- | --- | --- | --- |
| `index.json` | `ReadUnits` / `LoadUnits` | `unitdata/` | 单位定义列表，5 线程并行读取 |
| `code_index.json` | `XVM::init` | `code/` | X 脚本列表，启动时逐个编译 |
| `unit_animation_index.json` | `AnimationResource` | `animation/` | 单位图集动画（big/single/帧序列） |
| `animation_index.json` | `AnimationLoad` | `animation/` | 帧动画（width/height/animation_name 等） |
| `skeleton_index.json` | `SkeletonLoader` | `animation/` | 骨骼动画（递归组件树） |
| `structure_index.json` | `StructureRes` | `structure/` | 结构贴图（供 `setWHNoise` 使用） |
| `tile_index.json` | `ChunkTileLoader` | `images/tiles/` | 图块 id → 贴图 |
| `function/function.json` | `CmdFunction` | —— | 指令模板（废弃 / deprecated） |

Sample / 示例（real files / 真实文件）：

```json
["unitdata/test_unit.json","unitdata/ba.json","unitdata/bullet.json","unitdata/tree.json","unitdata/cursor.json","unitdata/house.json"]
["code/updata.x","code/test.x","code/test0.x","code/main.x","code/canvas_ui.x"]
```

Paths are relative to the **assets root** (e.g. `unitdata/test_unit.json`, `images/man.png`). Add new entries to the array when adding units / scripts / animations / 路径均相对 assets 根；新加单位 / 脚本 / 动画时，把路径追加进对应索引数组即可。

---

## 2. Common JSON Conventions / 通用 JSON 约定

* The project has **multiple independent JSON parsers** (`SimpleJSON`, `BiomeConfigLoader`'s built-in parser, `SkeletonLoader`'s built-in parser, `StructureRes`'s hand-rolled parser, etc.). Behavior differs in details, but all support standard JSON: objects, arrays, strings (with `\n \r \t \b \f \\ \/ \" \uXXXX` escapes and UTF-16 surrogate pairs), numbers, `true/false/null` / 项目内有多个独立 JSON 解析器，细节行为略有差异，但都支持标准 JSON。
* **Unknown fields are ignored**; missing fields use defaults (see each section's field table) / **未知字段一律被忽略**；缺字段用默认值。
* Boolean fields in some parsers accept strings `"true"/"1"` (unit `is_building` is such a case) / 布尔字段在部分解析器里支持字符串形式。
* Numeric fields in some parsers accept **string-typed numbers** (e.g. `"maxHp":"200"`); parse failure falls back to the default / 数值字段在部分解析器里支持字符串形式的数字。
* **Field-name convention / 字段命名约定**：UI / skeleton / unit use `vm_code` (points to an X module name); leftover `cmds` arrays in sample JSON are **deprecated legacy code** with no consumer — ignore them / UI / 骨骼 / 单位 用 `vm_code`；示例 JSON 里残留的 `cmds` 数组是废弃历史代码，没有任何加载器消费。

---

## 3. Unit Definition JSON / 单位定义 JSON

Files: `unitdata/*.json`, listed by `index.json`. Parser: `units/ldata/LoadUnits.cpp` (reads the `code` object) / 文件：`unitdata/*.json`，由 `index.json` 列出；解析器读取 `code` 对象。

### 3.1 `code` field table / `code` 字段表

| Field / 字段 | Type (parse mode) / 类型（实际解析方式） | Default / 默认值 | Notes / 说明 |
| --- | --- | --- | --- |
| `name` | string | "" | 单位名，用作 `singleUnitsData` 的 key（召唤、存档都用它） |
| `maxHp` | **string→float / 按字符串解析浮点** | `1.0` | 初始 hp = maxHp |
| `speed` | string→float | `0.0` | 移动速度 |
| `attack_r` | string→float | `0.0` | 攻击半径 |
| `radius` | string→float→int | `0` | 碰撞半径 |
| `w` / `h` | int | `0` | 宽高（像素）；若 `is_building` 为真则再 ×CELL |
| `is_building` | string `"true"`/`"1"` → true | `false` | 为真时 `w = w*CELL_WIDTH`、`h = h*CELL_HEIGHT` |
| `hit_box_left_top_x` / `_y` | int | `0` / `0` | 命中框左上角 |
| `hit_box_right_bootom_x` / `_y` | int | `w` / `h` | 命中框右下角（注意拼写 `bootom`） |
| `animation_name` | string | "" | 图集动画名（对应 §4 的 `name`） |
| `skeleton_name` | string | "" | 骨骼动画名（对应 §5 的 `main.name`） |
| `vm_code` | string | "" | **X 模块名（`.x` 文件名去扩展名）**，每帧运行 |
| `actionbar` | string | "" | 废弃历史代码：仅存为文本，未参与逻辑 |
| （无 `walkable` 字段解析） | —— | `[0]` | 既定现状：可走瓦片固定为 `{0}`，不是缺口，勿视为 bug |

Key points / 要点：
* Numeric fields parse via `optString` → `safe_stof/safe_stoi`, so both `200` and `"200"` work; malformed/missing values fall back to defaults, never crash / 数值字段的解析方式是字符串解析，所以 JSON 里写 `200` 或 `"200"` 都可以，写错或缺失不会崩溃。
* Unit scripts run **every frame**: `UnitData::update()` → `XVM::run(vm_code)`. Inside the script use `getThis()` for own id, `eventContain("事件名")` for events, `setThis(id)` to switch, `getX()/getY()` for coordinates / 单位每帧执行 `vm_code` 脚本。
* The save system (`ChunkUnitPersistence`) persists `vm_code` and `load_chunk` — the script name is a save field / 存档会持久化 `vm_code` 与 `load_chunk`。

Real sample / 真实示例（`unitdata/test_unit.json`）：

```json
{
  "code": {
    "name": "测试", "maxHp": 200,
    "actionbar": "移除单位:remove;扣血:kx|加血:ax",
    "animation_name": "test", "speed": 16, "attack_r": 10, "radius": 25,
    "w": 100, "h": 200, "skeleton_name": "test", "vm_code": "test"
  },
  "action": []
}
```

### 3.2 `action` block / `action` 块（废弃历史代码，勿动 / deprecated）

`unitdata/test_unit.json` contains an `action` array with `{name, condition, operate[]}` "condition → operation" descriptions (e.g. `"condition":"animation_name == attack and animation_index >= 11"`, `operate` with `setAnimation / findUnitBySector / ifRun / unitRun / str_append / addUi / [hp]` value syntax). **This is deprecated legacy code**: the current C++ code (`LoadUnits::singleUnitLoad`) parses only `code`; the `action` block has **no executor** (`CommandNode`/`UnitDataCommand` are only forward-declared). Convention: unit behavior goes in X scripts (`vm_code`); do not build an executor, do not migrate `cmds` / `action` 数组是废弃历史代码：当前 C++ 代码只解析 `code`，`action` 块没有任何执行器。约定：单位行为一律写在 X 脚本里；不要补执行器、不要迁移 `cmds`。

---

## 4. Animation JSON / 动画 JSON

Files: `animation/*.json`, listed by `unit_animation_index.json`. Parser: `base/AnimationResource.cpp` (branches on `type`) / 文件：`animation/*.json`；解析器按 `type` 分支。

| Field / 字段 | Default / 默认值 | Notes / 说明 |
| --- | --- | --- |
| `type` | `"big"` | `big`：整图切帧；`single`：单帧；其他：按编号序列取多张图 |
| `name` | `"null"` | 动画名（被单位 JSON 的 `animation_name` 引用） |
| `single_frame_tick` | `1` | 每帧停留的 tick 数（帧频控制） |
| `figure_w` / `figure_h` | `1` / `1` | big 类型：图集横向 / 纵向帧数 |
| `start_event_frame` | `0` | big 类型：起始帧序号 |
| `event_frame` | `figure_w*figure_h`（big）/ `1`（其他） | 结束帧序号（big：`[start_event_frame, event_frame)` 切出帧） |
| `animation_img` | "" | 贴图路径（big：图集；single：单图；其他：`路径前缀 + 序号 + 扩展名`） |

Real sample / 真实示例（`animation/test_unit.json`）：

```json
{ "type": "big", "name": "test", "single_frame_tick": 5,
  "figure_w": 3, "figure_h": 3, "event_frame": 9,
  "animation_img": "images/man.png" }
```

Meaning: `images/man.png` is a 3×3 sheet; 9 frames are sliced from index 0 to 8 / 含义：`images/man.png` 是 3×3 图集，从第 0 到第 8 帧共切出 9 帧动画。

There is also `animation_index.json` (`AnimationLoad`) with fields `name/width/height/animation_name/skeleton_name/loop` — a separate scheme; for units use the `unit_animation_index.json` scheme above / 另有一套 `animation_index.json`（`AnimationLoad`）字段体系；日常做单位动画以 `unit_animation_index.json` 那套为准。

---

## 5. Skeleton Animation JSON / 骨骼动画 JSON

Files: `animation/a.json`、`animation/b.json`, listed by `skeleton_index.json`. Parser: `skeleton_animation/SkeletonLoader.cpp`. Structure: root has `main` (MainComponent) with a recursive `children` array of SonComponents / 根对象含 `main` 组件，`main` 下有 `children` 数组；每个子组件递归可再带 `children`。

| Field (Main/SonComponent) / 字段 | Type / 类型 | Notes / 说明 |
| --- | --- | --- |
| `name` (main only) | string | 骨骼名（被单位 JSON 的 `skeleton_name` 引用） |
| `fatherNode` / `sonNode` | object `{x,y}` | 父节点 / 子节点锚点坐标 |
| `width` / `height` | int | 组件尺寸 |
| `image` | string | 贴图路径 |
| `id` | int | 组件 id（脚本里 `getId()` 可读） |
| `angle` | int | 初始角度（度） |
| `visible` | bool | 是否可见 |
| `vm_code` | string | **X 模块名，每帧运行**（作用于本组件的 `anim` 上下文） |
| `cmds` | array | 废弃历史代码：解析器只读 `vm_code`，`cmds` 被忽略 |

Real sample / 真实示例（`animation/a.json`，节选 / excerpt）：

```json
{
  "main": {
    "name": "test", "width": 100, "height": 200, "image": "", "id": 0,
    "angle": 0, "visible": true,
    "children": [
      {
        "fatherNode": { "x": 50, "y": 100 }, "sonNode": { "x": 0, "y": 0 },
        "width": 1, "height": 50, "image": "", "id": 2, "angle": 0,
        "visible": true, "vm_code": "",
        "children": [
          {
            "fatherNode": { "x": 0, "y": 165 }, "sonNode": { "x": 0, "y": 200 },
            "width": 8, "height": 150, "image": "images/tick.png",
            "id": 1, "angle": 111, "vm_code": "test0", "children": []
          }
        ]
      }
    ]
  }
}
```

**Runtime semantics / 运行语义**：`ComponentAnimation.cpp` runs `XVM::run(vm_code)` on the component every frame; `anim` builtins (`setSonNodeX/Y`, `setAngle`, `setW/H`...) act on the currently-playing SonComponent (`CPUDATA::anim`) / 每帧对该组件执行 `vm_code`，脚本内 `anim` 组函数作用于"当前正在播放的 SonComponent"。

---

## 6. UI JSON / UI JSON

Files: `menu.json`、`ui/gameui/*.json`. Parser: `widget/JsonUIParser.cpp`. **The root must be an array**, each element is a control object / 根必须是数组，每个元素是一个控件对象。

### 6.1 Common control fields / 控件通用字段

| Field / 字段 | Default / 默认值 | Notes / 说明 |
| --- | --- | --- |
| `type` | "" | `textview` / `image` / `button` / `rectangle` / `grid`（大小写不敏感） |
| `id` | "" | 控件 id；`button` 会登记到按钮表 |
| `x` / `y` | `0.5` | 相对坐标（屏幕宽高的比例，0–1） |
| `width` / `height` | `0.3` / `0.1` | 相对尺寸（比例） |
| `fixed_x` / `fixed_y` | 无 | 绝对像素坐标；**存在即覆盖对应相对值** |
| `fixed_width` / `fixed_height` | 无 | 绝对像素尺寸；存在即覆盖相对尺寸 |
| `center_anchor` | `false` | true 时以元素中心为锚点 |
| `visible` | `true` | 初始可见性 |
| `vm_code` | "" | 唯一有效的"点击脚本"：**按钮在点击抬起（ACTION_UP）时** `XVM::run(vm_code)` |
| `cmds` | array | 废弃历史代码：当前解析器忽略；只写 `cmds` 不写 `vm_code` 的按钮点击不会有行为 |

### 6.2 Type-specific fields / 各类型专属字段

**textview**：`text("")`、`textColor(#FFFFFF)`、`textSize(40)`、`gravity(center)`、`font("")`、`maxWidth(1)`。
**button**：`text`、`normalColor(#6495ED)`、`pressedColor(#4682B4)`、`textColor(#FFFFFF)`、`textSize(60)`、`backgroundImage`。
**image**：`image`（空或不存在→不显示，不崩溃）。
**rectangle**：`fillColor(#808080)`、`strokeColor(#000000)`、`strokeWidth(0)`、`cornerRadius(0)`。
**grid**：`rows/columns(4/4)`、`cellSpacing(5)`、`cellBgColor(#333333)`、`cellBorderColor(#666666)`、`cellBorderWidth(1)`、`cornerRadius(10)`、`gridBgColor(#CC000000)`、`listName`、`pressedCellBgColor(#88CCFF)`。

Real sample / 真实示例（`ui/gameui/health_bar.json`）：

```json
[
  { "type": "image", "id": "血量背景", "x": 0.5, "y": 0.5,
    "fixed_width": 100, "fixed_height": 15, "image": "images/ui/g.png", "cmds": [] },
  { "type": "rectangle", "id": "血量", "x": 0.5, "y": 0.5,
    "fixed_width": 95, "fixed_height": 7, "fillColor": "#FF0000",
    "strokeColor": "#000000", "strokeWidth": 0, "cornerRadius": 0,
    "visible": true, "cmds": [] }
]
```

**UI ↔ X script linking / UI 与 X 脚本联动**：`addUi("ui/gameui/xxx.json")`、`setUiXY(id, x, y)`、`setUiWH(id, w, h)`、`setUiId(old, new)`、`setVisible(id, bool)`、`removeUi(id)`、`clearUi()` — call these from X scripts (unit / button `vm_code`).

---

## 7. Map / Biome JSON / 地图与生物群系 JSON

Files: `maps/mp.json`、`maps/mp2.json`. Parser: `runtime/BiomeConfigLoader.cpp` (its own strict parser — wrong types throw and abort this load) / 独立 JSON 解析器，字段严格：写错类型会抛异常并中断本次加载。

### 7.1 Root fields / 根字段

| Field / 字段 | Type / 类型 | Default / 默认值 | Notes / 说明 |
| --- | --- | --- | --- |
| `horizontal_plane_index` | int | `0` | 水平面图块 id（水面 / 基面） |
| `images` | array | 无 | `[{id, path, special}]` 图块 id → 贴图 |
| `biome` | object | 无 | 生物群系配置 |

### 7.2 `biome` object / `biome` 对象

| Field / 字段 | Notes / 说明 |
| --- | --- |
| `name` | 群系名，默认 `"default"` |
| `master_noise` | `NoiseConfig`（见 7.3） |
| `sub_noises` | 数组，`SubNoiseConfig`（见 7.4） |
| `global_patterns` | 数组，`PatternConfig`（见 7.6），全局生效 |
| `excessive_cliff` | 数组，`{low:[id...], high:[id...], cliff_face_id}`，默认 `cliff_face_id=0` |
| `excessive_waves` | 数组，`{center_id:[...], check_id:[...]}` |

### 7.3 `NoiseConfig`（`master_noise` / `tile_noise` / `item_noise` / `perlin` 波共用）

| Field / 字段 | Default / 默认值 | Notes / 说明 |
| --- | --- | --- |
| `seed` | 当前时间戳（毫秒） | 噪声种子 |
| `scale` | `0.01` | 频率缩放 |
| `octaves` | `4` | 分形层数 |
| `persistence` | `0.5` | 分形持续度 |
| `lacunarity` | `2.0` | 分形间隙 |
| `superposition` | 无 | 叠加配置：`{enabled(默认true), includeBaseNoise(默认true), waves:[{type, amplitude, frequency, phase, perlin:{NoiseConfig}}]}` |

### 7.4 `SubNoiseConfig`（`sub_noises[]` 元素）

| Field / 字段 | Type/Default / 类型与默认值 | Notes / 说明 |
| --- | --- | --- |
| `id` | int **必填 / required** | 子群系 id |
| `name` | "" | 名称 |
| `threshold` | `{min:0, max:1}` | 主噪声落入该区间则本子群系生效 |
| `priority` | `0` | 优先级 |
| `tile_noise` / `item_noise` | NoiseConfig | 图块 / 物品噪声 |
| `tile_layers` | array | 见 7.5 |
| `items` | array | `{id, spawn_chance(默认0.1), height_range{min,max}, layer(0), avoid_tiles[], require_tiles[]}` |
| `units` | array | `{id, name, spawn_chance(默认0.05), team(0), allowed_tiles[], avoided_tiles[]}` |
| `correction_noise_value` | array | 见 7.7 |
| `patterns` | array | `PatternConfig`，本子群系局部生效 |

### 7.5 `tile_layers[]` element / `tile_layers[]` 元素

| Field / 字段 | Notes / 说明 |
| --- | --- |
| `height` | `{min, max}`（默认 0..1），噪声值落在此区间时使用本层图块 |
| `tileIds` | 数组 `[{id, chance(默认1.0)}]`，按 chance 加权随机 |
| `tileId` | 单个图块 id（与 `tileIds` 二选一，`tileIds` 优先） |

### 7.6 `PatternConfig`（`patterns[]` / `global_patterns[]` 元素）

| Field / 字段 | Default / 默认值 | Notes / 说明 |
| --- | --- | --- |
| `name` | "" | 图案名 |
| `mask` | **必填 / required** | `{rows, cols, data:[[...]]}`，rows×cols 的二维矩阵 |
| `centerRow` / `centerCol` | `rows/2` / `cols/2` | 图案中心 |
| `replaceId` | `0` | 替换图块 id |

`mask.data` cells support four forms / `mask.data` 单元格支持四种写法：number `2` / 数字；string `"2"` / 字符串；object `{"id":2}` or `{"value":2}` / 对象；object `{"any":[0,1], "except":[4]}`（match any ∈any and ∉except）。

### 7.7 `correction_noise_value[]` element / 元素（地形修正）

| Field / 字段 | Notes / 说明 |
| --- | --- |
| `scope` | 区域划分粒度（RandomFuncCreate 的 scope） |
| `interval` | 区域间隔 |
| `height` | `{min, max}` 触发高度区间 |
| `vm_code` | **X 模块名**：首次访问某区域时，把该区域 RD 设为当前噪声上下文并执行此脚本（见 Part II §19） |

**Runtime semantics / 运行语义**（`PerlinNoise::mutation`）：按 `scope/interval` 把世界切成矩形区域；某坐标首次落入区域且主噪声落在 `height` 区间时，创建区域缓存 `RD`，置 `CPUDATA::noise = rd`，然后 `XVM::run(vm_code)`。脚本内用 noise 组函数写入该区域缓存，之后该区域的采样优先返回缓存值。

**Deprecated in samples / 示例里的 `cmds` 是废弃历史代码**：`maps/mp.json` 的 `correction_noise_value` 写的是 `"cmds": ["r0 = 3", "run code/b.mic", ...]` 数组，而加载器只读 `"vm_code"` 字符串。无需迁移示例；新配置一律写 `"vm_code": "脚本模块名"`（且该脚本已在 `code_index.json` 中注册）/ the loader reads **only** `"vm_code"`; new configs write `"vm_code"` and register the script.

Real sample / 真实示例（`maps/mp.json`，节选 / excerpt）：

```json
{
  "horizontal_plane_index": 2,
  "biome": {
    "name": "default",
    "master_noise": { "seed": 12345, "scale": 0.001, "octaves": 1, "persistence": 1, "lacunarity": 2.0 },
    "sub_noises": [
      {
        "id": 0, "name": "平原", "priority": 0,
        "threshold": { "min": 0.4, "max": 0.6 },
        "tile_noise": { "seed": 55555, "scale": 0.05, "octaves": 4, "persistence": 0.5, "lacunarity": 2.0 },
        "tile_layers": [
          { "height": { "min": 0, "max": 0.6 }, "tileIds": [ { "id": 0, "chance": 0.5 }, { "id": 1, "chance": 0.5 } ] },
          { "height": { "min": 0.60, "max": 1 }, "tileId": 2 }
        ],
        "units": [ { "id": 1001, "name": "树", "spawn_chance": 0.01, "team": 2, "allowed_tiles": [0,1], "avoided_tiles": [] } ]
      }
    ],
    "excessive_cliff": [ { "low": [2], "high": [0,1], "high_type": 0, "cliff_face_id": 4 } ],
    "excessive_waves": [ { "center_id": [0,1], "check_id": [4] } ]
  }
}
```

---

## 8. Tile Index JSON / 图块索引 JSON

File: `tile_index.json`. Parser: `runtime/ChunkTileLoader.cpp`. **Supports three root forms / 支持三种根形态**：array `[{id,path,special}]`；object with `images` array；object keyed by id `{"0":{...}}`. Element fields: `id`（整数）、`path`（贴图路径）、`special`（整数，默认 0；标记特殊图块，如水面）。

Real sample / 真实示例：

```json
[
  {"id": 0, "path": "images/tiles/grass.png", "special": 0},
  {"id": 1, "path": "images/tiles/grass1.png", "special": 0},
  {"id": 2, "path": "images/tiles/water.png", "special": 1},
  {"id": 3, "path": "images/tiles/sand.png", "special": 0},
  {"id": 4, "path": "images/tiles/dirt.png", "special": 0}
]
```

Tile ids are the reference base for `tileId/tileIds`、`allowed_tiles/avoided_tiles` and pattern masks in map JSON / 图块 id 是地图 JSON 中各类图块引用的基准。

---

## 9. Structure JSON / 结构贴图 JSON

Files: `structure/*.json`, listed by `structure_index.json`. Parser: `runtime/StructureRes.cpp` (hand-rolled; key order irrelevant) / 手写解析，key 顺序无关。

| Field / 字段 | Notes / 说明 |
| --- | --- |
| `name` | 结构名（`setWHNoise` 按它查找） |
| `w` / `h` | 宽 / 高（格数） |
| `data` | 浮点数组，长度 `w*h`，噪声值数据块 |

Real sample / 真实示例（`structure/village_road.json`）：

```json
{ "name": "village_road", "w": 20, "h": 2, "data": [1.1, 1.1, ...] }
```

Usage / 用途：noise 脚本里 `setWHNoise("village_road", x, y)` 把该 w×h 数据块贴到世界 (x,y)，覆盖区域噪声采样。

---

## 10. Command Templates / 指令模板 JSON

File: `function/function.json` (default path hardcoded in `CmdFunction::DEFAULT_PATH`). Parser: `CmdFunction.cpp`. Format: **object** `{"组名": ["模板命令"]}`, placeholders `{0} {1} {2}` / 格式为对象，模板命令用 `{0} {1} {2}` 占位。

Real sample / 真实示例：

```json
{ "fff": ["summon {0} {1}_{2}_{3}"] }
```

Deprecated / 废弃：loaded into `GameGlobal::function` but **no consumer exists** in the current source — do not depend on it, do not add a consumer / 加载后存入 `GameGlobal::function`，当前源码中无任何消费点——**废弃历史代码**，不要依赖它做运行时逻辑，也不要补消费实现。
---

## 11. X Language: File Organization & Run Chain / X 语言：文件组织与运行链路

### 11.1 Compile chain / 编译链路

```
code_index.json（路径数组）
   → XVM::init() 逐个读取 .x 源文本
   → XCompiler::compileSource（词法→语法→AST→汇编文本 .asm）
   → Assembler::assembleText（汇编文本 → code.bin + data.bin）
   → 存入 XVM::compiledMap[文件名去扩展名]（即"模块名"）
```

* Module name = **`.x` filename without extension** (`code/test.x` → module `test`) / 模块名 = `.x` 文件名去掉扩展名。
* Each module must define `int main()`; `XVM::run("模块名")` executes from `main` until `hlt`/`return 0` / 每个模块必须包含 `int main()` 入口。
* Compile failures print `编译出错：...` to LogCat; the failing module is not registered, others continue / 编译失败只打日志，该模块不会被注册，但不中断其他模块。

### 11.2 Script mount points / 脚本挂载点

| Mount point / 挂载点 | When / 触发时机 | Context / 上下文（CPUDATA） |
| --- | --- | --- |
| Unit `vm_code` / 单位 | **every frame** / 每帧（`UnitData::update`） | `unit` = 所属 Units；事件表 = 该单位 events |
| UI button `vm_code` / UI 按钮 | click release (ACTION_UP) / 点击抬起 | 无单位上下文（勿调单位函数） |
| Skeleton component `vm_code` / 骨骼组件 | **every frame** / 每帧（`ComponentAnimation`） | `anim` = 当前 SonComponent |
| Noise correction `vm_code` / 噪声修正 | region first sampled / 区域首次被采样 | `noise` = 该区域 RD |
| `goto 模块(实参)` | script jump / 脚本内跳转 | 废弃路径，勿用于模块间通信 |

### 11.3 Cross-module calls / 跨模块调用与参数

* `goto 模块名(实参...)`：把当前模块挂起，用 R0..R15（最多 16 个实参）传给目标模块并执行其 `main`；目标 `return` 后恢复挂起模块继续执行。
* **Convention / 跨模块通信约定**：模块间通信**只用** `persist` 全局变量；跨模块 `goto` 传参 / 切模块在当前实现中不可靠，属废弃路径，**不要使用** / module-to-module communication uses `persist` only; cross-module `goto` is a deprecated path.

---

## 12. X Language: Lexical / X 语言：词法

Parser: `vm/compiler/Lexer.cpp`.

### 12.1 Comments / 注释

`// 行注释`（到行尾）。

### 12.2 Literals / 字面量

| Form / 形式 | Notes / 说明 |
| --- | --- |
| `"..."` string / 字符串 | Escapes / 转义：`\n` `\t` `\"` `\\` `\'` `\0`（另有 `\r` 等） |
| `'x'` char / 字符 | 折成 **ASCII/Unicode 码值**（数字 token） |
| `0x1F` hex / 十六进制 | 数字字面量 |
| `123` decimal / 十进制 | 数字字面量 |
| `true` / `false` | 直接折成整数字面量 `1` / `0` |

### 12.3 Keywords / 关键字

```
int  void  if  else  while  for  return  break  continue
string  import  byte  char  short  long  struct  do  phys  goto  bool
```

* `import` 已被移除：`import 函数名;` 会报"语法错误: 旧 'import 函数名;' 语法已移除，请使用 #include <模块名>" / `import` is removed.
* `phys` 是保留字（解析为类型声明分支），当前示例未使用 / reserved keyword, unused in samples.

### 12.4 Directive / 指令

```
#include <组名>
```

批量导入一组内建函数（组名：`io` / `math` / `string` / `mem` / `engine` / `persist` / `anim` / `noise` / `gl_draw`）。**必须 include 才能调用该组函数**；未 include 而调用会在编译期报"未定义的函数" / You **must** include a group to call its functions.

### 12.5 Operators / 运算符

```
+  -  *  /  %        算术
==  !=  <  >  <=  >=  比较
&&  ||  !            逻辑
&   |（位操作，单 & 为取地址）
=  赋值    ->  指针成员    *  解引用/声明指针（词法器按上下文区分）
```

* 单字符 `|`（无 `||` 上下文）→ `TOK_UNKNOWN`，编译报错 / a bare `|` is a compile error.

---

## 13. X Language: Type System / X 语言：类型系统

Parser: `vm/compiler/AST.h`、`CodeGenerator.cpp`.

| Type / 类型 | Storage / 存储 | Notes / 说明 |
| --- | --- | --- |
| `byte` / `char` | 1 字节，占 1 个 16 位槽 | 0..255 |
| `short` / `int` | 2 字节，1 个 16 位槽 | 与 VM 字长一致（-32768..32767） |
| `long` | 4 字节，**2 个 16 位槽** | 低字 + 高字；接收 `mul`、`getX/getY` 等 32 位返回值 |
| `bool` | 1 槽 | 0/1 |
| `string` | 1 槽 | **数据段字符串指针**（不是内联文本） |
| `struct` | 字段之和 | `struct 名 { ... };` 自定义结构体 |
| pointer / 指针 | `ptrLevel` | `int* p`、`int** pp`；函数指针 `int (*fp)(int,int)` |

Key points / 要点：
* Global variable initializers execute once at `main` entry / 全局变量声明后，其初始化语句在 `main` 入口执行一次。
* String literals live in the **data segment**; variables store addresses; `strlen/strcmp/strcpy/print_str` operate on addresses / 字符串字面量放在数据段，变量里存的是地址。
* `heap_start()` returns the free memory start after static data / 返回数据段静态占用结束后的空闲区起始地址。

---

## 14. X Language: Statements & Expressions / X 语言：语句与表达式

Parser: `vm/compiler/Parser.cpp`.

### 14.1 Statements / 语句

```
变量声明      int a;  int a = expr;  int a = 1, b = 2;  byte c;  long l;
if (cond) { ... } else { ... }
while (cond) { ... }
do { ... } while (cond);
for (init; cond; step) { ... }        // 完整三表达式 for
break;  continue;
return [expr];                          // 空 return 用于 void
goto 模块名(实参1, 实参2, ...);         // 废弃路径，勿用于模块间通信
表达式语句    f(a, b);  *p = 3;  x = y + 1;
```

### 14.2 Expression precedence / 表达式优先级（从低到高）

1. 赋值 `=`
2. `||`
3. `&&`
4. 比较 `== != < > <= >=`
5. 加减 `+ -`
6. 乘除模 `* / %`
7. 一元 `!`、`&`（取地址）、`*`（解引用）、负号 `-`
8. primary：字面量、标识符、函数调用、`(expr)`、数组/成员访问

### 14.3 Pointers & structs / 指针与结构体

```
int a = 5;
int* p = &a;        // 取地址
*p = 6;             // 解引用（可作左值）
p->field            // 指针成员
s.field             // 结构体成员
arr[i]              // 等价 *(arr + i)
(int*)0x1000        // 类型强转（地址常量转指针）
```

### 14.4 Calling convention / 调用约定（对写脚本很重要）

* Normal function calls: arguments in order; result in **R0**. `long` returns: **R0 = low 16 bits, R1 = high 16 bits** (`mul`, `getX`, `getY` etc.) / 普通函数调用结果写 R0；long 返回 R0 低字 + R1 高字。
* Builtin calls: the compiler auto-organizes arguments into an argument struct + `syscall`; source-level syntax is just `f(a, b, c)` / 内建函数调用源码层就是普通 `f(a, b, c)` 语法。
* Array-returning builtins (`findUnitByRect/CirCle/Sector`): **R0 = array address, R1 = length**; elements are 16-bit ints stored 2 bytes each / 数组型返回：R0=数组地址、R1=长度，元素按 2 字节一个顺序存放。

---

## 15. X Language: Custom Functions / X 语言：自定义函数

* Supports `int`/`void` (and other types) return values; locals on stack frames (params at `[bp+4+2i]`, locals below `[bp-2]`) / 支持各种返回值的函数定义，局部变量在栈帧。
* Function calls at source level `f(a, b)`; the compiler generates `fn_<函数名>` labels, `call` instruction / 编译器为每个函数生成 `fn_<name>` 标签。
* Function pointers: declare `int (*fp)(int,int)`, assign `fp = 某个函数名`, call `fp(a,b)` / 函数指针支持。
* ⚠️ Avoid function names that collide with builtins / 函数名不能与内建函数同名。
* Samples use the single-`main()` + per-frame pattern (`main.x`, `test.x`, `test0.x`, `canvas_ui.x`); custom functions can be used to factor out logic / 示例脚本全部是单 main + 每帧执行的写法，自定义函数可用于复用逻辑。

---

## 16. Built-in Function Groups Overview / 内建函数分组总览

| Group / 组 | `#include` | Functions / 函数 | Purpose / 用途 |
| --- | --- | --- | --- |
| `io` | `<io>` | `print` `print_str` `print_long` | 打印调试 |
| `math` | `<math>` | `add` `sub` `mul` `max` `min` `abs` `clamp` | 整数运算 |
| `string` | `<string>` | `strlen` `strcmp` `strcpy` | 字符串（按数据段地址） |
| `mem` | `<mem>` | `heap_start` | 空闲内存起点 |
| `engine` | `<engine>` | 见 §17 | 单位 / 世界 / UI / 事件 |
| `persist` | `<persist>` | `setVar` `getVar` `hasVar` `delVar` `clearVar` | 全局持久变量 |
| `anim` | `<anim>` | 见 §18 | 骨骼节点控制 |
| `noise` | `<noise>` | 见 §19 | 地形噪声读写 |
| `gl_draw` | `<gl_draw>` | 见 §21 | Canvas / 画笔 / 位图绘制 |

**Deprecated / 废弃内建**（已登记但未实现，启动时打"缺少实现"警告，调用会失败，**勿调用** / registered but unimplemented; startup warning; calls fail — do not call）：`setMainControl`、`addUiCmd`、`clearUiCmd`、`getAllEvents`、`getWalkAbleId`（engine 组）。
---

## 17. engine Group / engine 组

All signatures & semantics from `vm/use/engin.cpp`. Types: `i16` = 16-bit signed, `u16` = 16-bit unsigned, `i32` = 32-bit (2 slots), `str` = data-segment string address / 全部签名与语义来自 `vm/use/engin.cpp`。

### 17.1 Unit values / 单位数值

| Function / 函数 | Returns / 返回 | Notes / 说明 |
| --- | --- | --- |
| `setHp(i16)` `setMaxHp(i16)` `setSpeed(i16)` `setR(i16)` `setTeam(i16)` | - | 写当前单位数值 |
| `setLoadChunk(bool)` | - | 设置是否常驻加载块 |
| `getHp()` `getMaxHp()` `getSpeed()` `getR()` `getTeam()` | u16 | 读当前单位数值 |
| `getAttackAngle()` | u16 | 攻击角度 |
| `getLoadChunk()` | 0/1 | 是否常驻 |

### 17.2 Transform / move / 单位变换与移动

| Function / 函数 | Returns / 返回 | Notes / 说明 |
| --- | --- | --- |
| `setAngle(i16)` | - | 单位旋转角（**度**；内部转弧度渲染） |
| `setXY(i32, i32)` | - | 设置像素坐标（x、y 各 2 槽） |
| `setWH(i16, i16)` | - | 设置宽高 |
| `setH(i16)` | - | 设置高度 |
| `setIndex(i16)` | - | 设置动画帧序号 |
| `setAnimation(str)` | - | 切换图集动画名 |
| `setSkeletonName(str)` | - | 切换骨骼名 |
| `setHorizontalMirroring(bool)` / `setVerticalMirror(bool)` | - | 水平 / 垂直镜像 |
| `move(i32, i32)` | - | 设置移动向量（x、y 各 2 槽），置 isMoving |
| `getX()` / `getY()` | **i32**（R0 低字 / R1 高字） | 单位世界坐标 |
| `getWidth()` `getHeight()` `getH()` `getIndex()` `getVisible()` | u16 | 尺寸 / 帧 / 可见性 |
| `getMoveDx()` / `getMoveDy()` | u16 | 移动方向分量 **×100**（-100..100） |
| `getAnimationName()` / `getSkeletonName()` / `getName()` | str 地址（R0） | 名字写入数据段空闲区，R0 返回地址 |

### 17.3 Identity / events / 身份与事件

| Function / 函数 | Returns / 返回 | Notes / 说明 |
| --- | --- | --- |
| `getThis()` | u16 | 当前单位 id |
| `setThis(id)` | - | 遍历单位刷新并把当前单位切换为 id 所指单位 |
| `eventContain(str)` | 0/1 | 当前单位事件表是否含该事件名 |
| `addEvent(str)` | - | 给当前单位追加一个事件（下帧生效） |
| `addGlobalEvent(str)` | - | 全局事件 |

### 17.4 Environment / 环境量

| Function / 函数 | Returns / 返回 | Notes / 说明 |
| --- | --- | --- |
| `getCanvasX()` / `getCanvasY()` | u16 | 相机（画布）偏移 startX/startY |
| `getPressX()` / `getPressY()` | u16 | 当前触摸点坐标 |
| `getRandom()` | u16 | 0..65535 随机数 |

### 17.5 Unit queries / 单位查询

| Function / 函数 | Returns / 返回 | Notes / 说明 |
| --- | --- | --- |
| `searchUnit(x i32, y i32, r i16)` | u16 | 以 (x,y) 半径 r 找单位，返回 id 或 0 |
| `findUnitByRect(x i32, y i32, r i16)` | 数组地址 R0、长度 R1 | 矩形范围内单位 id 数组 |
| `findUnitByCirCle(x i32, y i32, r i16)` | 同上 | 圆形范围（拼写 CirCle） |
| `findUnitBySector(x i32, y i32, r i16, angle i16, width i16)` | 同上 | 扇形范围（例：`findUnitBySector x y 200 attack_angle 90`） |
| `summon(str 名, x 格, y 格, team)` | u16 | 在 (x 格, y 格) 召唤单位，**内部 ×CELL_WIDTH/CELL_HEIGHT 转像素**；返回新单位 id |

Array usage / 数组用法示例：

```
findUnitByRect(x, y, 100);
// R0 = 数组地址，R1 = 长度；用指针遍历：[addr + 2*i]
```

### 17.6 UI

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `addUi(str 路径)` | 从 assets 加载 UI JSON 并加入（相对 assets 根） |
| `removeUi(str id)` | 移除指定 id 控件 |
| `setUiXY(str id, i16 x, i16 y)` / `setUiWH(str id, i16 w, i16 h)` | 移动 / 缩放 |
| `setUiId(str 旧, str 新)` | 改 id |
| `setVisible(str id, bool)` | 设置可见 |
| `clearUi()` | 清空全部 UI |
| `getUiVisible(str id)` | **废弃**：底层返回值未写回 R0，脚本里拿不到结果；不要调用 / deprecated, no return value |

### 17.7 Other / 其他

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `log(i16)` | 打日志 `[LOG]:值` |
| `setRunning(bool)` | 切换游戏开始 / 暂停（写 `game_main->startGame`） |

---

## 18. anim Group / anim 组

Acts on the current skeleton component (`CPUDATA::anim`). **All values ×10 fixed-point except `setId/getId`** / 作用于"当前骨骼组件"；除 `setId/getId` 外，数值均为 ×10 定点。

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `setFatherNodeX/Y(i16)` `setSonNodeX/Y(i16)` `setTrustNodeX/Y(i16)` | 节点坐标（×10） |
| `setW(i16)` `setH(i16)` | 组件宽高（×10） |
| `setId(i16)` / `getId()` | 组件 id（普通整数） |
| `setAngle(i16)` | 角度，**度×10**（内部取模 360°；`setAngle(900)` = 90°） |
| `getAngle()` | 角度×10 |
| `getFatherNodeX/Y()` `getSonNodeX/Y()` `getTrustNodeX/Y()` `getW()` `getH()` | 对应读取（×10） |
| `getCanvasX()` `getCanvasY()` | 相机偏移（×10） |
| `getPressX()` `getPressY()` | 触摸点（×10） |
| `getRandom()` | 0..65535 |
| `sin(角度×10)` / `cos(角度×10)` | **输入输出均 ×10**（`sin(900)` = sin90°×10） |

Sample / 示例（`code/test0.x`，挂在骨骼组件上，控制摆动 / attached to a skeleton component to control swinging）：

```
#include <anim>
#include <persist>
#include <engine>

int main() {
    if(getVar("tick")==3){
        setVar("tick",0);
        if (getVar("add2") != -1 && getVar("add2") != 1) {
            setVar("add2", 1);
        }
        if (getAngle() <= 1100 || getAngle() >= 1150) {
            setVar("add2", - getVar("add2"));
        }
        if (getAngle() <= 1100 || getAngle() >= 1150) {
            setAngle(getAngle() + getVar("add2"));
        }
        setVar("bx", (getAngle() - 1100) / 200);
        int cos_bx = cos(getVar("bx"));
        setAngle(getAngle() + (cos_bx+1) * getVar("add2"));
        return 0;
    }
    setVar("tick", getVar("tick")+1);
    return 0;
}
```

---

## 19. noise Group / noise 组

Acts on the current noise region (`CPUDATA::noise`, set by the noise-correction mount point) / 作用于"当前噪声区域"。

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `setNoise(x, y, 值×10)` | 写一个采样点到当前 RD 缓存 |
| `getNoise(x, y)` | 读缓存；无则回退 `fractalNoise2(x,y)`；返回 ×10 |
| `setWHNoise(str 结构名, x, y)` | 从 `StructureRes` 取结构（§9），把 w×h 数据块贴到全局 (x,y) |
| `getCenterX()` `getCenterY()` `getLeftUpX()` `getLeftUpY()` `getSizeX()` `getSizeY()` | 当前 RD 区域信息 |
| `setStep(i16)` / `getStep()` | 区域步进值 |
| `sin(i16)` / `cos(i16)` | ⚠️ **输入 = 圈数×10**（`v/10` 即一圈），输出 ×10；与 anim 组（度×10）不同，注意区分 |
| `getRandom()` | 0..65535 |

Sample usage / 示例用法（修正脚本 / correction script）：

```
#include <noise>
int main() {
    setNoise(getCenterX(), getCenterY(), 50);   // 写缓存（值 5.0）
    setWHNoise("village_road", getCenterX(), getCenterY());  // 贴结构
    return 0;
}
```

---

## 20. persist Group / persist 组

Global persistent variable map (`CPUDATA::persist`), shared process-wide across scripts/modules. Note: the save file is a separate binary format — do not confuse them / 全局持久变量表，进程内共享，跨脚本、跨模块可读写；存档文件是另一套二进制格式，勿混淆。

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `setVar(str key, i16 value)` | 写变量 |
| `getVar(str key)` | 读变量；**key 不存在返回 0** |
| `hasVar(str key)` | 是否存在（1/0） |
| `delVar(str key)` | 删除 |
| `clearVar()` | 清空全部 |

---

## 21. gl_draw Group / gl_draw 组

Drawing API (`vm/use/gl_draw.cpp`). All objects (Paint/Path/Matrix/Bitmap/Canvas/Shader) are **integer ids**, allocated by `createXxx`, released by `deleteXxx/recycleBitmap` / 所有对象都以整数 id 表示。

### 21.1 Create / destroy / 创建与销毁

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `createPaint()` | 新建画笔，默认白色，返回 id |
| `createPath()` | 新建路径，返回 id |
| `createMatrix()` | 新建矩阵，返回 id |
| `createBitmap(w, h)` | 新建空位图，返回 id |
| `loadBitmap(str path)` | 从 assets 加载位图，返回 id；**加载失败返回 -1**，使用前应判非 -1 |
| `createCanvasFromBitmap(bitmap)` | 位图建画布，返回 id |
| `createLinearGradient(x0,y0,x1,y1, r0,g0,b0,a0, r1,g1,b1,a1, tileMode)` | 线性渐变 shader，返回 id（13 个参数） |
| `deletePaint(id)` `deleteMatrix(id)` `deletePath(id)` `deleteCanvas(id)` `recycleBitmap(id)` | 释放 |

### 21.2 Paint setters / Paint 设置

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `paintSetColorRGB(paint, r,g,b)` | 设颜色（0-255），alpha=255 |
| `paintSetColorRGBA(paint, r,g,b,a)` | 设颜色含 alpha |
| `paintSetTextSize(paint, size)` | 字号 |
| `paintSetTextAlign(paint, align)` | 对齐 |
| `paintSetStyle(paint, style)` | 画笔样式 |
| `paintSetStrokeWidth(paint, w)` | 描边宽 |
| `paintSetAntiAlias(paint, bool)` | 抗锯齿 |
| `paintSetAlpha(paint, a)` | 透明度 |
| `paintSetShadowLayer(paint, radius, dx, dy, r,g,b,a)` | 阴影 |
| `paintSetShader(paint, shader)` | 绑定渐变 |
| `paintSetFakeBoldText(paint, bool)` | 伪粗体 |

### 21.3 Canvas drawing / Canvas 绘制

Params unified as `(canvas, paint, geometry...)` / 参数统一为 `(canvas, paint, 几何参数...)`。

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `canvasDrawColor(canvas, r,g,b,a)` | 整屏着色 |
| `canvasDrawRect(canvas, paint, l, t, r, b)` | 矩形 |
| `canvasDrawRoundRect(canvas, paint, l, t, r, b, rx, ry)` | 圆角矩形 |
| `canvasDrawCircle(canvas, paint, cx, cy, radius)` | 圆 |
| `canvasDrawLine(canvas, paint, x1, y1, x2, y2)` | 线段 |
| `canvasDrawText(canvas, paint, str, x, y)` | 文本（str 为数据段字符串） |
| `canvasDrawPath(canvas, paint, path)` | 路径 |
| `canvasDrawBitmap(canvas, bitmap, paint, x, y)` | 位图 |
| `canvasDrawBitmapScaled(canvas, bitmap, paint, l, t, w, h)` | 缩放绘制 |
| `canvasDrawBitmapRect(canvas, bitmap, paint, srcL,srcT,srcR,srcB, dstL,dstT,dstR,dstB)` | 源矩形→目标矩形 |
| `canvasSave(canvas)` / `canvasRestore(canvas)` | 保存 / 恢复画布状态 |

### 21.4 Path

`pathMoveTo(path, x, y)`、`pathLineTo(path, x, y)`、`pathCubicTo(path, x1,y1, x2,y2, x3,y3)`、`pathClose(path)`、`pathReset(path)`。

### 21.5 Query / 查询

`measureText(paint, str)` → 文本像素宽；`getScreenWidth()` / `getScreenHeight()` → 屏幕尺寸。

Real sample / 真实示例（`code/canvas_ui.x`）：

```
#include <gl_draw>

int main() {
    canvasDrawColor(0, 255, 0, 0, 255);
    return 0;
}
```

> 这里的 `0` 是 canvas id。实用做法：`createBitmap` → `createCanvasFromBitmap` → 绘制 → 最终把位图画到主画布 / Practical pattern: createBitmap → createCanvasFromBitmap → draw → blit to main canvas.

---

## 22. io / math / string / mem Groups / io、math、string、mem 组

### 22.1 io（`#include <io>`）

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `print(i16)` | 打印数字（stdout + LogCat `[PRINT]:`），R0 返回原值 |
| `print_str(str 地址)` | 打印字符串 |
| `print_long(低, 高)` | 按无符号 32 位打印 |

### 22.2 math（`#include <math>`）

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `add(a, b)` / `sub(a, b)` | 加减，更新标志位，R0 返回 |
| `mul(a, b)` | 32 位结果：**R0 低字、R1 高字** |
| `max(a, b)` / `min(a, b)` | 取大 / 小 |
| `abs(v)` | 绝对值 |
| `clamp(v, lo, hi)` | 钳制 |

### 22.3 string（`#include <string>`）

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `strlen(str 地址)` | 长度 |
| `strcmp(s1 地址, s2 地址)` | 相等 → R0=1 且置 ZF；否则 R0=0 |
| `strcpy(dst 地址, src 地址)` | 复制，R0 = 复制字节数 |

### 22.4 mem（`#include <mem>`）

| Function / 函数 | Notes / 说明 |
| --- | --- |
| `heap_start()` | 空闲内存起始地址（数据段静态占用之后） |

---

## 23. Unit Conventions & Deprecated Code / 量纲速查与废弃代码约定

### 23.1 Unit-convention table / 量纲速查

| Value / 数值 | Convention / 量纲 | Source / 来源 |
| --- | --- | --- |
| `anim` 组坐标 / 宽高 | ×10 定点 | `anim.cpp` |
| `anim` 组角度 | **度×10** | `setAngle(AA/10.0)` + fmod 360 |
| `anim` 组 sin/cos | 输入度×10、输出×10 | `v*2π/360` |
| `noise` 组噪声值 | ×10 定点 | `getNoise/setNoise` 除以 10 |
| `noise` 组 sin/cos | **输入圈×10**（v/10 = 圈）、输出×10 | `v*2π`，与 anim 不一致 |
| `engine` `getAngle()` | 返回 `data->angle`；**推断量纲为 ×10 度**（如 1100 ≈ 110°，`test0.x` 用法佐证；如有出入以实测为准） | `engin.cpp` |
| `engine` `setAngle()` | 直接**度**（内部转弧度） | `Animation::setRotation(angle)` |
| `engine` `getMoveDx/Dy` | ×100（-100..100） | `move->dx*100` |
| `engine` `getX/getY` | 32 位（R0 低字 / R1 高字） | `returnL` |
| `engine` `summon` | x/y 参数为**格**，内部 ×CELL | `AL*CELL_WIDTH` |
| `findUnitBySector` | 第 5、6 参为角度与角宽 | `engin.cpp` |
| `getRandom` | 0..65535 | 各实现一致 |

### 23.2 Deprecated / legacy code / 废弃历史代码（勿动、勿推理）

以下内容属于历史遗留的**废弃代码 / 废弃写法**，**不是缺口、不是待修功能**；看到就当没看见，不补、不改、不对齐 / The items below are deprecated legacy code, not gaps; do not touch, do not infer from them:

1. **`action` 块 / `action` blocks** — 单位 JSON 的 `condition/operate` 命令体系无执行器，属废弃；行为逻辑一律写 X 脚本（`vm_code`）/ no executor; behavior goes in X scripts.
2. **`cmds` 数组 / `cmds` arrays** — UI、骨骼 JSON 里残留的 `cmds` 是废弃写法，无任何效果；按钮点击只走 `vm_code`。菜单 JSON（`menu.json`）里没有 `vm_code` 的按钮点击不会有行为 / deprecated; buttons execute `vm_code` only.
3. **地图 correction 里的 `cmds` / `cmds` in map correction** — `maps/mp.json` 的 `correction_noise_value.cmds` 是废弃写法；新配置只写 `"vm_code": "脚本模块名"` 并注册脚本 / legacy write; new configs use `"vm_code"`.
4. **`walkable` 硬编码 `[0]` / `walkable` fixed to `[0]`** — 单位可走瓦片固定为 0，属既定现状，不是 bug，无需 JSON 配置 / current state by design.
5. **跨模块 `goto` / cross-module `goto`** — 不用于模块间通信；模块间数据交换只用 `persist` / inter-module data uses `persist` only.
6. **未实现内建 / unimplemented builtins** — `setMainControl / addUiCmd / clearUiCmd / getAllEvents / getWalkAbleId` 只在组表登记、无实现；`getUiVisible` 不返回值——都不要调用 / do not call.

### 23.3 Development notes / 开发注意事项

1. **字符串是数据段指针 / Strings are data-segment pointers** — `string` 变量、`getName()` 等返回的都是地址；要用 `strcmp` 比较、`print_str` 打印、`strcpy` 复制。
2. **单位脚本每帧执行 / Unit scripts run every frame** — `vm_code` 每帧 run 一次，别在脚本里做耗时循环；`getVar/setVar` 适合做跨帧状态。
3. `import` **已废弃 / is removed** — 写 `import` 直接报错，改用 `#include <组名>`。
4. **module 名 = 文件名去扩展名 / Module name = filename stem** — `vm_code` / `correction.vm_code` / UI `vm_code` 里写的都是这个 stem；改文件名等于改引用名，记得同步 `code_index.json` 与 JSON 引用。
5. **编译失败静默跳过 / Compile failures are silent** — 某 `.x` 编译失败只打日志，该模块缺失；运行 `XVM::run(缺失模块)` 直接 return，不报错。
6. **骨骼组件 / 单位每帧各自 run / Multiple mount points run scripts per frame** — 挂载点多（单位、骨骼、按钮、噪声区域），脚本别假设自己独享 VM；`CPUDATA::unit/anim/noise` 由调用方在 run 前设置，脚本内可安全读对应组的 getter。

---

## Appendix A / 附录 A：脚本模板 / Script Templates

### A1. Minimal entry template / 最小入口模板（每帧空转）

```
// code/main.x
#include <anim>
#include <persist>
#include <engine>

int main() {
    setRunning(1);
    return 0;
}
```

### A2. Unit behavior script template / 单位行为脚本模板

```
// code/my_unit.x —— 单位 JSON 里 "vm_code": "my_unit"
#include <engine>
#include <persist>
#include <anim>
#include <io>

int main() {
    // 每帧执行：事件判断
    if (eventContain("move")) {
        if (getMoveDx() > 0) setHorizontalMirroring(1);
        if (getMoveDx() < 0) setHorizontalMirroring(0);
    }
    if (eventContain("attack")) {
        // 用扇形查询攻击范围内敌人
        findUnitBySector(getX(), getY(), 200, getAttackAngle(), 90);
        // R1 = 数组长度；[R0 + 2*i] 取第 i 个单位 id
        // ...（用 persist 变量中转）
    }
    // 全局状态
    setVar("frame", getVar("frame") + 1);
    return 0;
}
```

### A3. UI button script template / UI 按钮脚本模板

```
// code/open_panel.x —— 按钮 JSON 里 "vm_code": "open_panel"
#include <engine>

int main() {
    addUi("ui/gameui/backpack.json");
    return 0;
}
```

### A4. Skeleton component script template / 骨骼组件脚本模板

```
// code/tick_anim.x —— 骨骼组件 JSON 里 "vm_code": "tick_anim"
#include <anim>

int main() {
    setAngle(getAngle() + 10);   // 每帧 +1°（×10）
    setSonNodeY(35);             // 子节点 y = 3.5（×10）
    return 0;
}
```

### A5. Noise correction script template / 噪声修正脚本模板

```
// code/correct_village.x —— correction_noise_value 里 "vm_code": "correct_village"
#include <noise>

int main() {
    // 把结构贴到当前区域左上角
    setWHNoise("village_road", getLeftUpX(), getLeftUpY());
    return 0;
}
```

---

## Appendix B / 附录 B：核心源码索引 / Core Source Code Index

| Topic / 主题 | File / 文件 |
| --- | --- |
| X 词法 / X lexer | `src/cpp/vm/compiler/Lexer.cpp` |
| X 语法 / X parser | `src/cpp/vm/compiler/Parser.cpp` |
| X 类型 / AST / X types | `src/cpp/vm/compiler/AST.h` |
| 代码生成 / Code generation | `src/cpp/vm/compiler/CodeGenerator.cpp` |
| 汇编器 / Assembler | `src/cpp/vm/assembler/Assembler.cpp` |
| 编译 / 运行入口 / Compile & run entry | `src/cpp/vm/use/XVM.cpp`、`src/cpp/vm/toolchain/XToolchain.h` |
| 内建组表 / Builtin group table | `src/cpp/vm/common/BuiltinGroups.cpp` |
| 内建注册 / Builtin registration | `src/cpp/vm/vm/builtins/BuiltinRegistry.cpp`、`BuiltinFunctions.cpp` |
| io / math / string / mem | `src/cpp/vm/vm/builtins/builtin_*.cpp` |
| engine | `src/cpp/vm/use/engin.cpp` |
| anim | `src/cpp/vm/use/anim.cpp` |
| noise | `src/cpp/vm/use/builtin_noise.cpp`、`src/cpp/runtime/PerlinNoise.cpp` |
| persist | `src/cpp/vm/use/builtin_persist.cpp` |
| gl_draw | `src/cpp/vm/use/gl_draw.cpp`、`src/cpp/jni/JNIBridge.h` |
| 单位 JSON / Unit JSON | `src/cpp/units/ldata/LoadUnits.cpp`、`units/rdata/ReadUnits.cpp` |
| 图集动画 JSON / Sprite-sheet animation | `src/cpp/base/AnimationResource.cpp` |
| 骨骼 JSON / Skeleton JSON | `src/cpp/skeleton_animation/SkeletonLoader.cpp` |
| UI JSON | `src/cpp/widget/JsonUIParser.cpp` |
| 地图 JSON / Map JSON | `src/cpp/runtime/BiomeConfigLoader.cpp` |
| 图块 JSON / Tile JSON | `src/cpp/runtime/ChunkTileLoader.cpp` |
| 结构 JSON / Structure JSON | `src/cpp/runtime/StructureRes.cpp` |
| 指令模板 / Command templates | `src/cpp/CmdFunction.cpp` |
| 持久化 / Persistence | `src/cpp/runtime/ChunkUnitPersistence.cpp`、`DataBusPersistence.cpp` |

---

