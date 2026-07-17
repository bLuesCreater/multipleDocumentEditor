#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QMdiSubWindow>
#include <QTextEdit>
#include <QCloseEvent>

class SubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    SubWindow(QWidget * parent = nullptr);
    ~SubWindow();

    // 打开文件
    void openFile();
    // 保存文件
    void saveFile();
    // 另存为文件
    void saveAsFile();
    // 关闭文件
    void closeFile();
    // 关闭事件(重写)
    void closeEvent(QCloseEvent * event) override;
    // 判断文件是否已打开
    bool hasOpened(QString filePath);


private:
    // 文本框
    QTextEdit * textEditPtr;
    // 子窗口区域
    QMdiArea * subWindowsAreaPtr;

private slots:
    // 文本框文件修改状态变化槽函数
    void on_textEdit_document_ModificationChanged();

};

#endif // SUBWINDOW_H
