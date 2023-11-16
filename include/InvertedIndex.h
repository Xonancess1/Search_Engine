#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

struct Entry
{
    size_t doc_id, count;

    // Данный оператор необходим для проведения тестовых сценариев:
    bool operator ==(const Entry& other) const
    {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex
{
public:
    InvertedIndex() = default;

    // ---------------------------------------------- {
    /**
    * Обновить или заполнить базу документов, по которой будем совершать
    поиск
    * @param texts_input содержимое документов
    */
    void updateDocumentBase(const std::vector<std::string>& input_docs);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
    * Метод определяет количество вхождений слова word в загруженной базе
    документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> getWordCount(const std::string& word);
    // ---------------------------------------------- }


    // ---------------------------------------------- {
    /**
     * Получит количество слов в определенном документе
     * @param [in] word — слово, которое учитывается в документе.
     * @param [in] doc_id — идентификатор документа для поиска
     * @return количество слов в определенном документе
     */
    size_t getWordCountInDoc(const std::string& word, const size_t doc_id) const; // ADDITIONAL METHOD
    // ---------------------------------------------- }


private:
    // ---------------------------------------------- {
    /**
     * Выполнит индексацию отдельного файла
     * @param [in] fileContent — std::string с содержимым файла
     * @param [in] docId — идентификатор файла
     */
    void indexTheFile(const std::string& fileContent, size_t docId);
    // ---------------------------------------------- }

    std::map<int, std::string> document_list; // map документов(уникальный идентификатор и имя файла)
    static std::mutex mutexIndexMap; // доступ к индексной map
    std::map<std::string, std::vector<Entry>> frequencyDictionary; // частотный словарь для всех файлов
    bool indexingIsOngoing;
};

#endif // INVERTEDINDEX_H
