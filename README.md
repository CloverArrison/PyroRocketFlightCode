Flight Code for Pyro High Power Rocket
Made by Clover Arrison

I have been building a high-power rocket since June 2024, it has not flown yet but I am still working on it. This code aims to collect data from sensors and filter that data to give the computer the best prediction of its position, velocity, acceleration, and attitude. This data will be streamed back to a ground station via radio and tell the computer when the rocket is at apogee so it deploys the parachute at the right time.

Uses Arduino, made in Platform IO
My flight computer uses:
  Adafruit Adaloger Feather M0
  MPU6050 IMU
  MPL3115A2 Barometer
  Adafruit GPS
  LoRa 915 Radio
  SPI Flash Chip
  Three Pyro Chanles using N-Chanel Mosfets
  PWM Buzzer

The Rocket "Pyro"
<img width="1027" height="209" alt="image" src="https://github.com/user-attachments/assets/639c8afe-1507-4fe2-b6db-ddcef59b0fe3" />

The Flight Computer
<img width="949" height="949" alt="image" src="https://github.com/user-attachments/assets/b76bf399-d4d5-43f4-8a9d-8fc14ca16c92" />

