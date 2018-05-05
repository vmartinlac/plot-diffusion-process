#pragma once

#include <QWidget>
#include <QString>
#include <iostream>

class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QCustomPlot;

class Window : public QWidget
{
    Q_OBJECT
public:
    Window();
    QString expressionForDrift();
    QString expressionForDiffusion();
    double endTime();
    double initialState();
    int numTimesteps();
    int numTrajectories();
    int seed();
protected slots:
    void update();
protected:
    QLineEdit* _drift;
    QLineEdit* _diffusion;
    QDoubleSpinBox* _endTime;
    QDoubleSpinBox* _initialState;
    QSpinBox* _numTimesteps;
    QSpinBox* _numTrajectories;
    QCustomPlot* _plot;
    QSpinBox* _seed;
};

