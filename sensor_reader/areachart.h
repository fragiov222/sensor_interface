#ifndef AREACHART_H
#define AREACHART_H



/*
 *******************************************************************************
 *> Include Files
 *******************************************************************************
 */

#include <QMainWindow>
#include <QWidget>
#include <QByteArray>
#include <QDebug>
#include <QChartView>
#include <QLineSeries>
#include <QAreaSeries>

/*
 *******************************************************************************
 *> Typedefs and Struct
 *******************************************************************************
 */

using namespace QtCharts;

class AreaChart:public QWidget
{
    Q_OBJECT

public:

    AreaChart();
    ~AreaChart();

signals:

public slots:

private:
    QLineSeries *series0;
    QLineSeries *series1;

    QAreaSeries *area;

    QChart *chart;

};



#endif // AREACHART_H
