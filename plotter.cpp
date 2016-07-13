#include "plotter.h"
#include "ui_plotter.h"

#include <QTextStream>

plotter::plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotter)
{
    ui->setupUi(this);

    view = new QChartView(this);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout->addWidget(view);
    view->chart()->setTheme(QChart::ChartThemeQt);    // Установка темы QChartView
    view->setRenderHint(QPainter::Antialiasing);

    showT = true;
    showH = true;
    xStep = 1000;
    timeWindow = 600;
    autoY = true;

    initPlot();

   // view->viewport()->setAttribute(Qt::WA_AcceptTouchEvents, true);
   // view->chart()->setAcceptTouchEvents(true);
    this->setAttribute(Qt::WA_AcceptTouchEvents, true);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);

}

plotter::~plotter()
{
    delete ui;
}

void plotter::initPlot()
{



    xMin=0;
    xMax=xMin+timeWindow;
    xZeroValue = QDateTime::currentDateTime();

    xTimeAxis = new QDateTimeAxis;
    xTimeAxis->setFormat("hh:mm:ss");
    xTimeAxis->setRange(xZeroValue, xZeroValue.addSecs(timeWindow));

    xAxis = new QValueAxis;
    xAxis->setRange(xMin, xMax);
    xAxis->setVisible(false);

    yAxis = new QValueAxis;
    if(!autoY)
        yAxis->setRange(yMin, yMax);
    else
    {
        yAxis->setRange(0, 30);
        yMaxValue=30;
        yMinValue=1;
        yMin = 1;
        yMax = 30;
    }

    channelT.setWidthF(3);
    channelT.setBrush(Qt::red);

    channelH.setWidthF(3);
    channelH.setBrush(Qt::blue);
    channelH.setJoinStyle(Qt::RoundJoin);

    diagramT = new QLineSeries;
    diagramT->setName(tr("Температура"));
    diagramT->setPen(channelT);
    diagramT->setVisible(showT);
    //diagramT->setUseOpenGL(true);               // Включить поддержку OpenGL

    diagramH = new QLineSeries;
    diagramH->setName(tr("Влажность"));
    diagramH->setPen(channelH);
    diagramH->setVisible(showH);
    //diagramH->setUseOpenGL(true);               // Включить поддержку OpenGL

    view->chart()->addSeries(diagramT);         // Добавить на график
    view->chart()->setAxisX(xAxis, diagramT);   // Назначить ось xAxis, осью X для diagramA
    view->chart()->setAxisY(yAxis, diagramT);
    view->chart()->addSeries(diagramH);         // Добавить на график
    view->chart()->setAxisX(xAxis, diagramH);   // Назначить ось xAxis, осью X для diagramA
    view->chart()->addAxis(xTimeAxis, Qt::AlignBottom);
    view->chart()->setAxisY(yAxis, diagramH);   // Назначить ось yAxis, осью Y для diagramA

    connect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::addDataT(double t)
{
    if(autoY)
    {
        bool needRescale=false;
        if(t < yMinValue) { yMinValue=t; needRescale=true; }
        if(t > yMaxValue) { yMaxValue=t; needRescale=true; }
        if(needRescale)
        {
            yAxis->setRange(yMinValue-1, yMaxValue+1);
            yMin = yMinValue-1;
            yMax = yMaxValue+1;
        }
    }

   int lastIndex = diagramT->count()-1;
   int currentXValue=0;

   if(lastIndex != -1)
   {
       currentXValue = diagramT->at(lastIndex).x() + xStep/1000;
   }

   diagramT->append(currentXValue, t);
   diagramT->setName(tr("Температура: ") + QString::number(t, 'f', 2));

   if(currentXValue > xMax)
   {
//       xMin+= timeWindow/4;
//       xMax= xMin + timeWindow;

//       xAxis->setRange(xMin, xMin + timeWindow);
//       xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMin + timeWindow));
       emit onlineIntervalEnded();
   }

}

void plotter::addDataH(double h)
{
    if(autoY)
    {
        bool needRescale=false;
        if(h < yMinValue) { yMinValue=h; needRescale=true; }
        if(h > yMaxValue) { yMaxValue=h; needRescale=true; }
        if(needRescale)
        {
            yAxis->setRange(yMinValue-1, yMaxValue+1);
            yMin = yMinValue-1;
            yMax = yMaxValue+1;
        }
    }

   int lastIndex = diagramH->count()-1;
   int currentXValue=0;

   if(lastIndex != -1)
   {
       currentXValue = diagramH->at(lastIndex).x() + xStep/1000;
   }

   diagramH->append(currentXValue, h);
   diagramH->setName(tr("Влажность: ") + QString::number(h, 'f', 2));

   if(currentXValue > xMax)
   {
//       xMin+= timeWindow/4;
//       xMax= xMin + timeWindow;

//       xAxis->setRange(xMin, xMin + timeWindow);
//       xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMin + timeWindow));
       emit onlineIntervalEnded();
   }

}

void plotter::setTimeRange(int sec)
{
    timeWindow = sec;
    xMax = xMin + timeWindow;
    xAxis->setRange(xMin, xMax);
    xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMax));
}

void plotter::setTimeStep(int msec)
{
    xStep = msec;
}

void plotter::setValuesRange(int min, int max)
{
    if(autoY) return;

    yMin = min;
    yMax = max;
    yAxis->setRange(min, max);
}

void plotter::setAutoYScale(bool autoYScale)
{
    autoY = autoYScale;
}


bool plotter::getAutoY()
{
    return autoY;
}

int plotter::getMinY()
{
    return yMin;
}

int plotter::getMaxY()
{
    return yMax;
}

int plotter::getTimeRange()
{
    return timeWindow;
}

int plotter::getXMin()
{
    return xMin;
}

QDateTime plotter::getXZeroValue()
{
    return xZeroValue;
}

void plotter::showInterval(QDateTime start, QDateTime end)
{

    int min, max;
    min = xZeroValue.secsTo(start);
    max = xZeroValue.secsTo(end);

    xAxis->setRange(min, max);
    xTimeAxis->setRange(start, end);

    disconnect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::updateOnlineInterval()
{

     xMin+= timeWindow/4;
     xMax= xMin + timeWindow;

     xAxis->setRange(xMin, xMax);
     xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMax));

}

void plotter::returnToOnline()
{
    xAxis->setRange(xMin, xMax);
    xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMax));
    connect(this, &plotter::onlineIntervalEnded, this, &plotter::updateOnlineInterval);
}

void plotter::toStart()
{
    showInterval(xZeroValue, xZeroValue.addSecs(timeWindow));
}

void plotter::toEnd()
{
    int lastPoint = diagramT->at(diagramT->count()-1).x();
    showInterval(xZeroValue.addSecs(lastPoint - timeWindow), xZeroValue.addSecs(lastPoint));
}

void plotter::screenBack()
{
    //QDateTimeAxis *timeAxis = (QDateTimeAxis*)view->chart()->axisX(diagramT);
    showInterval(xTimeAxis->min().addSecs((-1)*timeWindow), xTimeAxis->min());
}

void plotter::screenForward()
{
    //QDateTimeAxis *timeAxis = (QDateTimeAxis*)view->chart()->axisX(diagramT);
    showInterval(xTimeAxis->max(), xTimeAxis->max().addSecs(timeWindow));
}

void plotter::setGraphicsVisible(bool t, bool h)
{
    showT = t;
    showH = h;
    diagramT->setVisible(t);
    diagramH->setVisible(h);
}

bool plotter::getTVisible()
{
    return showT;
}

bool plotter::getHVisible()
{
    return showH;
}

void plotter::startNewPlot()
{
    xZeroValue = QDateTime::currentDateTime();
    diagramT->clear();
    diagramH->clear();

    xMin = 0;
    xMax = xMin + timeWindow;

    xAxis->setRange(xMin, xMax);
    xTimeAxis->setRange(xZeroValue.addSecs(xMin), xZeroValue.addSecs(xMax));
    if(!autoY)
        yAxis->setRange(yMin, yMax);
    else
    {
        yAxis->setRange(0, 30);
        yMaxValue=30;
        yMinValue=1;
    }
}

bool plotter::event(QEvent *event)
{
    QTextStream err(stderr);
    //err << tr("Connect failed: ") + event->type().;

    if (event->type() == QEvent::Gesture)
    {
        err << tr("\nQEvent type = Gesture");
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

bool plotter::gestureEvent(QGestureEvent *event)
{
    QTextStream err(stderr);
    if(QGesture *swipe = event->gesture(Qt::SwipeGesture))
    {
        err << tr("\nGesture type = swipe");
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    }
    if(QGesture *pan = event->gesture(Qt::PanGesture))
    {
         err << tr("\nGesture type = pan");
        panTriggered(static_cast<QPanGesture *>(pan));
    }
    if(QGesture *pinch = event->gesture(Qt::PinchGesture))
    {
         err << tr("\nGesture type = pinch");
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    }
    return true;
}

void plotter::swipeTriggered(QSwipeGesture *gesture)
{
    if(gesture->state() == Qt::GestureFinished)
    {
        if(gesture->horizontalDirection() == QSwipeGesture::Left)
            this->screenForward();
        if(gesture->horizontalDirection() == QSwipeGesture::Right)
            this->screenBack();
        update();
    }
}

void plotter::panTriggered(QPanGesture *gesture)
{

}

void plotter::pinchTriggered(QPinchGesture *gesture)
{

}
