#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <random>
#include <future>

class thread_pool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<double()>> task_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop_flag;
    std::vector<double> results;
    std::mutex results_mutex;

public:
    explicit thread_pool(size_t thread_count) : stop_flag(false) {
        for (size_t i = 0; i < thread_count; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<double()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop_flag || !task_queue.empty(); });

                        if (stop_flag && task_queue.empty()) return;

                        task = std::move(task_queue.front());
                        task_queue.pop();
                    }

                    double result = task();

                    {
                        std::lock_guard<std::mutex> lock(results_mutex);
                        results.push_back(result);
                    }
                }
            });
        }
    }

    ~thread_pool() {
        stop();
    }

    void add_task(std::function<double()> task) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            task_queue.push(std::move(task));
        }
        condition.notify_one();
    }

    double get_average() {
        std::lock_guard<std::mutex> lock(results_mutex);
        if (results.empty()) return 0.0;

        double sum = 0.0;
        for (double val : results) {
            sum += val;
        }
        return sum / results.size();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop_flag = true;
        }
        condition.notify_all();
        for (std::thread& t : workers) {
            if (t.joinable()) t.join();
        }
    }
};

// zadanie 1 - sortowanie wątkowe
void parallel_sort(std::vector<int>& vec) {
    int mid = vec.size() / 2;
    thread_pool pool(2);

    std::promise<void> p1_done, p2_done;
    std::future<void> f1_done = p1_done.get_future();
    std::future<void> f2_done = p2_done.get_future();

    pool.add_task([&vec, mid, &p1_done]() -> double {
        std::sort(vec.begin(), vec.begin() + mid);
        p1_done.set_value();
        return 0.0;
    });

    pool.add_task([&vec, mid, &p2_done]() -> double {
        std::sort(vec.begin() + mid, vec.end());
        p2_done.set_value();
        return 0.0;
    });

    f1_done.wait();
    f2_done.wait();

    pool.stop();


    // Łączymy posortowane części
    //inplace_merge służy do scalania dwóch posortowanych części wektora w jedną posortowaną całość,
    //bez używania dodatkowej pamięci
    std::inplace_merge(vec.begin(), vec.begin() + mid, vec.end());
}

void zadanie1() {
    std::vector<int> numbers(10);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (auto &n : numbers) n = dis(gen);

    std::cout << "Przed sortowaniem: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    parallel_sort(numbers);

    std::cout << "Po sortowaniu: ";
    for (int n : numbers) std::cout << n << " ";
    std::cout << std::endl;
}

// zadanie 2 - statyczna pula wątków
void zadanie2() {
    thread_pool pool(4);

    pool.add_task([]() { return 10.5; });
    pool.add_task([]() { return 25.7; });
    pool.add_task([]() { return 8.3; });
    pool.add_task([]() { return 42.1; });

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Średnia wyników: " << pool.get_average() << std::endl;

                                                                      pool.stop();
}

int main() {
    std::cout << "Zadanie 1\n";
    zadanie1();

    std::cout << "\nZadanie 2\n";
    zadanie2();

    return 0;
}
