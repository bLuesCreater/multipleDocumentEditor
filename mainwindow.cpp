#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "subwindow.h"

#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , subWindowsAreaPtr(new QMdiArea(this))
    , toolBarPtr(new QToolBar(this))
    , fileMenuPtr(new QMenu("文件"))
{
    ui->setupUi(this);
    // 修改主窗口标题
    setWindowTitle("多文档编辑器");
    // 设置中心部件
    setCentralWidget(subWindowsAreaPtr);
    // 设置主窗口关闭即销毁
    setAttribute(Qt::WA_DeleteOnClose);
    // 添加工具栏动作
    toolBarPtr->addAction("新建", this, &MainWindow::on_menuBar_newAction_clicked);
    toolBarPtr->addAction("打开", this, &MainWindow::on_menuBar_openAction_clicked);
    toolBarPtr->addAction("保存", this, &MainWindow::on_menuBar_saveAction_clicked);
    toolBarPtr->addAction("另存为...", this, &MainWindow::on_menuBar_saveAsAction_clicked);
    toolBarPtr->addAction("关闭", this, &MainWindow::on_menuBar_closeAction_clicked);
    // 添加工具栏
    addToolBar(toolBarPtr);
    // 添加文件菜单动作
    fileMenuPtr->addAction("新建", this, &MainWindow::on_menuBar_newAction_clicked, QKeySequence::New);
    fileMenuPtr->addAction("打开", this, &MainWindow::on_menuBar_openAction_clicked, QKeySequence::Open);
    fileMenuPtr->addAction("保存", this, &MainWindow::on_menuBar_saveAction_clicked, QKeySequence::Save);
    fileMenuPtr->addAction("另存为...", this, &MainWindow::on_menuBar_saveAsAction_clicked, QKeySequence::SaveAs);
    fileMenuPtr->addAction("关闭", this, &MainWindow::on_menuBar_closeAction_clicked, QKeySequence::Close);
    // 添加文件菜单
    menuBar()->addMenu(fileMenuPtr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 获取新子窗口
SubWindow * MainWindow::getNewSubWindowPtr()
{
    // 新子窗口
    SubWindow * newSubWindowPtr = new SubWindow(subWindowsAreaPtr);
    // 添加新子窗口到子窗口区域
    subWindowsAreaPtr->addSubWindow(newSubWindowPtr);
    // 返回新子窗口
    return newSubWindowPtr;
}

// 点击菜单栏文件菜单的新建动作
void MainWindow::on_menuBar_newAction_clicked()
{
    // 尝试
    try
    {
        // 创建新子窗口并显示
        getNewSubWindowPtr()->show();
    }
    // 捕获异常
    catch(const std::exception & e)
    {
        // 报错
        QMessageBox::critical(this, "错误", e.what());
    }
}

// 点击菜单栏文件菜单的打开动作
void MainWindow::on_menuBar_openAction_clicked()
{
    // 新子窗口
    SubWindow * newSubWindowPtr = getNewSubWindowPtr();
    // 尝试
    try
    {
        // 创建新子窗口并在新子窗口打开文件
        newSubWindowPtr->openFile();
    }
    // 捕获异常
    catch (const std::exception & e)
    {
        // 弹窗报错
        QMessageBox::critical(this, "错误", e.what());
        // 关闭新子窗口
        newSubWindowPtr->close();
        // 返回
        return;
    }
    // 如果新子窗口用户取消打开文件
    if(newSubWindowPtr->windowFilePath().isEmpty())
    {
        // 关闭新子窗口
        newSubWindowPtr->close();
        // 返回
        return;
    }
    // 显示新子窗口
    newSubWindowPtr->show();
}

// 点击菜单栏文件菜单的保存动作
void MainWindow::on_menuBar_saveAction_clicked()
{
    // 如果没有活动子窗口
    if(subWindowsAreaPtr->activeSubWindow() == nullptr)
    {
        QMessageBox::warning(this, "警告", "无活跃子窗口");
        return;
    }
    // 尝试
    try
    {
        // 在活跃子窗口保存文件
        qobject_cast<SubWindow *>(subWindowsAreaPtr->activeSubWindow())->saveFile();
    }
    // 捕获异常
    catch (const std::exception& e)
    {
        // 弹窗报错
        QMessageBox::critical(this, "错误", e.what());
    }
}

// 点击菜单栏文件菜单的另存为动作
void MainWindow::on_menuBar_saveAsAction_clicked()
{
    // 如果没有活动子窗口
    if(subWindowsAreaPtr->activeSubWindow() == nullptr)
    {
        QMessageBox::warning(this, "警告", "无活跃子窗口");
        return;
    }
    // 尝试
    try
    {
        // 在活跃子窗口另存为文件
        qobject_cast<SubWindow *>(subWindowsAreaPtr->activeSubWindow())->saveAsFile();
    }
    // 捕获异常
    catch (const std::exception& e)
    {
        // 弹窗报错
        QMessageBox::critical(this, "错误", e.what());
    }
}

// 点击菜单栏文件菜单的关闭动作
void MainWindow::on_menuBar_closeAction_clicked()
{
    // 关闭窗口
    close();
}

// 关闭事件(重写)
void MainWindow::closeEvent(QCloseEvent * event)
{
    // 子窗口表
    QList<QMdiSubWindow *> subWindows = subWindowsAreaPtr->subWindowList();
    // 遍历子窗口表
    for(QMdiSubWindow * subWindowPtr : subWindows)
    {
        // 如果关闭激活的子窗口失败
        if(!subWindowPtr->close())
        {
            // 忽略关闭事件
            event->ignore();
            // 返回
            return;
        }
    }
    // 同意关闭事件
    event->accept();
}

