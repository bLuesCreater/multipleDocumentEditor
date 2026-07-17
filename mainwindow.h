#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "subwindow.h"

#include <QMainWindow>
#include <QMdiArea>
#include <QToolBar>
#include <QMenu>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // 子窗口区域
    QMdiArea * subWindowsAreaPtr;
    // 工具栏
    QToolBar * toolBarPtr;
    // 文件菜单
    QMenu * fileMenuPtr;

    // 获取新子窗口
    SubWindow * getNewSubWindowPtr();
    // 关闭事件(重写)
    void closeEvent(QCloseEvent * event) override;

private slots:
    // 点击菜单栏文件菜单的新建动作
    void on_menuBar_newAction_clicked();
    // 点击菜单栏文件菜单的打开动作
    void on_menuBar_openAction_clicked();
    // 点击菜单栏文件菜单的保存动作
    void on_menuBar_saveAction_clicked();
    // 点击菜单栏文件菜单的另存为动作
    void on_menuBar_saveAsAction_clicked();
    // 点击菜单栏文件菜单的关闭动作
    void on_menuBar_closeAction_clicked();

};
#endif // MAINWINDOW_H
