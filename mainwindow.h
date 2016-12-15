#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "video.h"
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayNextFrame(Selection lastSelection);
    void nextFrame();
    void delay(int milliseconds);

private slots:

    void on_buttonOpenVideo_clicked();

    void on_buttonPlay_clicked();

    void on_buttonAddFile_clicked();

    void on_buttonForward_clicked();

    void on_buttonPause_clicked();

    void on_buttonBackward_clicked();

    void on_buttonRed_clicked();

    void on_buttonYellow_clicked();

    void on_buttonGreen_clicked();

    void on_buttonRedYellow_clicked();

    void on_buttonSaveToFile_clicked();


    void on_buttonDeleteSelection_clicked();

    void on_sliderVideo_valueChanged(int value);

    void on_buttonMatching_clicked();

private:
    Ui::MainWindow *ui;
    Video *video;
    bool matching = false;
};

#endif // MAINWINDOW_H
