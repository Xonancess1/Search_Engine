#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "helpwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    helpwindow* m_h;

private slots:
    // Oбщая функция ----------------------------------------------- {
    void openFolder(QWidget *m_pathLabel, QString &fileExtension);
    // Oбщие функции ----------------------------------------------- }


    // txt --------------------------------------------------------------------- {
    void showListTxtFiles();

    void on_openTxtButton_clicked();

    void addTxtToConfig(QWidget *m_label);
    // txt --------------------------------------------------------------------- }


    // Config.json ------------------------------------ {
    void on_openConfigButton_clicked();
    // Config.json ------------------------------------ {


    // Requsets.json ---------------------------------- {
    void on_openRequestsButton_clicked();
    // Requsets.json ---------------------------------- }


    // help window ----------------------------------- {
    void on_openHelpButton_clicked();

    void on_closeHelpButton_clicked();
    // help window ----------------------------------- }


    // answers ----------------------------------- {
    void showAnswer();

    void on_saveButton_clicked();
    // answers ----------------------------------- {


    // start ----------------------------------------- {
    void on_startButton_clicked();
    // start ----------------------------------------- }


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
