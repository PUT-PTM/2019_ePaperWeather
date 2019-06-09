# 2019_ePaperWeather

Overview:
  Jest to projekt przenośnej stacji pogodowej, podaje ona na wyświetlaczu ePapierowym pomiary temperatury,
ciśnienia oraz wilgotności otoczenia.

Description:
  Stacja pogodowa wykonana jest przy użyciu płytki z mikrokontrolerem STM32F407G.
Opórcz tego użyte zostały następujące peryferia: czujnik temperatury i wilgotności DHT11, czujnik ciśnienia BMP280
oraz wyświetlacz epapierowy Waveshare E-paper E-Ink 1,54" 200x200px.
Projekt całkowicie mieści się w pamięci mikrokontrolera oraz poza czujnikami nie potrzebuje innych narzędzi do prawidłowego działania.

Tools:
  Przy tworzeniu projektu użyte zostały następujące narzędzia:
Środowisko System Workbench for STM32 (wersja 1.16.0.201807130628) będące modyfikacją do środkowiska Eclipse (wersja 4.6.3).
Narzędzie Cube MX (wersja 4.1.0.0) służące do generowania projektów w języku C
z gotową konfiguracją danych peryferiów mikrokontrolera.

How to run:
  Ostateczna wersja Release znajduje się w folderze 2019_ePaperWeatherStation,
do uruchomienia potrzeba jedynie płytki z mikrokontrolerem oraz peryferiów.
Program nie zawiera funkcji interaktywnych, wystarczy obserwować wskazania ekranu.

How to compile:
  Aby uruchomić projekt wystarczy pobrać kod oraz skompilować i wgrać go do pamięci mikroprocesora.
Sugerowanym środowiskiem do tego celu jest wspomniane wyżej System Workbench for STM32.

Future improvements:
  Projekt jest ukończony, na chwilę obecną nie jest planowany jego rozwój, a wszelkie błędy zostały wyeliminowane.

Attributions:
  Obsługa wyświetlacza epapierowego zrealizowana jest za pomocą biblioteki epd-library-stm32,
w wersji przeznaczonej dla tego konkretnego wyświetlacza, przy użyciu funkcji rysujących kształty geometryczne
oraz wypisujące napisy w konkretnych współrzędnych wyświetlacza. Autorem jest użytkownik soonuse, ponieżej link do biblioteki:
(https://github.com/soonuse/epd-library-stm32)
  Obsługa czujnika ciśnienia BMP280 została zrealizowana przy pomocy dedykowanej biblioteki dla BMP280 przeznaczonej dla płytek 
STM32. Biblioteka ta zawiera funkcje przeznaczone do odczytu danych z czujnika oraz ich konwersji na jednostki SI.
  Obsługa czujnika DHT11 została zrealizowana przy użyciu 2 funkcji: inicjującej urządzenie i dokonującej pomiaru danych.
Obie są zawarte w pliku main.c.

License:
  MIT.

Credits:
  Authors: Adrian Lorenc, Tomasz Musiałek
The project was conducted during the Microprocessor Lab course held by
the Institute of Control and Information Engineering, Poznan University of Technology.
Supervisor: Tomasz Mańkowski.
