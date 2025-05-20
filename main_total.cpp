#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <filesystem>
#include <omp.h>
#include <numeric>
#include <thread>

namespace fs = std::filesystem;
using namespace std;

string PreprocessText(const string& text) {
    string result;
    result.reserve(text.size());  // Space pre-allocation
    for (const char c : text) {
        if (isalpha(c) || c == '\'') { // Alphabetic characters only
            result += tolower(c);
        } else if (c == ' ') {
            result += c;
        } else {
            result += ' ';
        }
    }
    return result;
}

// split text into words
vector<string> TokenizeWords(const string& text) {
    vector<string> tokens;
    stringstream ss(text); // A string stream is created that encapsulates text. Allows the string to be treated as a data stream
    string word;
    while (ss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}

// Split text into characters
vector<char> TokenizeCharacters(const string& text) {
    vector<char> tokens;
    for (const char c : text) {
        if (isalpha(c)) {
            tokens.push_back(tolower(c));
        } else if (c == ' ') {
            tokens.push_back(c);
        } else {
          tokens.push_back(' ');
        }
    }
    return tokens;
}

// Bigrams and trigrams of words
unordered_map<string, int> ComputeWordNGrams(const vector<string>& words, const int ngram_length) {
    unordered_map<string, int> ngram_freq;
    for (size_t i = 0; i <= words.size() - ngram_length; ++i) {
        string ngram;
        for (int j = 0; j < ngram_length; ++j) {
            ngram += (j > 0 ? " " : "") + words[i + j];
        }
        ngram_freq[ngram]++;
    }
    return ngram_freq;
}

// Bigrams and trigrams of characters
unordered_map<string, int> ComputeCharNGrams(const vector<char>& chars, const int ngram_length) {
    unordered_map<string, int> ngram_freq;
    for (size_t i = 0; i <= chars.size() - ngram_length; ++i) {
        string ngram;
        for (int j = 0; j < ngram_length; ++j) {
            ngram += chars[i + j];
        }
        ngram_freq[ngram]++;
    }
    return ngram_freq;
}


// Save n-grams to file
void SaveNGramsToFile(const unordered_map<string, int>& ngram_freq, const string& filename) {
    ofstream output_file(filename, ios::app);
    if (!output_file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    for (const auto& [ngram, count] : ngram_freq) {
        output_file << ngram << " " << count << "\n";
    }
}

// Print summary
void PrintSummary(const size_t word_bigram_count, const size_t word_trigram_count,
                  const size_t char_bigram_count, const size_t char_trigram_count,
                  const size_t total_word_bigram_count, const size_t total_word_trigram_count,
                  const size_t total_char_bigram_count, const size_t total_char_trigram_count) {
    cout << "Total n-grams in texts:\n";
    cout << "-------------------------------------------------------------\n";
    cout << "Characters (Bigrams, Trigrams) | " << char_bigram_count << ", " << char_trigram_count
         << " | Quantity: " << total_char_bigram_count << ", " << total_char_trigram_count << endl;
    cout << "Words (Bigrams, Trigrams)      | " << word_bigram_count << ", " << word_trigram_count
         << " | Quantity: " << total_word_bigram_count << ", " << total_word_trigram_count << endl;
}



// SEQUENTIAL VERSION
void SequentialExecution(const string& folder, int repeat_count) {

    // Clear the output files - uncomment to save n-grams to file 
    /*ofstream("word_bigrams.txt", ios::trunc).close();
    ofstream("word_trigrams.txt", ios::trunc).close();
    ofstream("char_bigrams.txt", ios::trunc).close();
    ofstream("char_trigrams.txt", ios::trunc).close();*/

    size_t total_word_bigram_count = 0, total_word_trigram_count = 0;
    size_t total_char_bigram_count = 0, total_char_trigram_count = 0;
    size_t word_bigram_count = 0, word_trigram_count = 0;
    size_t char_bigram_count = 0, char_trigram_count = 0;

    unordered_map<string, int> unique_word_bigrams, unique_word_trigrams, unique_char_bigrams, unique_char_trigrams;

    // Load each .txt file once and store them in a vector
    vector<fs::path> base_files;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            base_files.push_back(entry.path());
        }
    }

    // Replicate the list of files 'repeat_count' times
    vector<fs::path> files;
    for (int k = 0; k < repeat_count; ++k) {
        files.insert(files.end(), base_files.begin(), base_files.end());
    }

    // Process each file
    for (const auto& file_path : files) {
        ifstream file(file_path);
        if (file) {
            string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            string preprocessed_text = PreprocessText(text);

            vector<string> words = TokenizeWords(preprocessed_text);
            vector<char> chars = TokenizeCharacters(preprocessed_text);

            unordered_map<string, int> word_bigrams = ComputeWordNGrams(words, 2);
            unordered_map<string, int> word_trigrams = ComputeWordNGrams(words, 3);
            unordered_map<string, int> char_bigrams = ComputeCharNGrams(chars, 2);
            unordered_map<string, int> char_trigrams = ComputeCharNGrams(chars, 3);

            // uncomment to save n-grams to file 
            /*SaveNGramsToFile(word_bigrams, "word_bigrams.txt");
            SaveNGramsToFile(word_trigrams, "word_trigrams.txt");
            SaveNGramsToFile(char_bigrams, "char_bigrams.txt");
            SaveNGramsToFile(char_trigrams, "char_trigrams.txt"); */

            word_bigram_count += word_bigrams.size();
            word_trigram_count += word_trigrams.size();
            char_bigram_count += char_bigrams.size();
            char_trigram_count += char_trigrams.size();

            for (const auto& [ngram, count] : word_bigrams) unique_word_bigrams[ngram] += count;
            for (const auto& [ngram, count] : word_trigrams) unique_word_trigrams[ngram] += count;
            for (const auto& [ngram, count] : char_bigrams) unique_char_bigrams[ngram] += count;
            for (const auto& [ngram, count] : char_trigrams) unique_char_trigrams[ngram] += count;

            total_word_bigram_count += accumulate(word_bigrams.begin(), word_bigrams.end(), 0,
                [](const int sum, const pair<string, int>& p) { return sum + p.second; });
            total_word_trigram_count += accumulate(word_trigrams.begin(), word_trigrams.end(), 0,
                [](const int sum, const pair<string, int>& p) { return sum + p.second; });
            total_char_bigram_count += accumulate(char_bigrams.begin(), char_bigrams.end(), 0,
                [](const int sum, const pair<string, int>& p) { return sum + p.second; });
            total_char_trigram_count += accumulate(char_trigrams.begin(), char_trigrams.end(), 0,
                [](const int sum, const pair<string, int>& p) { return sum + p.second; });
        } else {
            cerr << "Error opening file: " << file_path << endl;
        }
    }

    // Print summary
    PrintSummary(unique_word_bigrams.size(), unique_word_trigrams.size(), unique_char_bigrams.size(), unique_char_trigrams.size(),
                 total_word_bigram_count, total_word_trigram_count, total_char_bigram_count, total_char_trigram_count);
}



// PARALLEL VERSION
void ParallelExecution(const string& folder, int repeat_count) {
    size_t total_word_bigram_count = 0, total_word_trigram_count = 0;
    size_t total_char_bigram_count = 0, total_char_trigram_count = 0;
    size_t word_bigram_count = 0, word_trigram_count = 0;
    size_t char_bigram_count = 0, char_trigram_count = 0;
    
    /*ofstream("word_bigrams.txt", ios::trunc).close();
    ofstream("word_trigrams.txt", ios::trunc).close();
    ofstream("char_bigrams.txt", ios::trunc).close();
    ofstream("char_trigrams.txt", ios::trunc).close(); */

    vector<fs::path> base_files;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            base_files.push_back(entry.path());
        }
    }

    vector<fs::path> files;
    for (int k = 0; k < repeat_count; k++) {
        files.insert(files.end(), base_files.begin(), base_files.end());
    }

    unordered_map<string, int> unique_word_bigrams, unique_word_trigrams;
    unordered_map<string, int> unique_char_bigrams, unique_char_trigrams;

#pragma omp parallel
    {
        unordered_map<string, int> local_word_bigrams, local_word_trigrams;
        unordered_map<string, int> local_char_bigrams, local_char_trigrams;

#pragma omp for schedule(dynamic, 10)
        for (size_t i = 0; i < files.size(); ++i) {
            ifstream file(files[i]);
            if (file) {
                string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
                string preprocessed_text = PreprocessText(text);

                vector<string> words = TokenizeWords(preprocessed_text);
                vector<char> chars = TokenizeCharacters(preprocessed_text);

                local_word_bigrams = ComputeWordNGrams(words, 2);
                local_word_trigrams = ComputeWordNGrams(words, 3);
                local_char_bigrams = ComputeCharNGrams(chars, 2);
                local_char_trigrams = ComputeCharNGrams(chars, 3);
            }

#pragma omp critical
            {
                for (const auto& [ngram, count] : local_word_bigrams) unique_word_bigrams[ngram] += count;
                for (const auto& [ngram, count] : local_word_trigrams) unique_word_trigrams[ngram] += count;
                for (const auto& [ngram, count] : local_char_bigrams) unique_char_bigrams[ngram] += count;
                for (const auto& [ngram, count] : local_char_trigrams) unique_char_trigrams[ngram] += count;

                total_word_bigram_count += accumulate(local_word_bigrams.begin(), local_word_bigrams.end(), 0,
                    [](const int sum, const pair<string, int>& p) { return sum + p.second; });
                total_word_trigram_count += accumulate(local_word_trigrams.begin(), local_word_trigrams.end(), 0,
                    [](const int sum, const pair<string, int>& p) { return sum + p.second; });
                total_char_bigram_count += accumulate(local_char_bigrams.begin(), local_char_bigrams.end(), 0,
                    [](const int sum, const pair<string, int>& p) { return sum + p.second; });
                total_char_trigram_count += accumulate(local_char_trigrams.begin(), local_char_trigrams.end(), 0,
                    [](const int sum, const pair<string, int>& p) { return sum + p.second; });
            }
        } // end for

#pragma omp single
        {
            word_bigram_count = unique_word_bigrams.size();
            word_trigram_count = unique_word_trigrams.size();
            char_bigram_count = unique_char_bigrams.size();
            char_trigram_count = unique_char_trigrams.size();
        }
    } // end parallel section

    PrintSummary(word_bigram_count, word_trigram_count, char_bigram_count, char_trigram_count,
                 total_word_bigram_count, total_word_trigram_count, total_char_bigram_count, total_char_trigram_count);
}


uintmax_t GetFolderSizeMB(const string& folder) {
    uintmax_t total_size = 0;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            total_size += file_size(entry);
        }
    }
    return total_size / (1024 * 1024); // convert bytes to MB
}


int main() {

#ifdef _OPENMP
    cout << "_OPENMP defined" << std::endl;
#endif

    const string folder = "../texts";
    uintmax_t folder_size_MB = GetFolderSizeMB(folder);

    const int max_threads = thread::hardware_concurrency(); // To test the maximum number of threads available on the pc
    const int num_thread[12] = {2, 4, 6, max_threads, 10, 12, 14, 16, 18, 20, 22, 24}; // max_threads=8
    int max_text_iter = 10; // How many times iterate on the text folder
    int n_test_iter = 10; // How many times repeat the execution to calculate the average
    const int text_size[6] = {10, 20, 40, 60, 80, 100};
    int text_size_length = size(text_size);

    ofstream logfile("log.txt", ofstream::trunc);

    bool experiment1 = false;

    if (experiment1 == true) {
        // EXPERIMENT 1: speedup as the number of threads changes. Text size is 100MB
        logfile << "EXPERIMENT 1: speedup as the number of threads changes \n";

        // SEQUENTIAL EXECUTION
        vector<double> seq_test_times;
        cout << "SEQUENTIAL EXECUTION" << endl;

        // Iterate multiple times to evaluate the average performance
        for (int test_count = 0; test_count < n_test_iter; test_count++) {
            logfile << "Test " << test_count << " --> ";
            double start_time = omp_get_wtime();
            SequentialExecution(folder, max_text_iter); // iterate on the text folder 10 times
            double end_time = omp_get_wtime();
            const double sequential_time = end_time - start_time;
            logfile << "Sequential execution time: " << sequential_time << " seconds\n\n";
            seq_test_times.push_back(sequential_time);
        }

        // Evaluate the mean execution time
        double sum, seq_mean;
        sum = accumulate(seq_test_times.begin(), seq_test_times.end(), 0.0);
        //seq_mean = sum / seq_test_times.size();
        seq_mean = sum / static_cast<double>(seq_test_times.size()); // seq_test_times.size() returns a type size_t -> explicit cast
        logfile << "Mean sequential execution time : " << seq_mean << endl;

        //double seq_mean = 76;

        // PARALLEL EXECUTION
        vector<double> par_test_times;

        cout << "PARALLEL EXECUTION" << endl;
        // Find the optimal number of threads: Testing with variable number of threads.
        // Increasing threads does not always mean improving performance! There may be synchronization overheads.
        // If the number of threads is greater than the available cores, you may get worse times due to context switching.
        ofstream csv_file_threads("speedup_variable_threads.csv");
        csv_file_threads << "Threads,Execution Time,Speedup\n";

        for (const int &thread_num : num_thread) {
            omp_set_num_threads(thread_num);
            cout << "----------- Num used threads: " << thread_num << " -----------\n";
            logfile << "NUM THREADS: " << thread_num << "\n";

            // Iterate multiple times to evaluate the average performance
            for (int test_count = 0; test_count < n_test_iter; test_count++) {
                logfile << "Test " << test_count << " --> \n";
                double start_time = omp_get_wtime();
                ParallelExecution(folder, max_text_iter);
                double end_time = omp_get_wtime();
                const double parallel_time = end_time - start_time;
                logfile << "Parallel execution time: " << parallel_time << " seconds\n\n";
                par_test_times.push_back(parallel_time);
            }

            // Evaluate the mean execution time
            double par_sum, par_mean;
            par_sum = accumulate(par_test_times.begin(), par_test_times.end(), 0.0);
            par_mean = par_sum / static_cast<double>(par_test_times.size());
            logfile << "Mean parallel execution time: " << par_mean << " seconds\n\n";

            double speedup = seq_mean / par_mean;
            logfile << "Speedup: " << speedup << "\n\n";
            csv_file_threads << thread_num << "," << par_mean << "," << speedup << "\n";
        }

        csv_file_threads.close();

    } else {

        //EXPERIMENT 2: keep the number of threads constant = 8. increase the total size of the text set.

        vector<double> sequential_mean_times;
        logfile << "EXPERIMENT 2: Benchmarking Sequential and Parallel Execution with various text sizes \n\n";

        // SEQUENTIAL
        cout << "SEQUENTIAL EXECUTION" << endl;
        for (int i = 0; i < text_size_length; ++i) {
            vector<double> seq_times;
            logfile << "Sequential - Text size: " << text_size[i] * folder_size_MB << "MB\n";

            for (int test = 0; test < n_test_iter; ++test) {
                logfile << "Test " << test + 1 << ": ";
                double start = omp_get_wtime();
                SequentialExecution(folder, text_size[i]);
                double end = omp_get_wtime();
                double sequential_execution_time = end - start;
                logfile << sequential_execution_time << " sec\n";
                seq_times.push_back(sequential_execution_time);
            }

            double mean_seq = accumulate(seq_times.begin(), seq_times.end(), 0.0) / n_test_iter;
            sequential_mean_times.push_back(mean_seq);
            logfile << "Mean sequential execution time: " << mean_seq << " sec\n\n";
        }

        // PARALLEL
        cout << "PARALLEL EXECUTION" << endl;
        omp_set_num_threads(max_threads);
        ofstream csv_file_size("speedup_variable_size.csv");
        csv_file_size << "Text Size(MB),Parallel Time,Speedup\n";

        for (int i = 0; i < text_size_length; ++i) {
            vector<double> par_times;
            logfile << "Parallel - Text size: " << text_size[i] * folder_size_MB << "MB\n";

            for (int test = 0; test < n_test_iter; ++test) {
                logfile << "Test " << test + 1 << ": ";
                double start = omp_get_wtime();
                ParallelExecution(folder, text_size[i]);
                double end = omp_get_wtime();
                double parallel_time = end - start;
                logfile << parallel_time << " sec\n";
                par_times.push_back(parallel_time);
            }

            double par_mean = accumulate(par_times.begin(), par_times.end(), 0.0) / n_test_iter;
            double speedup = sequential_mean_times[i] / par_mean;
            logfile << "Mean parallel execution time: " << par_mean << " sec\n";
            logfile << "Speedup: " << speedup << "x\n\n";

            csv_file_size << (text_size[i] * folder_size_MB) << "," << par_mean << "," << speedup << "\n";
        }

        csv_file_size.close();
    }

    logfile.close();

    return 0;
}

