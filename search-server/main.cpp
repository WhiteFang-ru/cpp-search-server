// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:
// см ответ на загадку в пердыдущем коммите "riddle commit"
// Закомитьте изменения и отправьте их в свой репозиторий.

// Далее: поисковик с ранжированием по TF-IDF , итоговая работа по спринту 1, для код-ревью

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance; // calculated via TF-IDF
};

class SearchServer {
public:
    
    int document_count_ = 0; // is number of docs in "database"
    
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);

        for(const auto& word: words) {
           word_to_document_freqs_[word][document_id] += 1.0/words.size();
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto top_relevant_documents = FindAllDocuments(query);

        sort(top_relevant_documents.begin(), top_relevant_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (top_relevant_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            top_relevant_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return top_relevant_documents;
    }
private:
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    map<string, map<int, double>> word_to_document_freqs_; // is {each_word_in_the_doc, {doc_containing_the_word, TF_for_the_word}

    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }


    vector<Document> FindAllDocuments(const Query& query) const {
        vector<Document> relevant_documents;
        map<int, double> document_to_relevance; // is {id_of_the_doc_found, relevance_of_the_doc}

        for(const auto& plus_word: query.plus_words) {
            if(word_to_document_freqs_.count(plus_word) > 0) {
                int num_of_docs_with_plus_word = word_to_document_freqs_.at(plus_word).size();
                double IDF = log((static_cast<double>(document_count_))/num_of_docs_with_plus_word);
                for(const auto& [id, TF]: word_to_document_freqs_.at(plus_word)) {
                    document_to_relevance[id] += IDF * TF;
                }
            }
        }
  
        for(const auto& minus_word: query.minus_words) {
            if(word_to_document_freqs_.count(minus_word) > 0) {
                for (const auto& [id, rel]: word_to_document_freqs_.at(minus_word)) {
                    document_to_relevance.erase(id);
                }
            }
        }

        for(const auto& [doc_id, rel]: document_to_relevance) {
            Document document;
            document.id = doc_id;
            document.relevance = rel;
            relevant_documents.push_back(document);
        }
 
        return relevant_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    search_server.document_count_ = ReadLineWithNumber();
    for (int document_id = 0; document_id < search_server.document_count_; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
