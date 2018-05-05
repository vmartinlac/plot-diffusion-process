#include <QSpinBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <iostream>
#include <random>
#include <vector>
#include "muParser.h"
#include "qcustomplot.h"
#include "Window.h"

Window::Window()
{
    QVBoxLayout* lay = new QVBoxLayout();
    _initialState = new QDoubleSpinBox();
    _drift = new QLineEdit(this);
    _diffusion = new QLineEdit(this);
    QFormLayout* form = new QFormLayout;
    QPushButton* btn = new QPushButton("Compute");
    _numTimesteps = new QSpinBox();
    _endTime = new QDoubleSpinBox();
    _numTrajectories = new QSpinBox();
    _plot = new QCustomPlot();
    _seed = new QSpinBox();

    _endTime->setMinimum(1.0e-4);
    _endTime->setMaximum(1.0e4);
    _endTime->setValue(1.0);
    _endTime->setDecimals(4);

    _initialState->setMinimum(-1.0e4);
    _initialState->setMaximum(1.0e4);
    _initialState->setValue(0.0);
    _initialState->setDecimals(4);

    _numTimesteps->setMaximum(100000);
    _numTimesteps->setMinimum(2);
    _numTimesteps->setValue(500);

    _seed->setMaximum(100000);
    _seed->setMinimum(0);
    _seed->setValue(1);

    _numTrajectories->setMaximum(100000);
    _numTrajectories->setMinimum(1);
    _numTrajectories->setValue(1);

    _drift->setText("1");
    _diffusion->setText("1");

    _plot->setMinimumSize(420, 300);

    form->addRow("X0 = ", _initialState);
    form->addRow("drift(t,x) = ", _drift);
    form->addRow("diffusion(t,x) = ", _diffusion);
    form->addRow("T = ", _endTime);
    form->addRow("N = ", _numTimesteps);
    form->addRow("K = ", _numTrajectories);
    form->addRow("seed = ", _seed);
    lay->addLayout(form);
    lay->addWidget(btn);
    lay->addWidget(_plot, 1);
    setLayout(lay);

    connect(btn, &QPushButton::clicked, this, &Window::update);

    setWindowTitle("Sample and Plot 1d Diffusion Processes");
}

void Window::update()
{
    const double x0 = initialState();
    const double T = endTime();
    const int N = numTimesteps();
    const double dt = T / double(N-1);
    const int K = numTrajectories();

    std::default_random_engine engine;
    std::normal_distribution<double> normal;

    std::default_random_engine color_engine;
    std::uniform_int_distribution<int> color_distrib(0, 255);

    double state;
    double time;

    engine.seed( seed() );

    mu::Parser pdrift;
    pdrift.DefineVar("x", &state);
    pdrift.DefineVar("t", &time);
    pdrift.SetExpr(expressionForDrift().toLocal8Bit().data());

    mu::Parser pdiffusion;
    pdiffusion.DefineVar("x", &state);
    pdiffusion.DefineVar("t", &time);
    pdiffusion.SetExpr(expressionForDiffusion().toLocal8Bit().data());

    // check that parser is ready.
    try
    {
        time = 0.0;
        state = x0;
        pdiffusion.Eval();
        pdrift.Eval();
    }
    catch(const mu::ParserError& e)
    {
        QMessageBox::critical(this, "Error", "Expression for drift and/or diffusion is incorrect !");
        return;
    }

    std::vector<double> X(N);

    double xmin = x0;
    double xmax = x0;

    _plot->clearGraphs();

    for(int i=0; i<K; i++)
    {
        QVector<double> t(N);
        QVector<double> x(N);
        t.front() = 0.0;
        x.front() = x0;

        for(int i=1; i<N; i++)
        {
            state = x[i-1];
            time = T * double(i-1) / double(N-1);
            t[i] = T * double(i) / double(N-1);
            x[i] =
                state +
                pdrift.Eval()*dt +
                pdiffusion.Eval()*normal(engine)*std::sqrt(dt);
            xmin = std::min(xmin, x[i]);
            xmax = std::max(xmax, x[i]);
        }

        QCPGraph* g = _plot->addGraph();
        g->setPen(QColor(
            color_distrib(color_engine),
            color_distrib(color_engine),
            color_distrib(color_engine)
        ));
        g->setData(t, x);
    }

    const double margin = 0.1*(xmax - xmin);
    xmin -= margin;
    xmax += margin;

    _plot->xAxis->setRange(0.0, T);
    _plot->yAxis->setRange(xmin, xmax);
    _plot->replot();

    _seed->setValue( (_seed->value() + 1) % _seed->maximum() );
}

QString Window::expressionForDrift()
{
    return _drift->text();
}

QString Window::expressionForDiffusion()
{
    return _diffusion->text();
}

double Window::endTime()
{
    return _endTime->value();
}

double Window::initialState()
{
    return _initialState->value();
}

int Window::numTimesteps()
{
    return _numTimesteps->value();
}

int Window::numTrajectories()
{
    return _numTrajectories->value();
}

int Window::seed()
{
    return _seed->value();
}

