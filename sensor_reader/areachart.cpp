
/*
 *******************************************************************************
 *> Include Files
 *******************************************************************************
 */

#include "areachart.h"

/*
 *******************************************************************************
 *> Global variables
 *******************************************************************************
 */

/*!
 * *******************************************************************************
 *
 * \fn AreaChart::AreaChart()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		Costruttore della classe AreaChart
 * \n
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

AreaChart::AreaChart()
{

    series0 = new QLineSeries();
    series1 = new QLineSeries();

    *series0 << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
                 << QPointF(16, 7) << QPointF(18, 5);

    *series1 << QPointF(1, 3) << QPointF(3, 4) << QPointF(7, 3) << QPointF(8, 2) << QPointF(12, 3)
                 << QPointF(16, 4) << QPointF(18, 3);


    area = new QAreaSeries(series0, series1);
    area->setName("Batman");

    QPen pen(0x059605);
    pen.setWidth(3);
    area->setPen(pen);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, 0x3cc63c);
    gradient.setColorAt(1.0, 0x26f626);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    area->setBrush(gradient);

    chart = new QChart();
    chart->addSeries(area);
    chart->setTitle("Simple areachart example");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 20);
    chart->axes(Qt::Vertical).first()->setRange(0, 10);
}

/*!
 * *******************************************************************************
 *
 * \fn AreaChart::~AreaChart()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		Costruttore della classe AreaChart
 * \n
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */
AreaChart::~AreaChart()
{
}
