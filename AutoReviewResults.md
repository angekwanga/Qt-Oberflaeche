BEWERTUNG: 99

# Auswertung der Abgabe

- Status: Abgabe wurde bewertet
- Punkte: **99** von **100**

## Test 1

- Test: *Erforderliche Dateien sind vorhanden*
- Beschreibung: Das erforderliche Makefile muss vorhanden sein.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.000016
##### Testschritte
- Makefile soll vorhanden sein: OK

## Test 2

- Test: *Qt-Projektdatei ist vorhanden*
- Beschreibung: Eine Projektdatei für das Qt-Projekt zum erstellen mit qmake ist vorhanden.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.000291
- Punkte: **1**
##### Testschritte
- Für ./**/*.pro wurde ./Uebung05.pro gefunden: OK

## Test 3

- Test: *Makefile-Target autotest existiert*
- Beschreibung: Das Makefile besitzt ein Target zum kompilieren der Tests.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.000079
##### Testschritte
- Inhalt von Makefile prüfen: OK

## Test 4

- Test: *tester.cpp-Datei ist unverändert*
- Beschreibung: Die Datei tester.cpp wurde nicht verändert und ist genau so wie im Original-Repository.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.000930
##### Testschritte
- Hash-Test von tester.cpp auf ['bfb51d75114c6432faa78b69c397273aaaff2ee4']: OK

## Test 5

- Test: *Kompilieren der Tests*
- Beschreibung: Mit dem Befehl `make autotest` können die Tests kompiliert werden.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:28.941310
- Punkte: **1**
- Return-Code / Fehlercode: `0`
- Kommandozeile: `make autotest`
##### Testschritte
- Rückgabe-Code ist `0`: OK

##### Ausgabe

```g++ -I. -I/usr/local/include -std=c++17 -Wall -Wextra -c network.cpp -o network.o
g++ -I. -I/usr/local/include -std=c++17 -Wall -Wextra -c csv.cpp -o csv.o
g++ -I. -I/usr/local/include -std=c++17 -Wall -Wextra -c scheduled_trip.cpp -o scheduled_trip.o
g++ -I. -I/usr/local/include -std=c++17 -Wall -Wextra -o test_runner /usr/local/lib/libgtest_main.a /usr/local/lib/libgtest.a tester.cpp network.cpp csv.cpp scheduled_trip.cpp -lpthread
./test_runner
Running main() from /usr/src/googletest/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from Network
[ RUN      ] Network.getStopsForTransfer
[       OK ] Network.getStopsForTransfer (7941 ms)
[ RUN      ] Network.getTravelPlanDepartingAt
[       OK ] Network.getTravelPlanDepartingAt (13957 ms)
[----------] 2 tests from Network (21898 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (21898 ms total)
[  PASSED  ] 2 tests.
```

##### Fehlerausgabe

```tester.cpp: In member function 'virtual void {anonymous}::Network_getTravelPlanDepartingAt_Test::TestBody()':
tester.cpp:91:29: warning: comparison of integer expressions of different signedness: 'int' and 'std::vector<bht::SStopTime>::size_type' {aka 'long unsigned int'} [-Wsign-compare]
   91 |   for (int index = 1; index < results.size(); index++) {
      |                       ~~~~~~^~~~~~~~~~~~~~~~
```

## Test 6

- Test: *Ausführbare Anwendung der Unit-Tests wurde erstellt*
- Beschreibung: Die Anwendung wird mit Unit-Tests kompiliert und erzeugt die geforderte Datei `/repo/test_runner`.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.000043
##### Testschritte
- test_runner soll vorhanden sein: OK

## Test 7

- Test: *Ausführbare Anwendung der Unit-Tests enthält die Testfälle*
- Beschreibung: Die geforderte Datei `/repo/test_runner` enthält die `tester.cpp`-Datei.
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.334010
- Return-Code / Fehlercode: `0`
- Kommandozeile: `readelf -s --wide /repo/test_runner`
##### Testschritte
- Ausgabe ist korrekt: OK
- Rückgabe-Code ist `0`: OK

##### Ausgabe

```<AUSGABE WIRD NICHT ANGEZEIGT>
```

## Test 8

- Test: *Testfälle werden korrekt ausgeführt*
- Beschreibung: Die Unit-Tests der Anwendung werden korrekt ausgeführt (max. Laufzeit 60 Sekunden).
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:21.989088
- Punkte: **96**
- Return-Code / Fehlercode: `0`
- Kommandozeile: `/repo/test_runner`
##### Testschritte
- Rückgabe-Code ist `0`: OK

##### Ausgabe

```Running main() from /usr/src/googletest/googletest/src/gtest_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from Network
[ RUN      ] Network.getStopsForTransfer
[       OK ] Network.getStopsForTransfer (7895 ms)
[ RUN      ] Network.getTravelPlanDepartingAt
[       OK ] Network.getTravelPlanDepartingAt (13820 ms)
[----------] 2 tests from Network (21715 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (21715 ms total)
[  PASSED  ] 2 tests.
```

## Test 9

- Test: *QMake kann ausgeführt werden*
- Beschreibung: qmake erzeugt ein Makefile für die Anwendung
- Status: ausgeführt
- Erfolgreich: **Ja**
- Laufzeit: 0:00:00.327135
- Punkte: **1**
- Return-Code / Fehlercode: `0`
- Kommandozeile: `/usr/bin/qmake6 /repo/./Uebung05.pro`
##### Testschritte
- Rückgabe-Code ist `0`: OK

##### Ausgabe

```Info: creating stash file /repo/build/.qmake.stash
```

##### Fehlerausgabe

```WARNING: Failure to find: config.h
WARNING: Failure to find: mainwindow.ui
```

## Test 10

- Test: *Erzeugtes Makefile kann die Anwendung erstellen*
- Beschreibung: Das erzeugte Makefile für die Anwendung kann kompiliert werden.
- Status: ausgeführt
- Erfolgreich: **Nein**
- Laufzeit: 0:00:00.286385
- Punkte: **0**
- Return-Code / Fehlercode: `2`
- Kommandozeile: `make`
##### Testschritte
- Rückgabe-Code ist `0`: fehlgeschlagen

##### Fehlerausgabe

```make: *** No rule to make target 'mainwindow.ui', needed by 'ui_mainwindow.h'.  Stop.
```

