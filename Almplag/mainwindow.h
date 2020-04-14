#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void optionsradiobutt_clicked();
    void choosebuttonsclicked();
private:
    Ui::MainWindow *ui;
private slots:

};
#endif // MAINWINDOW_H
