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
    
    // Title
    QLabel *titleLabel = new QLabel("GTFS Route Planner", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Input section
    inputLayout = new QVBoxLayout();
    
    // Start stop selection
    startLayout = new QHBoxLayout();
    startLabel = new QLabel("Start Station:", this);
    startLabel->setMinimumWidth(120);
    startStopCombo = new QComboBox(this);
    startStopCombo->setEnabled(false);
    startLayout->addWidget(startLabel);
    startLayout->addWidget(startStopCombo);
    inputLayout->addLayout(startLayout);
    
    // Destination stop selection
    destLayout = new QHBoxLayout();
    destLabel = new QLabel("Destination:", this);
    destLabel->setMinimumWidth(120);
    destStopCombo = new QComboBox(this);
    destStopCombo->setEnabled(false);
    destLayout->addWidget(destLabel);
    destLayout->addWidget(destStopCombo);
    inputLayout->addLayout(destLayout);
    
    // Departure time
    timeLayout = new QHBoxLayout();
    timeLabel = new QLabel("Departure Time:", this);
    timeLabel->setMinimumWidth(120);
    departureTimeEdit = new QTimeEdit(this);
    departureTimeEdit->setTime(QTime::currentTime());
    departureTimeEdit->setEnabled(false);
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(departureTimeEdit);
    inputLayout->addLayout(timeLayout);
    
    // Calculate button
    buttonLayout = new QHBoxLayout();
    calculateButton = new QPushButton("Calculate Route", this);
    calculateButton->setEnabled(false);
    calculateButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 10px; font-weight: bold; } QPushButton:disabled { background-color: #cccccc; }");
    buttonLayout->addStretch();
    buttonLayout->addWidget(calculateButton);
    buttonLayout->addStretch();
    inputLayout->addLayout(buttonLayout);
    
    mainLayout->addLayout(inputLayout);
    
    // Status label
    statusLabel = new QLabel("Select start and destination stations", this);
    statusLabel->setStyleSheet("color: #666; font-style: italic; margin: 10px;");
    mainLayout->addWidget(statusLabel);
    
    // Route table
    routeTable = new QTableWidget(this);
    routeTable->setColumnCount(4);
    QStringList headers;
    headers << "Stop" << "Arrival Time" << "Departure Time" << "Trip ID";
    routeTable->setHorizontalHeaderLabels(headers);
    routeTable->horizontalHeader()->setStretchLastSection(true);
    routeTable->setAlternatingRowColors(true);
    routeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    
    startStopCombo->addItem("-- Select Start Station --", "");
    destStopCombo->addItem("-- Select Destination --", "");
    
    for (const auto& stop : allStops) {
        QString displayName = QString::fromStdString(stop.name);
        QString stopId = QString::fromStdString(stop.id);
        
        startStopCombo->addItem(displayName, stopId);
        destStopCombo->addItem(displayName, stopId);
    }
    
    startStopCombo->setEnabled(true);
    statusLabel->setText("Select start station");
}

void MainWindow::updateButtonState()
{
    bool startSelected = startStopCombo->currentIndex() > 0;
    bool destSelected = destStopCombo->currentIndex() > 0;
    bool timeValid = departureTimeEdit->time().isValid();
    
    destStopCombo->setEnabled(startSelected);
    departureTimeEdit->setEnabled(startSelected && destSelected);
    calculateButton->setEnabled(startSelected && destSelected && timeValid);
    
    if (startSelected && !destSelected) {
        statusLabel->setText("Select destination station");
    } else if (startSelected && destSelected && !timeValid) {
        statusLabel->setText("Select departure time");
    } else if (startSelected && destSelected && timeValid) {
        statusLabel->setText("Ready to calculate route");
    } else {
        statusLabel->setText("Select start station");
    }
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
        displayError("Please select both start and destination stations.");
        return;
    }
    
    if (startStopId == destStopId) {
        displayError("Start and destination stations cannot be the same.");
        return;
    }
    
    // Convert QTime to GTFSTime
    bht::GTFSTime gtfsTime;
    gtfsTime.hour = departureTime.hour();
    gtfsTime.minute = departureTime.minute();
    gtfsTime.second = departureTime.second();
    
    try {
        statusLabel->setText("Calculating route...");
        QApplication::processEvents(); // Update UI
        
        std::vector<bht::StopTime> route = network->getTravelPlanDepartingAt(
            startStopId.toStdString(), 
            destStopId.toStdString(), 
            gtfsTime
        );
        
        if (route.empty()) {
            displayError("No route found between the selected stations at the specified time.");
            statusLabel->setText("No route found");
        } else {
            displayRoute(route);
            statusLabel->setText(QString("Route found with %1 stops").arg(route.size()));
        }
    } catch (const std::exception& e) {
        displayError(QString("Error calculating route: %1").arg(e.what()));
        statusLabel->setText("Error occurred");
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
    QMessageBox::warning(this, "Route Calculation Error", message);
}

QString MainWindow::formatTime(const bht::GTFSTime& time)
{
    return QString("%1:%2:%3")
        .arg(time.hour, 2, 10, QChar('0'))
        .arg(time.minute, 2, 10, QChar('0'))
        .arg(time.second, 2, 10, QChar('0'));
}