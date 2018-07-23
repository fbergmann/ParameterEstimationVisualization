#include <QFileDialog>
#include <QString>
#include <QDebug>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parameterestimationutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotFileOpen()
{
    auto fileName = QFileDialog::getOpenFileName(this, "Load Protocol");
    loadFile(fileName);
}

void MainWindow::loadFile(QString & filename)
{
    if (filename.isEmpty())
    {
        slotFileOpen();
        return;
    }

    qDebug() << "FileName: " << filename;

	// set onto status bar
	this->setStatusTip(QString("Loaded: %1").arg(filename));

    // parse the result
    auto& results = ResultParser::fromFile(filename.toStdString());
    drawResults(results);

}

void MainWindow::drawResults(std::vector<ResultData*>& results)
{
    auto* scene = ui->graphicsView->scene();

    // create a new scene if we don't have one
    if (scene == NULL)
    {
        scene = new QGraphicsScene(ui->graphicsView);
    }

    // clear it
    scene->clear();

    // construct message to set into the scene
    QString message = QString("num results: %1").arg(results.size());

    for (auto& result : results)
    {
        qDebug() << "NumFittingItems: " << result->numFittingItems();
        qDebug() << "NumCheckPoints:  " << result->numCheckPoints();

        // add things to the message
        message += QString("\n\tNumFittingItems: %1").arg(result->numFittingItems());
        message += QString("\n\tNumCheckPoints:  %1").arg(result->numCheckPoints());

        // we go through all the items print their settings and all checkpoints
        int index = 0;
        for (auto& item : result->mFittingItems)
        {
            qDebug() << "Name:  " << QString::fromStdString(item->mName);
            qDebug() << "lower: " << item->mLowerBound;
            qDebug() << "upper: " << item->mUpperBound;
            qDebug() << "start: " << item->mStartValue;

            // go through all the checkpoints (the values found for the current parameter)
            for (auto& checkpoint : result->mCheckPoints)
            {
                qDebug() << "\t" << checkpoint->mParameters[index];
            }

            // increment index, so we access the next parameter on the following loop
            ++index;

        }
    }

    // set the text item onto the graphics scene
    auto* text = scene->addText(message);
    text->setPos(0, 0);

    // set the graphics scene
    ui->graphicsView->setScene(scene);
}
