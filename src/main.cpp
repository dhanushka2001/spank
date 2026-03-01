#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <atomic>
#include <thread>
#include <random>
#include <format>
#include <filesystem>
#include <string>
#include "miniaudio.hpp"

using Clock = std::chrono::steady_clock;

void playSound(const char* file)
{
    ma_result result;
    static ma_engine engine; // static to keep it alive across calls
    static bool initialized = false;

    if (!initialized) {
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            fprintf(stderr, "Failed to init audio engine\n");
            return;
        }
        initialized = true;
    }

    result = ma_engine_play_sound(&engine, file, NULL);
    if (result != MA_SUCCESS) {
        fprintf(stderr, "Failed to play sound: %s\n", file);
    }

    // no need to sleep; miniaudio plays asynchronously
}

double read_value(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open " + path);

    double value;
    file >> value;
    return value;
}

int main(int argc, char** argv)
{
    // const std::string base = "/sys/bus/iio/devices/iio:device0/";
    const std::string base = "/sys/bus/iio/devices/iio:device3/";
    // const std::string base = "/sys/bus/iio/devices/iio:device6/";
    
    // const std::string x_path = base + "in_incli_x_raw";
    const std::string x_path = base + "in_accel_x_raw";
    const std::string y_path = base + "in_accel_y_raw";
    const std::string z_path = base + "in_accel_z_raw";

    // tune these
    double threshold = 2000.0;
    double upper_bound = 1000000.0;
    std::chrono::milliseconds cooldown(100);

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 59); // define the range
    
    namespace fs = std::filesystem;

    double lastMagnitude = 0.0;
    int slapCounter = 0;
    auto lastTriggerTime = Clock::now() - cooldown;

    std::cout << "Listening for slaps... (threshold=" << threshold << ", cooldown=" << cooldown << ")\n";

    while (true)
    {
        try
        {
            double x = read_value(x_path);
            double y = read_value(y_path);
            double z = read_value(z_path);

            double magnitude = std::sqrt(x*x + y*y + z*z);
            // double magnitude = x > 0 ? x : -x;
            // double y_mag = y > 0 ? y : -y;
            // double magnitude = std::sqrt(y*y);
            double delta = magnitude - lastMagnitude;
            if (delta < 0) delta *= -1;

            auto now = Clock::now();

            // std::cout << "delta=" << delta << "\n";
            
            if (delta > threshold &&
                delta < upper_bound &&
                now - lastTriggerTime > cooldown)
            {
                slapCounter++;
                
                int my_int = distr(gen);
                std::string s = std::format("{:02}", my_int);
                fs::path dir = "./audio/sexy/";
                fs::path fullPath = dir / s;
                fullPath += ".mp3";
                std::string finalPath = fullPath.string();
                playSound(fullPath.c_str());
                
                std::cout << slapCounter << " SLAP detected! delta=" << delta << " playing " << s << ".mp4\n";
                
                lastTriggerTime = now;
            }

            lastMagnitude = magnitude;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
