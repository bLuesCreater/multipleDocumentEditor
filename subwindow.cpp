#include "subwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMdiArea>

SubWindow::SubWindow(QWidget * parent)
    : QMdiSubWindow(parent)
    , textEditPtr(new QTextEdit(this))
    , subWindowsAreaPtr(qobject_cast<QMdiArea *>(parent))
{
    // 设置子窗口部件
    setWidget(textEditPtr);
    // 设置子窗口关闭即销毁
    setAttribute(Qt::WA_DeleteOnClose);
    // 连接信号与槽函数
    connect(textEditPtr->document()
            , &QTextDocument::modificationChanged
            , this
            , &SubWindow::on_textEdit_document_ModificationChanged);
}

SubWindow::~SubWindow()
{
    // ...
}

// 打开文件
void SubWindow::openFile()
{
    // 文件路径
    QString filePath = QFileDialog::getOpenFileName(this, "选择打开文件", "../../temp", "所有文件(*);;文本文件(.txt)");
    // 用户取消选择
    if(filePath.isEmpty())
    {
        // 返回
        return;
    }
    // 如果文件已经被打开
    if(hasOpened(filePath))
    {
        // 抛出异常
        throw std::runtime_error("文件已存在");
    }
    // 文件
    QFile file(filePath);
    // 如果文件只读打开失败
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 抛出异常
        throw std::runtime_error("文件打开失败");
    }
    // 设置窗口文件路径
    setWindowFilePath(filePath);
    // 设置窗口题目
    setWindowTitle(windowFilePath());
    // 文件输出流
    QTextStream in(&file);
    // 写打开文件内容入文本框
    textEditPtr->setText(in.readAll());
    // 关闭文件
    file.close();
    // 重置文本框文件修改状态
    textEditPtr->document()->setModified(false);
    // 返回
    return;
}

// 保存文件
void SubWindow::saveFile()
{
    // 如果子窗口无打开的文件
    if(windowFilePath().isEmpty())
    {
        // 另存为文件
        saveAsFile();
        // 返回
        return;
    }
    // 文件
    QFile file(windowFilePath());
    // 如果只写打开文件失败
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 跑出异常
        throw std::runtime_error("文件保存失败");
    }
    // 文件输入流
    QTextStream out(&file);
    // 写文本框内容入文件
    out << textEditPtr->toPlainText();
    // 关闭文件
    file.close();
    // 重置文本框文件修改状态
    textEditPtr->document()->setModified(false);
    // 返回
    return;
}

// 另存为文件
void SubWindow::saveAsFile()
{
    // 文件路径
    QString filePath = QFileDialog::getSaveFileName(this, "选择保存路径", "../../temp");
    // 如果用户取消选择
    if(filePath.isEmpty())
    {
        // 返回
        return;
    }
    // 文件
    QFile file(filePath);
    // 如果只读打开路径文件失败
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        // 抛出异常
        throw std::runtime_error("打开路径文件失败");
    }
    // 更新窗口文件路径
    setWindowFilePath(filePath);
    // 更新窗口标题
    setWindowTitle(windowFilePath());
    // 文件输入流
    QTextStream out(&file);
    // 写文本框内容入文件
    out << textEditPtr->toPlainText();
    // 关闭文件
    file.close();
    // 重置文本框文件修改状态
    textEditPtr->document()->setModified(false);
}

// 关闭文件
void SubWindow::closeFile()
{
    // 关闭窗口
    close();
}

// 文本框文件修改状态变化槽函数
void SubWindow::on_textEdit_document_ModificationChanged()
{
    // 如果文件被修改
    if(textEditPtr->document()->isModified())
    {
        // 设置带前缀的窗口标题
        setWindowTitle("* " + windowFilePath());
    }
    // 文件已保存则
    else
    {
        // 设置无前缀的窗口标题
        setWindowTitle(windowFilePath());
    }
}

// 关闭事件
void SubWindow::closeEvent(QCloseEvent * event)
{
    // 如果文本框文件已修改
    if(textEditPtr->document()->isModified())
    {
        // 回复
        QMessageBox::StandardButton reply = QMessageBox::question(this, "", "是否保存文件");
        // 如果保存
        if(reply == QMessageBox::Yes)
        {
            // 保存文件
            saveFile();
            // 同意关闭事件
            event->accept();
            // 返回
            return;
        }
        // 如果取消关闭
        else if(reply == QMessageBox::Cancel)
        {
            // 忽视关闭事件
            event->ignore();
            // 返回
            return;
        }
    }
    // 同意关闭事件
    event->accept();
}

// 判断文件是否已打开
bool SubWindow::hasOpened(QString filePath)
{
    // 如果子窗口区域不存在
    if(!subWindowsAreaPtr)
    {
        // 返回假
        return false;
    }
    // 子窗口表
    QList<QMdiSubWindow *> subWindows = subWindowsAreaPtr->subWindowList();
    // 遍历子窗口表
    for(QMdiSubWindow * subWindowPtr : subWindows)
    {
        // 如果有窗口已打开该路径
        if(subWindowPtr->windowFilePath() == filePath)
        {
            // 返回真
            return true;
        }
    }
    // 返回假
    return false;
}

