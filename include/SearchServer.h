#ifndef SEARCHSERVER_H
#define SEARCHSERVER_H

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include "InvertedIndex.h"

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    size_t absoluteIndex = 0;

    bool operator ==(const RelativeIndex& other) const
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
    bool operator > (const RelativeIndex& other) const
    {
        return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
    }
};

class SearchServer
{
public:
    // ---------------------------------------------- {
    /**
    * @param idx в конструктор класса передаётся ссылка на класс
    InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в
    запросе
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Установит максимальное количество ответов с Search Server
    * @param [in] newMaxResponses - новое значение для максимального количества ответов
    */
    void setMaxResponses(const int& newMaxResponses);
    // ---------------------------------------------- }


private:
    InvertedIndex _index;
    int maxResponses {5};


    // ---------------------------------------------- {
    /**
     * Получение искомых слов из строки requests
     * @param [in] requests - отдельная строка из requests.json
     * @return возвращает набор искомых слов
     */
    std::set<std::string> getSearchWords(const std::string& request);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Получит вектор частотностей слов
     * @param [in] words - набор слов
     * @return возвращает вектор частотностей слов
     */
    std::vector<std::pair<std::string, size_t>> getWordFrequency(const std::set<std::string>& words);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Сортирует вектор частотностей слов по возрастанию
     * @param [in/out] wordFrequency - вектор частотностей слов
    */
    void sortWordsFrequency(std::vector<std::pair<std::string, size_t>>& wordsFrequency);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Получит вектор документов, в котором можно найти все слова из requests
     * @param [in] words - вектор пар «слово-частотность»
     * @return возвращает вектор идентификаторов документов, в котором можно найти все слова
     */
    std::vector<size_t> getAllDocumentsWithWords(const std::vector<std::pair<std::string, size_t>>& words);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Вычислит абсолютную релевантность документа для определенных слов
     * @param [in] docId - идентификатор документа
     * @param [in] uniqueWords - вектор слов из requests
     * @return возвращает абсолютную релевантность
     */
    size_t  getAbsoluteRelevance (size_t docId, std::set<std::string>& uniqueWords);
    // ---------------------------------------------- }
};

#endif // SEARCHSERVER_H
