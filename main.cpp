 #include <iostream>

#include "Postbinary.h"







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
    
    double meanValue;      // Среднее значение
    double stdDeviation;   // Стандартное отклонение (погрешность)
    int intervalMs;        // Интервал в миллисекундах
    
    std::atomic<bool> running{false};

public:
    SensorSimulator(double mean, double stdDev, int intervalMilliseconds)
        : gen(rd()), 
          distribution(mean, stdDev),
          meanValue(mean),
          stdDeviation(stdDev),
          intervalMs(intervalMilliseconds) {}

    // Генерация одного значения
    double generateValue() {
        return distribution(gen);
    }

    // Запуск непрерывной генерации с callback функцией
    void startContinuous(std::function<void(double)> callback) {
        running = true;
        
        while (running) {
            auto startTime = std::chrono::steady_clock::now();
            
            double value = generateValue();
            callback(value);
            
            auto endTime = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                endTime - startTime).count();
            
            // Компенсация времени выполнения callback
            int sleepTime = intervalMs - elapsed;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        }
    }

    void stop() {
        running = false;
    }

    // Изменение параметров на лету
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

int visualiseResults() {

       // Создаем симулятор: среднее=25.0, отклонение=2.0, интервал=500мс
    SensorSimulator sensor(17.5, 0.258, 500);

    std::cout << "Симулятор датчика запущен (Ctrl+C для остановки)\n";
    std::cout << "Среднее значение: 25.0, Погрешность: ±2.0\n";
    std::cout << "Интервал: 500 мс\n\n";

    // Запускаем генерацию с выводом в консоль
    sensor.startContinuous([](double value) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::cout << "Время: " << std::put_time(std::localtime(&time), "%H:%M:%S") 
                  << " | Значение: " << std::fixed << std::setprecision(2) 
                  << value << std::endl;
    });

    return 1;
}



void createBordersFromBytes() {
    Postbinary::byte bl1 = 0b01000010;
    Postbinary::byte bl2 = 0b00111111;
    Postbinary::byte bl3 = 0b01100111;
    Postbinary::byte bl4 = 0b00111000;

    float lowIntervalBorder = 0;
    Postbinary::Utilities::Convert::bytesToFloat(&lowIntervalBorder, bl1, bl2, bl3, bl4);


    Postbinary::byte bh1 = 0b01000010;
    Postbinary::byte bh2 = 0b01000100;
    Postbinary::byte bh3 = 0b10100100;
    Postbinary::byte bh4 = 0b10101000;

    float highIntervalBorder = 0;
    Postbinary::Utilities::Convert::bytesToFloat(&highIntervalBorder, bh1, bh2, bh3, bh4);

    std::cout << "Borders: [" << lowIntervalBorder << ", " << highIntervalBorder << "]" << std::endl;
}


void test(float& left, float& right) {
    std::cout << "Initial range: [" << left << ", " << right << "]" << std::endl;

    std::cout << "Numbers bits: " << std::endl;
    std::cout << Postbinary::Utilities::Convert::binaryToString(&left, 4) << std::endl;
    std::cout << Postbinary::Utilities::Convert::binaryToString(&right, 4) << std::endl;

    Postbinary::Pb64_32p postbinaryNumber;
    postbinaryNumber.convertFromFloat(&left, &right);
    std::cout << "Postbinary: " << std::endl << postbinaryNumber.toString() << std::endl;


    float minRangeStart = 0;
    float minRangeEnd = 0;
    postbinaryNumber.getMinimumFloatRange(minRangeStart, minRangeEnd);
    std::cout << "Minimum range: [" << minRangeStart << "; " << minRangeEnd << "]" << std::endl;


    float maxRangeStart = 0;
    float maxRangeEnd = 0;
    postbinaryNumber.getMaximumFloatRange(maxRangeStart, maxRangeEnd);
    std::cout << "Maximum range: [" << maxRangeStart << "; " << maxRangeEnd << "]" << std::endl;
}


int main() {


    createBordersFromBytes();


    float a = 17.0134;
    float b = 18.0928;
    test(a, b);






    // Создаем симулятор: среднее=25.0, отклонение=2.0, интервал=500мс

    float averageResult = 17.554;   // 17.554 or 95.357
    float deviation = 0.00258;        // 0.258 or
    int interval = 62; // ms
    SensorSimulator sensor(averageResult, deviation, interval);

    std::cout << "Симулятор датчика запущен (Ctrl+C для остановки)\n";
    std::cout << "Среднее значение: 25.0, Погрешность: ±2.0\n";
    std::cout << "Интервал: 500 мс\n\n";


    std::vector<float> measurements; 
    float modifier = 0;

    std::cout;

    bool debugMode = false;
    int counter = 0;


    std::cout << "#,time,min,max,average,left,right,postbinary,left1,right1,average1,left2,right2,average2" << std::endl;

    // Запускаем генерацию с выводом в консоль
    sensor.startContinuous([&measurements, &modifier, &debugMode, &counter](float value) {


        counter++;


        int MEASUREMENTS_COUNT = 16;


        // через 10 секунд начнет расти
        if (measurements.size() >= MEASUREMENTS_COUNT && counter > 160 && counter <= 320) {
            modifier += 0.0625;
        } else

        // ещё через 10 секунд начнет расти сильнее
        if (measurements.size() >= MEASUREMENTS_COUNT && counter > 320 && counter <= 400) {
            modifier += 0.625;
        }

        if (measurements.size() >= MEASUREMENTS_COUNT && counter > 400 && counter <= 480) {

        }

        if (measurements.size() >= MEASUREMENTS_COUNT && counter > 480) {
            return;
        }







        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        measurements.push_back(value + modifier);

        // wait for minimum 16 measurements
        if (measurements.size() < MEASUREMENTS_COUNT) {
            return;
        }

        // remove old measurement
        if (measurements.size() > MEASUREMENTS_COUNT) {
            measurements.erase(measurements.begin());
        }

        // find min and max
        float min = *std::min_element(measurements.begin(), measurements.end());
        float max = *std::max_element(measurements.begin(), measurements.end());

        // interval symmetrization
        float average = 0;
        for (float measurement : measurements) {
            average += measurement;
        }
        average /= 16;

        if (debugMode) {
            std::cout << "[" << min << "; " << max << "]";
        } else {
            float time = (float) counter / 16;
            std::cout << counter << "," << time << "," << min << "," << max << "," << average << ",";
        }

        float leftDiff = average - min;
        float rightDiff = max - average;
        float maxDiff = leftDiff < rightDiff ? rightDiff : leftDiff;
        if (leftDiff < rightDiff) {
            min = average - maxDiff;
        } else {
            max = average + maxDiff;
        }

        // TODO debug
        if (debugMode) {
            std::cout << ". --> Average: " << average << "; left: " << leftDiff << "; right: " << rightDiff << "; max: " << maxDiff << ". ---> " << "[" << min << "; " << max << "]" << std::endl;
            std::cout << "Numbers bits: " << std::endl;
        }

        if (debugMode) {
            std::cout << Postbinary::Utilities::Convert::binaryToString(&min, 4) << std::endl;
            std::cout << Postbinary::Utilities::Convert::binaryToString(&max, 4) << std::endl;
        }

        Postbinary::Pb64_32p p;
        p.convertFromFloat(&min, &max);

        if (debugMode) {
            std::cout << "Postbinary: " << std::endl << p.toString() << std::endl;
        }

        float minRangeStart = 0;
        float minRangeEnd = 0;
        p.getMinimumFloatRange(minRangeStart, minRangeEnd);

        if (debugMode) {
            std::cout << "Minimum range: [" << minRangeStart << "; " << minRangeEnd << "]" << std::endl;
        }

        float maxRangeStart = 0;
        float maxRangeEnd = 0;
        p.getMaximumFloatRange(maxRangeStart, maxRangeEnd);

        if (debugMode) {
            std::cout << "Maximum range: [" << maxRangeStart << "; " << maxRangeEnd << "]" << std::endl;
            std::cout << std::endl;
        }

        float minAverage = (minRangeStart + minRangeEnd) / 2;
        float maxAverage = (maxRangeStart + maxRangeEnd) / 2;


        std::cout << min << "," << max << "," << p.toString() << "," << minRangeStart << "," << minRangeEnd << "," << minAverage << "," << maxRangeStart << "," << maxRangeEnd << "," << maxAverage << std::endl;







/*
        std::cout << "Время: " << std::put_time(std::localtime(&time), "%H:%M:%S") 
                  << " | Значение: " << std::fixed << std::setprecision(2) 
                  << value << std::endl;
        */
    });

    return 0;

    
}
