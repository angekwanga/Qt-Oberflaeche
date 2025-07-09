#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QHeaderView>
#include "network.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStopChanged();
    void onDestinationStopChanged();
    void onTimeChanged();
    void onCalculateRoute();

private:
    void setupUI();
    void populateStopComboBoxes();
    void updateButtonState();
    void displayRoute(const std::vector<bht::StopTime>& route);
    void displayError(const QString& message);
    QString formatTime(const bht::GTFSTime& time);

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *inputLayout;
    QHBoxLayout *startLayout;
    QHBoxLayout *destLayout;
    QHBoxLayout *timeLayout;
    QHBoxLayout *buttonLayout;
    
    QLabel *startLabel;
    QComboBox *startStopCombo;
    QLabel *destLabel;
    QComboBox *destStopCombo;
    QLabel *timeLabel;
    QTimeEdit *departureTimeEdit;
    QPushButton *calculateButton;
    QTableWidget *routeTable;
    QLabel *statusLabel;

    // Network data
    bht::Network *network;
    std::vector<bht::Stop> allStops;
};

#endif // MAINWINDOW_H