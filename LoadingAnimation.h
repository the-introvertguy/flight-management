#ifndef LOADINGANIMATION_H
#define LOADINGANIMATION_H

#include <chrono>
#include <iostream>


class LoadingAnimation {
public:
  LoadingAnimation(int duration)
      : duration(duration), colorIndex(0), spinnerIndex(0) {}

  void start() {
    clearConsole();
    std::cout << "Loading... Please wait." << std::endl;
    animate();
    std::cout << "Loading complete!" << std::endl;
  }

private:
  int duration;
  int colorIndex;
  int spinnerIndex;
  const std::string colors[7] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m",
                                 "\033[35m", "\033[36m", "\033[37m"};
  const char spinner[4] = {'|', '/', '-', '\\'};

  void clearConsole() const { std::cout << "\033[2J\033[1;1H"; }

  void delay(int milliseconds) const {
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
               .count() < milliseconds) {
      end = std::chrono::high_resolution_clock::now();
    }
  }

  void animate() {
    auto start = std::chrono::steady_clock::now();
    while (true) {

      auto now = std::chrono::steady_clock::now();
      auto elapsed =
          std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

      if (elapsed >= duration)
        break;

      std::cout << colors[colorIndex % 7] << spinner[spinnerIndex % 4]
                << "\033[0m" << std::flush;

      colorIndex++;
      spinnerIndex++;

      delay(200);

      std::cout << "\r";
    }
    system("cls");
    std::cout << "\033[0m" << std::endl;
  }
};

#endif