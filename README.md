# 2019_ePaperWeather

Jest to projekt stacji pogodowej, wykonany przy użyciu płytki z mikrokontrolerem STM32F407G.
Opórcz tego użyte zostały następujące peryferia: czujnik temperatury i wilgotności DHT11, czujnij ciśnienia BMP280
oraz wyświetlacz epapierowy Waveshare E-paper E-Ink 1,54" 200x200px.

Obsługa wyświetlacza epapierowego zrealizowana jest za pomocą biblioteki epd-library-stm32,
w wersji przeznaczonej dla tego konkretnego wyświetlacza, przy użyciu funkcji rysujących kształty geometryczne
oraz wypisujące napisy w konkretnych współrzędnych wyświetlacza.

Obsługa czujnika ciśnienia BMP280 została zrealizowana przy pomocy dedykowanej biblioteki dla BMP280 przeznaczonej dla płytek 
STM32. Biblioteka ta zawiera funkcje przeznaczone do odczytu danych z czujnika oraz ich konwersji na jednostki SI.

Obsługa czujnika DHT11 została zrealizowana przy użyciu 2 funkcji: inicjującej urządzenie i dokonującej pomiaru danych.
Obie są zawarte w pliku main.c.

