#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
#include <iomanip>

class SensorSimulator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<double> distribution;

    double meanValue;      // average value
    double stdDeviation;   // standard deviation
    int intervalMs;        // interval in milliseconds

    std::atomic<bool> running{false};

public:
    SensorSimulator(double mean, double stdDev, int intervalMilliseconds)
        : gen(rd()),
          distribution(mean, stdDev),
          meanValue(mean),
          stdDeviation(stdDev),
          intervalMs(intervalMilliseconds) {}

    // generating one value
    double generateValue() {
        return distribution(gen);
    }

    // launch continuous generation with callback function
    void startContinuous(std::function<void(double)> callback) {
        running = true;

        while (running) {
            auto startTime = std::chrono::steady_clock::now();

            double value = generateValue();
            callback(value);

            auto endTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                endTime - startTime).count();

            // execution time compensation with callback
            int sleepTime = intervalMs - elapsed;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        }
    }

    void stop() {
        running = false;
    }

    // change parameters continuous
    void setMean(double mean) {
        meanValue = mean;
        distribution = std::normal_distribution<double>(meanValue, stdDeviation);
    }

    void setStdDeviation(double stdDev) {
        stdDeviation = stdDev;
        distribution = std::normal_distribution<double>(meanValue, stdDeviation);
    }

    void setInterval(int intervalMilliseconds) {
        intervalMs = intervalMilliseconds;
    }
};
