#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

/**
* Класс для работы с JSON-файлами
*/

class ConverterJSON
{
public:
    // ---------------------------------------------- {
    /*
    * Получит экземпляр класса ConverterJSON.
    * @return Вернёт экземпляр singleton
    */
    static ConverterJSON* getInstance();
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Метод получения содержимого файлов
    * @return Возвращает список с содержимым файлов перечисленных в config.json
    */
    std::vector<std::string> getTextDocuments();
    // ---------------------------------------------- }

    // ---------------------------------------------- {
    /**
    * Метод считывает поле max_responses для определения предельного количества ответов на один запрос
    * @return
    */
    int getResponsesLimit() const;
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> getRequests();
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Положить в файл answers.json результаты поисковых запросов
    */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Читает файл config, указанный в CONFIG_FILE_PATH
     */
    void readConfigFile(std::string path);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Читает файл requests, указанный в REQUEST_FILE_PATH
     */
    void readRequestFile(std::string path);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
      * Получит максимальное количество ответов, которое может вернуть search server.
      * @return возвращает максимальное количество ответов
      */
    int getMaxResponses() const;
    // ---------------------------------------------- }


private:
    ConverterJSON() = default;
    static ConverterJSON* instance;
    const std::string ANSWERS_FILE_PATH = "answers.json";
    std::string applicationName;
    std::string applicationVersion;
    int maxResponses  {5};
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;
};

#endif // CONVERTERJSON_H
