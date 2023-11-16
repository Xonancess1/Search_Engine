#include "SearchServer.h"

std::set<std::string> SearchServer::getSearchWords(const std::string& request)
{
    std::set<std::string> result;
    std::istringstream ist(request);

    for (std::string word; ist >> word; )
    {
        //Преобразование символов в нижний регистр:
        std::transform(word.begin(), word.end(), word.begin(),
            [](unsigned char c){ return std::tolower(c); });

        result.emplace(word);
    }
    return result;
}

std::vector<std::pair<std::string, size_t>> SearchServer::getWordFrequency(const std::set<std::string>& words) {
    std::vector<std::pair<std::string, size_t>> result;

    for (const auto& word : words) {
        auto wordFrequency = _index.getWordCount(word);
        size_t sumWordFrequency  = 0;

        for (auto frequency : wordFrequency) {
            sumWordFrequency += frequency.count;
        }

        std::pair<std::string, size_t> wordAndFrequency;
        wordAndFrequency.first = word;
        wordAndFrequency.second = sumWordFrequency;
        result.push_back(wordAndFrequency);
    }
    return result;
}

std::vector<size_t> SearchServer::getAllDocumentsWithWords(const std::vector<std::pair<std::string, size_t>> &words)
{
    std::vector<size_t> docIds {};

    // Получение частотности и идентификаторов документов::
    for (const auto& wordAndFrequency : words)
    {
        auto frequencies = _index.getWordCount(wordAndFrequency.first);
        for (auto frequency : frequencies)
        {
            docIds.push_back(frequency.doc_id);
        }
    }

    // Получение уникальных идентификаторов из docIds:
    std::set<size_t> uniqueDocIds (docIds.begin(), docIds.end());
    docIds.clear();
    docIds.assign(uniqueDocIds.begin(), uniqueDocIds.end());
    std::sort(docIds.begin(), docIds.end(), std::less<size_t>());
    return docIds;
}

void SearchServer::sortWordsFrequency(std::vector<std::pair<std::string, size_t>>& wordFrequency)
{
    std::sort(wordFrequency.begin(), wordFrequency.end(), [](auto &left, auto &right)
              {
                  return left.second < right.second;
              });
}

size_t SearchServer::getAbsoluteRelevance(size_t docId, std::set<std::string> &uniqueWords) {
    size_t absoluteRelevance {0};
    for (const auto& word : uniqueWords)
    {
        size_t wordCountInDoc = _index.getWordCountInDoc(word, docId);
        absoluteRelevance += wordCountInDoc;
    }
    return absoluteRelevance;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> result{};
    if (queries_input.empty())
    {
        std::cout << "Requests are empty.\n";
        return result;
    }

    for (const auto& query : queries_input)
    {
        // Получит искомые слова из запроса
        std::set<std::string> searchWords = getSearchWords(query);
        if (searchWords.empty())
        {
            std::cout << "\t-bad request.\n";
            continue;
        }

        // Получит частотность для каждого слова
        auto wordsFrequency = getWordFrequency(searchWords);

        // Сортировка искомых слов по частотности в возрастающем порядке
        sortWordsFrequency(wordsFrequency);
        
        auto documentIds = getAllDocumentsWithWords(wordsFrequency);
        std::string docOrDocs = documentIds.size() == 1 ? " document " : " documents ";
        std::string wordOrWords = searchWords.size() == 1 ? " word: " : " words: ";

        // Получит абсолютную релевантность и максимальную релевантность:
        std::vector<RelativeIndex>* relativeIndexes = new std::vector<RelativeIndex>();
        size_t maxAbsoluteRelevance {0};
        for (const auto& docId : documentIds)
        {
            size_t absoluteRelevance = getAbsoluteRelevance(docId, searchWords);
            auto* relativeIndex = new RelativeIndex();
            relativeIndex->doc_id = docId;
            relativeIndex->absoluteIndex = absoluteRelevance;
            relativeIndexes->push_back(*relativeIndex);
            delete relativeIndex;
            if (absoluteRelevance > maxAbsoluteRelevance) maxAbsoluteRelevance = absoluteRelevance;
        }

        // Получит относительную релевантность для каждого документа:
        for (auto& relativeIndex : *relativeIndexes)
        {
            if (maxAbsoluteRelevance != 0)
            {
                float rank = (float) relativeIndex.absoluteIndex / (float) maxAbsoluteRelevance;
                int rounded = (int) std::round(rank * 100);
                rank = (float) rounded / 100;
                relativeIndex.rank = rank;
            }
            else relativeIndex.rank = 0;
        }

        // Отсортирует документы по релевантности (по убыванию):
        std::sort(relativeIndexes->begin(), relativeIndexes->end(), [&relativeIndexes](RelativeIndex &left, RelativeIndex &right)
                  {
                      return (left.rank > right.rank || (left.rank == right.rank && left.doc_id < right.doc_id));
                  });

       
        if (relativeIndexes->size() > maxResponses)
        {
            relativeIndexes->erase(relativeIndexes->begin() + maxResponses, relativeIndexes->end());
        }

        result.push_back(*relativeIndexes);
        delete relativeIndexes;
    }
    return result;
}

void SearchServer::setMaxResponses(const int &newMaxResponses)
{
    this->maxResponses = newMaxResponses;
}
