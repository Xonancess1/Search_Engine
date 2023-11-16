#include "helpwindow.h"
#include "ui_helpwindow.h"

helpwindow::helpwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::helpwindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint );
    ui->textEdit->setReadOnly(true);
    this->setStyleSheet("background-color: rgb(212,208,200);");
    this->setWindowTitle("Search_Engine.manual");
}

helpwindow::~helpwindow()
{
    delete ui;
}
