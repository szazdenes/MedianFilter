#include "mainwindow.h"
#include "ui_mainwindow.h"

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

QImage MainWindow::getMedianFilteredImage(QStringList &imageList)
{
    QMap<QString, QList<double> > pixelValueRMap, pixelValueGMap, pixelValueBMap; //key: pixel position, value: list of pixelcolors

    foreach(QString currentImageName, imageList){
        QImage currentImage(currentImageName);
        for(int i = 0; i < currentImage.width(); i++){
            for(int j = 0; j < currentImage.height(); j++){
                QString index = QString::number(i) + " " + QString::number(j);
                pixelValueRMap[index].append(currentImage.pixelColor(i,j).red());
                pixelValueGMap[index].append(currentImage.pixelColor(i,j).green());
                pixelValueBMap[index].append(currentImage.pixelColor(i,j).blue());
            }
        }
    }

    QImage resultImage(imageList.at(0));
    resultImage.fill(Qt::white);

    foreach(QString key, pixelValueRMap.keys()){
        QPoint index;
        index.setX(QString(key.split(" ").first()).toInt());
        index.setY(QString(key.split(" ").last()).toInt());

        int r = qRound(getMedian(pixelValueRMap[key]));
        int g = qRound(getMedian(pixelValueGMap[key]));
        int b = qRound(getMedian(pixelValueBMap[key]));

        resultImage.setPixelColor(index, QColor(r, g, b));
    }

    return resultImage;
}

double MainWindow::getMedian(QList<double> &list)
{
    QList<double> currentList = list;
    qSort(list.begin(), list.end());
    int listSize = list.size();
    if(listSize % 2 == 0)
        return (double)(currentList.at(listSize/2.0) + currentList.at(listSize/2.0))/2.0;
    else
        return (double)currentList.at((listSize-1)/2.0);
}

void MainWindow::on_pushButton_clicked()
{
    QStringList imageList = QFileDialog::getOpenFileNames();
    QImage filteredImage = getMedianFilteredImage(imageList);
    filteredImage.save(QFileDialog::getSaveFileName());
}
