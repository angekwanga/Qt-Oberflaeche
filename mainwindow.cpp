#include "mainwindow.h"
#include <QApplication>
#include <QTime>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , network(nullptr)
{
    // Initialize network with GTFS data
    QString dataPath = QDir::currentPath() + "/GTFSShort";
    if (!QDir(dataPath).exists()) {
        dataPath = QDir::currentPath() + "/GTFSBbg";
    }
    
    try {
        network = new bht::Network(dataPath.toStdString());
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to load GTFS data: %1").arg(e.what()));
        return;
    }
    
    setupUI();
    populateStopComboBoxes();
    updateButtonState();
}

MainWindow::~MainWindow()
{
    delete network;
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    QLabel *titleLabel = new QLabel("Anwendung   Geben Sie Text ein", this);
    titleLabel->setAlignment(Qt::AlignLeft);
    titleLabel->setStyleSheet("font-size: 14px; font-weight: bold; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Input section in a frame
    QFrame *inputFrame = new QFrame(this);
    inputFrame->setFrameStyle(QFrame::Box);
    inputFrame->setStyleSheet("QFrame { border: 1px solid #ccc; background-color: #f9f9f9; }");
    QVBoxLayout *frameLayout = new QVBoxLayout(inputFrame);
    frameLayout->setSpacing(15);
    frameLayout->setContentsMargins(15, 15, 15, 15);
    
    // Start stop selection
    startLabel = new QLabel("Station für die Abfahrt auswählen:", this);
    startStopCombo = new QComboBox(this);
    startStopCombo->setEnabled(false);
    startStopCombo->setMinimumHeight(25);
    frameLayout->addWidget(startLabel);
    frameLayout->addWidget(startStopCombo);
    
    // Destination stop selection
    destLabel = new QLabel("Ziel auswählen:", this);
    destStopCombo = new QComboBox(this);
    destStopCombo->setEnabled(false);
    destStopCombo->setMinimumHeight(25);
    frameLayout->addWidget(destLabel);
    frameLayout->addWidget(destStopCombo);
    
    // Departure time
    timeLabel = new QLabel("Gewünschte Abfahrtszeit:", this);
    departureTimeEdit = new QTimeEdit(this);
    departureTimeEdit->setTime(QTime(0, 0, 0));
    departureTimeEdit->setDisplayFormat("hh:mm");
    departureTimeEdit->setEnabled(false);
    departureTimeEdit->setMinimumHeight(25);
    frameLayout->addWidget(timeLabel);
    frameLayout->addWidget(departureTimeEdit);
    
    // Calculate button
    calculateButton = new QPushButton("Route berechnen", this);
    calculateButton->setEnabled(false);
    calculateButton->setMinimumHeight(35);
    calculateButton->setStyleSheet("QPushButton { background-color: #e0e0e0; border: 1px solid #999; } QPushButton:enabled { background-color: #ffffff; } QPushButton:disabled { background-color: #f0f0f0; color: #999; }");
    frameLayout->addWidget(calculateButton);
    
    mainLayout->addWidget(inputFrame);
    
    // Route section
    QLabel *routeLabel = new QLabel("Ihre Route:", this);
    routeLabel->setStyleSheet("font-weight: bold; margin-top: 10px;");
    mainLayout->addWidget(routeLabel);
    
    // Route table
    routeTable = new QTableWidget(this);
    routeTable->setColumnCount(4);
    QStringList headers;
    headers << "Station" << "Ankunftszeit" << "Abfahrtszeit" << "Fahrt ID";
    routeTable->setHorizontalHeaderLabels(headers);
    routeTable->horizontalHeader()->setStretchLastSection(true);
    routeTable->setAlternatingRowColors(true);
    routeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    routeTable->setFrameStyle(QFrame::Box);
    routeTable->setStyleSheet("QTableWidget { border: 1px solid #ccc; }");
    mainLayout->addWidget(routeTable);
    
    // Connect signals
    connect(startStopCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onStartStopChanged);
    connect(destStopCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onDestinationStopChanged);
    connect(departureTimeEdit, &QTimeEdit::timeChanged, this, &MainWindow::onTimeChanged);
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::onCalculateRoute);
    
    // Set window properties
    setWindowTitle("GTFS Route Planner - Übungsblatt 5");
    setMinimumSize(800, 600);
}

void MainWindow::populateStopComboBoxes()
{
    if (!network) return;
    
    // Get all stops and sort them by name
    for (const auto& stopPair : network->stops) {
        if (stopPair.second.locationType == bht::LocationType_Stop) {
            allStops.push_back(stopPair.second);
        }
    }
    
    std::sort(allStops.begin(), allStops.end(), [](const bht::Stop& a, const bht::Stop& b) {
        return a.name < b.name;
    });
    
    // Populate combo boxes
    startStopCombo->clear();
    destStopCombo->clear();
    
    startStopCombo->addItem("", "");
    destStopCombo->addItem("", "");
    
    for (const auto& stop : allStops) {
        QString displayName = QString::fromStdString(stop.name);
        QString stopId = QString::fromStdString(stop.id);
        
        startStopCombo->addItem(displayName, stopId);
        destStopCombo->addItem(displayName, stopId);
    }
    
    startStopCombo->setEnabled(true);
}

void MainWindow::updateButtonState()
{
    bool startSelected = startStopCombo->currentIndex() > 0;
    bool destSelected = destStopCombo->currentIndex() > 0;
    bool timeValid = departureTimeEdit->time().isValid();
    
    destStopCombo->setEnabled(startSelected);
    departureTimeEdit->setEnabled(startSelected && destSelected);
    calculateButton->setEnabled(startSelected && destSelected && timeValid);
}

void MainWindow::onStartStopChanged()
{
    updateButtonState();
}

void MainWindow::onDestinationStopChanged()
{
    updateButtonState();
}

void MainWindow::onTimeChanged()
{
    updateButtonState();
}

void MainWindow::onCalculateRoute()
{
    if (!network) return;
    
    QString startStopId = startStopCombo->currentData().toString();
    QString destStopId = destStopCombo->currentData().toString();
    QTime departureTime = departureTimeEdit->time();
    
    if (startStopId.isEmpty() || destStopId.isEmpty()) {
        displayError("Bitte wählen Sie sowohl Start- als auch Zielstation aus.");
        return;
    }
    
    if (startStopId == destStopId) {
        displayError("Start- und Zielstation können nicht identisch sein.");
        return;
    }
    
    // Convert QTime to GTFSTime
    bht::GTFSTime gtfsTime;
    gtfsTime.hour = departureTime.hour();
    gtfsTime.minute = departureTime.minute();
    gtfsTime.second = departureTime.second();
    
    try {
        std::vector<bht::StopTime> route = network->getTravelPlanDepartingAt(
            startStopId.toStdString(), 
            destStopId.toStdString(), 
            gtfsTime
        );
        
        if (route.empty()) {
            displayError("Keine Route zwischen den gewählten Stationen zur angegebenen Zeit gefunden.");
            routeTable->setRowCount(0);
        } else {
            displayRoute(route);
        }
    } catch (const std::exception& e) {
        displayError(QString("Fehler bei der Routenberechnung: %1").arg(e.what()));
    }
}

void MainWindow::displayRoute(const std::vector<bht::StopTime>& route)
{
    routeTable->setRowCount(route.size());
    
    for (size_t i = 0; i < route.size(); ++i) {
        const bht::StopTime& stopTime = route[i];
        
        // Stop name
        QString stopName;
        try {
            bht::Stop stop = network->getStopById(stopTime.stopId);
            stopName = QString::fromStdString(stop.name);
        } catch (...) {
            stopName = QString::fromStdString(stopTime.stopId);
        }
        
        // Arrival time
        QString arrivalTime = formatTime(stopTime.arrivalTime);
        
        // Departure time
        QString departureTime = formatTime(stopTime.departureTime);
        
        // Trip ID
        QString tripId = QString::fromStdString(stopTime.tripId);
        
        routeTable->setItem(i, 0, new QTableWidgetItem(stopName));
        routeTable->setItem(i, 1, new QTableWidgetItem(arrivalTime));
        routeTable->setItem(i, 2, new QTableWidgetItem(departureTime));
        routeTable->setItem(i, 3, new QTableWidgetItem(tripId));
    }
    
    routeTable->resizeColumnsToContents();
}

void MainWindow::displayError(const QString& message)
{
    QMessageBox::warning(this, "Routenberechnung", message);
}

QString MainWindow::formatTime(const bht::GTFSTime& time)
{
    return QString("%1:%2:%3")
        .arg(time.hour, 2, 10, QChar('0'))
        .arg(time.minute, 2, 10, QChar('0'))
        .arg(time.second, 2, 10, QChar('0'));
}