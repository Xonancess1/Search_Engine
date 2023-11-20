#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <fstream>
#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    ui->pathTxtTextEdit->setReadOnly(true);

    ui->configTxtTextEdit->setReadOnly(true);

    this->setStyleSheet("background-color: rgb(212,208,200);");

    this->setWindowTitle("Search_Engine");

    m_h = new helpwindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --------------------------------------------------------------------------------- {
void writeAnswers(const std::vector<std::vector<RelativeIndex>>& allRequestsResults)
{
    if (allRequestsResults.empty())
    {
        std::cout << "No matches are found.\n";
        return;
    }
    std::vector<std::vector<std::pair<int, float>>> allRequestsResultsReadyForJSON;
    for (auto& requestResult : allRequestsResults)
    {
        std::vector<std::pair<int, float>> requestResultReadyForJSON;
        for (auto& pageRelevance : requestResult)
        {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int) pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        allRequestsResultsReadyForJSON.push_back(requestResultReadyForJSON);
    }
    ConverterJSON::getInstance()->putAnswers(allRequestsResultsReadyForJSON);
}
// --------------------------------------------------------------------------------- }


// *General Function* -------------------------------------------------------------------- {
// функция для выбора файла/файлов "..." {
void MainWindow::openFolder(QWidget *m_pathLabel, QString &fileExtension)
{
    if(qobject_cast<QLabel *>(m_pathLabel) && (qobject_cast<QLabel *>(m_pathLabel) != nullptr))
    {
        QString path = QDir::toNativeSeparators(QFileDialog::getOpenFileName(
            this,
            "Open Document",
            QDir::currentPath(),
            "*." + fileExtension));

        QFile file = path;
        QFileInfo fileInfo(file.fileName());

        if(fileInfo.fileName() == "config.json") {
            qobject_cast<QLabel *>(m_pathLabel)->setText(path);
            showListTxtFiles();
        }
        else if(fileInfo.fileName() == "requests.json") {
            qobject_cast<QLabel *>(m_pathLabel)->setText(path);
        }
    }
    else if (qobject_cast<QTextEdit *>(m_pathLabel) && (qobject_cast<QTextEdit *>(m_pathLabel) != nullptr))
    {
        QFileDialog dialog(this);
        dialog.setDirectory(QDir::homePath());
        dialog.setFileMode(QFileDialog::ExistingFiles);
        dialog.setNameFilter("*." + fileExtension);

        QStringList fileNames;

        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        QString path = fileNames.join("\n");
        qobject_cast<QTextEdit *>(m_pathLabel)->insertPlainText(path + "\n");
    }
}
// }
// *General Function* -------------------------------------------------------------------- }


// *TXT* --------------------------------------------------------------------------------- {
// функция для вывода записанных файлов txt из config.json {
void MainWindow::showListTxtFiles()
{
    if(!(ui->pathConfigLabel->text().isEmpty()))
    {
        QString configPath = ui->pathConfigLabel->text();
        std::string configPathStd = configPath.toStdString();

        std::ifstream configFile(configPathStd);
        json configJson;
        configFile >> configJson;

        for (auto& el : configJson["files"].items())
        {
            QString pathTxt = QString::fromStdString(el.value());
            ui->configTxtTextEdit->insertPlainText(pathTxt + "\n");
        }
    }
}
// }

// Выбираем файлы txt "..."
void MainWindow::on_openTxtButton_clicked()
{
    QString file("txt");
    openFolder(ui->pathTxtTextEdit, file);
}
// }


// функция для добавления файлов txt в config.json {
void MainWindow::addTxtToConfig(QWidget *m_label){
    if(!(qobject_cast<QTextEdit *>(m_label)->toPlainText().isEmpty())
        && (qobject_cast<QTextEdit *>(m_label) != nullptr)
        ) {
            QString configPath = ui->pathConfigLabel->text();
            std::string configPathStd = configPath.toStdString();

            QString pathTxt = (qobject_cast<QTextEdit *>(m_label)->toPlainText());

            vector<string> vec;

            QStringList myStringList = pathTxt.split("\n");

            for(int index =0;index < myStringList.length();index++)
            {
                if(myStringList.at(index).toStdString() != "") {
                    vec.push_back(myStringList.at(index).toStdString());
                }
            }

            std::ifstream configFileIf(configPathStd);
            json configJson;
            configFileIf >> configJson;

            int count = 0;

            for(auto& path : vec)
            {
                for (auto& el : configJson["files"].items())
                {
                    if(el.value() == path) {
                        count++;
                    }
                }
            }

            for(auto& path : vec)
            {
                if(count == 0) {
                    configJson["files"].push_back(path);
                }
            }

            std::ofstream configFileOf(configPathStd);
            configFileOf << std::setw(4) << configJson << std::endl;
        }
}
// }
// *TXT* --------------------------------------------------------------------------------- }


// *CONFIG* --------------------------------------------------------------------------------- {
// Выбираем файл "config.json" "..."
void MainWindow::on_openConfigButton_clicked() {
    QString file("json");
    openFolder(ui->pathConfigLabel, file);
}
// }
// *CONFIG* --------------------------------------------------------------------------------- }


// *REQUESTS* --------------------------------------------------------------------------------- {
// Выбираем файл "requests.json" "..." {
void MainWindow::on_openRequestsButton_clicked()
{
    QString file("json");
    openFolder(ui->pathRequestsLabel, file);
}
// }
// *REQUESTS* --------------------------------------------------------------------------------- }


// *HELP WINDOW* ------------------------------------------------------------------------------ {
// кнопка "Инструкция". Вызов окна с объяснением работы программы {
void MainWindow::on_openHelpButton_clicked()
{
    if(!(m_h->isVisible())) {
        m_h->show();
        ui->openHelpButton->setEnabled(false);
    }
}
// }

// кнопка "X". Закрытие окна с объяснением работы программы {
void MainWindow::on_closeHelpButton_clicked()
{
    m_h->close();
    ui->openHelpButton->setEnabled(true);
}
// }
// *HELP WINDOW* ------------------------------------------------------------------------------ }


// *ANSWER* ----------------------------------------------------------------------------------- {
// Выводит ответ за запросы в answerTextEdit из answers.json {
void MainWindow::showAnswer()
{
    QFile answersFile("answers.json");

    answersFile.open(QIODevice::ReadOnly);
    QString qstr = answersFile.readAll();
    answersFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(qstr.toUtf8());
    QString formattedJsonString = doc.toJson(QJsonDocument::Indented);

    ui->answerTextEdit->setPlainText(formattedJsonString);
}
// }

// Сохранить answers.json {
void MainWindow::on_saveButton_clicked()
{
    QFileDialog fDialog(this);

    fDialog.setAcceptMode(QFileDialog::AcceptSave);
    QString filename = fDialog.getSaveFileName(this,
                                QString("Сохранить answers.json"),
                                QCoreApplication::applicationDirPath(),
                                QString("*.json"));

    if (filename.isEmpty())
        return;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << ui->answerTextEdit->toPlainText() << "\n";

    file.close();
}
// }
// *ANSWER* ----------------------------------------------------------------------------------- }


// *START* ------------------------------------------------------------------------------------ {
// Кнопка "старт". Запуск программы ------------------------------------------------- {
void MainWindow::on_startButton_clicked()
{
    if (ui->pathConfigLabel->text().isEmpty() || ui->pathRequestsLabel->text().isEmpty())
    {
        if (ui->pathConfigLabel->text().isEmpty()) {
            cerr << "\t - file not found error: config.json" << endl;
        }
        if (ui->pathRequestsLabel->text().isEmpty()) {
            cerr << "\t - file not found error: requests.json" << endl;
        }
        cout << endl;
    }
    else
    {
        //Initialization:
        QString configPath = ui->pathConfigLabel->text();
        std::string configPathStd = configPath.toStdString();

        QString requestsPath = ui->pathRequestsLabel->text();
        std::string requestsPathStd = requestsPath.toStdString();

        addTxtToConfig(ui->pathTxtTextEdit);

        try {
        ConverterJSON::getInstance()->readConfigFile(configPathStd);
        ConverterJSON::getInstance()->readRequestFile(requestsPathStd);
        std::vector<std::string> documents = ConverterJSON::getInstance()->getTextDocuments();
        auto invertedIndex = new InvertedIndex();
        invertedIndex->updateDocumentBase(documents);

        //Search:
        std::cout << "Searching...\n";
        SearchServer searchServer(*invertedIndex);
        searchServer.setMaxResponses(ConverterJSON::getInstance()->getMaxResponses());
        auto allRequestsResults = searchServer.search(ConverterJSON::getInstance()->getRequests());
        writeAnswers(allRequestsResults);
        std::cout << "End of search.\n";
        //Pause the console - uncomment if pause is necessary:
        std::cin.get();
        delete invertedIndex;
        }
        catch(...)
        {
            std::cerr << "Error";
        }

        showAnswer();
    }
}
// }
// *START* ------------------------------------------------------------------------------------ }
