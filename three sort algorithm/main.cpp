// Benchmark of bubble sort, selection sort and merge sort.
//
// The three algorithms sort fresh copies of the same random input, so every
// measurement starts from the same unsorted data. The timer is stopped before
// the result is verified, and the program prints the mean, fastest and slowest
// of several runs together with a correctness check (sorted and same multiset).

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

void bubbleSort(std::vector<int>& values) {
    const std::size_t size = values.size();
    for (std::size_t pass = 0; pass + 1 < size; ++pass) {
        bool swapped = false;
        for (std::size_t index = 0; index + 1 < size - pass; ++index) {
            if (values[index] > values[index + 1]) {
                std::swap(values[index], values[index + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break;  // already sorted: stop early
        }
    }
}

void selectionSort(std::vector<int>& values) {
    const std::size_t size = values.size();
    for (std::size_t start = 0; start + 1 < size; ++start) {
        std::size_t smallest = start;
        for (std::size_t index = start + 1; index < size; ++index) {
            if (values[index] < values[smallest]) {
                smallest = index;
            }
        }
        if (smallest != start) {
            std::swap(values[start], values[smallest]);
        }
    }
}

void mergeRange(std::vector<int>& values, std::vector<int>& buffer,
                std::size_t left, std::size_t middle, std::size_t right) {
    std::size_t leftIndex = left;
    std::size_t rightIndex = middle;
    std::size_t out = left;

    while (leftIndex < middle && rightIndex < right) {
        if (values[rightIndex] < values[leftIndex]) {
            buffer[out++] = values[rightIndex++];
        } else {
            buffer[out++] = values[leftIndex++];
        }
    }
    while (leftIndex < middle) {
        buffer[out++] = values[leftIndex++];
    }
    while (rightIndex < right) {
        buffer[out++] = values[rightIndex++];
    }

    for (std::size_t index = left; index < right; ++index) {
        values[index] = buffer[index];
    }
}

void mergeSortRange(std::vector<int>& values, std::vector<int>& buffer,
                    std::size_t left, std::size_t right) {
    if (right - left < 2) {
        return;
    }

    const std::size_t middle = left + (right - left) / 2;
    mergeSortRange(values, buffer, left, middle);
    mergeSortRange(values, buffer, middle, right);
    mergeRange(values, buffer, left, middle, right);
}

void mergeSort(std::vector<int>& values) {
    if (values.size() < 2) {
        return;
    }
    std::vector<int> buffer(values.size());
    mergeSortRange(values, buffer, 0, values.size());
}

struct SortAlgorithm {
    const char* name;
    void (*sort)(std::vector<int>&);
};

struct Measurement {
    std::string algorithm;
    std::size_t size;
    double meanMs;
    double fastestMs;
    double slowestMs;
    bool correct;
};

Measurement measure(const SortAlgorithm& algorithm, const std::vector<int>& reference,
                    const std::vector<int>& expected, int repetitions) {
    std::vector<double> timings;
    timings.reserve(static_cast<std::size_t>(repetitions));
    bool correct = true;

    for (int run = 0; run < repetitions; ++run) {
        std::vector<int> work = reference;  // fresh copy for every run

        const Clock::time_point start = Clock::now();
        algorithm.sort(work);
        const Clock::time_point finish = Clock::now();

        timings.push_back(std::chrono::duration<double, std::milli>(finish - start).count());

        const bool sorted = std::is_sorted(work.begin(), work.end());
        const bool sameValues = sorted && std::equal(work.begin(), work.end(), expected.begin());
        correct = correct && sameValues;
    }

    double total = 0.0;
    for (std::size_t index = 0; index < timings.size(); ++index) {
        total += timings[index];
    }

    Measurement result;
    result.algorithm = algorithm.name;
    result.size = reference.size();
    result.meanMs = total / static_cast<double>(timings.size());
    result.fastestMs = *std::min_element(timings.begin(), timings.end());
    result.slowestMs = *std::max_element(timings.begin(), timings.end());
    result.correct = correct;
    return result;
}

std::vector<int> randomValues(std::size_t size, std::mt19937& engine) {
    std::uniform_int_distribution<int> distribution(0, 1000000);
    std::vector<int> values(size);
    for (std::size_t index = 0; index < size; ++index) {
        values[index] = distribution(engine);
    }
    return values;
}

void printTable(const std::vector<Measurement>& measurements) {
    std::cout << std::left << std::setw(14) << "algorithm" << std::right << std::setw(8) << "n"
              << std::setw(14) << "mean (ms)" << std::setw(14) << "min (ms)" << std::setw(14)
              << "max (ms)" << std::setw(10) << "correct" << '\n';
    std::cout << std::string(74, '-') << '\n';

    for (std::size_t index = 0; index < measurements.size(); ++index) {
        const Measurement& item = measurements[index];
        std::cout << std::left << std::setw(14) << item.algorithm << std::right << std::setw(8)
                  << item.size << std::setw(14) << std::fixed << std::setprecision(3)
                  << item.meanMs << std::setw(14) << item.fastestMs << std::setw(14)
                  << item.slowestMs << std::setw(10) << (item.correct ? "yes" : "NO")
                  << '\n';
    }
}

void writeCsv(const std::vector<Measurement>& measurements, const std::string& path) {
    std::ofstream file(path.c_str());
    if (!file) {
        std::cerr << "warning: could not write " << path << '\n';
        return;
    }

    file << "algorithm,n,mean_ms,min_ms,max_ms,correct\n";
    for (std::size_t index = 0; index < measurements.size(); ++index) {
        const Measurement& item = measurements[index];
        file << item.algorithm << ',' << item.size << ',' << std::fixed << std::setprecision(3)
             << item.meanMs << ',' << item.fastestMs << ',' << item.slowestMs << ','
             << (item.correct ? "yes" : "no") << '\n';
    }
    std::cout << "\nResults written to " << path << '\n';
}

}  // namespace

int main() {
    const int repetitions = 5;
    const std::size_t sizes[] = {1000, 5000, 10000};

    const SortAlgorithm algorithms[] = {
        {"bubble", bubbleSort},
        {"selection", selectionSort},
        {"merge", mergeSort},
    };

    std::mt19937 engine(42);  // fixed seed: the same input for every algorithm
    std::vector<Measurement> measurements;
    bool allCorrect = true;

    for (std::size_t sizeIndex = 0; sizeIndex < sizeof(sizes) / sizeof(sizes[0]); ++sizeIndex) {
        const std::size_t size = sizes[sizeIndex];
        const std::vector<int> reference = randomValues(size, engine);
        const std::vector<int> expected = [&reference]() {
            std::vector<int> sorted = reference;
            std::sort(sorted.begin(), sorted.end());
            return sorted;
        }();

        for (std::size_t algorithmIndex = 0;
             algorithmIndex < sizeof(algorithms) / sizeof(algorithms[0]); ++algorithmIndex) {
            Measurement measurement =
                measure(algorithms[algorithmIndex], reference, expected, repetitions);
            allCorrect = allCorrect && measurement.correct;
            measurements.push_back(measurement);
        }
    }

    std::cout << "\nSorting benchmark: " << repetitions << " runs per algorithm and input size\n\n";
    printTable(measurements);
    writeCsv(measurements, "results.csv");

    std::cout << "\ncorrectness: " << (allCorrect ? "all results sorted correctly" : "FAILED")
              << '\n';
    return allCorrect ? 0 : 1;
}
