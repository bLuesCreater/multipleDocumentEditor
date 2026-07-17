# Multiple Document Editor

基于 Qt MDI（多文档界面）的文本编辑器，演示 `QMdiArea`、`QMdiSubWindow` 与子窗口生命周期管理。

## 功能

- **新建** — 创建空白子窗口编辑区
- **打开** — 打开文本文件（含重复文件打开检测）
- **保存** — 保存当前子窗口内容
- **另存为** — 存为新文件
- **关闭** — 关闭前提示保存未保存内容

## 技术要点

| 模块 | 说明 |
|------|------|
| `QMdiArea` | 多文档区域容器，管理所有子窗口 |
| `QMdiSubWindow` | 子窗口基类，通过 `SubWindow` 继承扩展 |
| `QTextEdit` | 文本编辑控件 |
| `QFileDialog` | 打开/保存文件对话框 |
| `QMessageBox` | 修改提示对话框 |
| `Qt::WA_DeleteOnClose` | 子窗口关闭即销毁，保证生命周期安全 |
| 异常处理 | `openFile` / `saveFile` 中文件操作失败时抛出异常 |

### 已修复的坑

1. `addSubWindow()` 后子窗口 parent 变为 viewport，需在构造时保存 `QMdiArea` 指针
2. 文件重复检测需先判空再判重复，避免取消对话框时误报
3. 未添加 `WA_DeleteOnClose` 时已关闭子窗口残留于 `subWindowList()`
4. `saveFile()` 未检查写入结果即标记已保存

## 环境要求

- **Qt**: 6.x
- **CMake**: ≥ 3.5
- **编译器**: 支持 C++17

## 构建

```bash
cd multipleDocumentEditor
cmake -B build
cmake --build build
./build/multipleDocumentEditor
```

## 文件结构

```
multipleDocumentEditor/
├── CMakeLists.txt        # CMake 构建配置
├── main.cpp              # 入口
├── mainwindow.h/cpp      # 主窗口（QMdiArea + 工具栏 + 菜单）
├── subwindow.h/cpp       # 子窗口（文件读写 + 重复检测 + 关闭处理）
├── Resource.qrc          # Qt 资源文件
└── README.md
```
