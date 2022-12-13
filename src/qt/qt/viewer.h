#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <math.h>
#include <QString>
#include <QDialog>
#include <QColorDialog>
#include <QPainter>
#include <QFileInfo>


extern "C" {
#include "../../parser.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class Viewer; }
QT_END_NAMESPACE

class Viewer : public QMainWindow
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();

private slots:
    void on_pushButton_openfile_clicked();
    void on_horizontalSlider_sliderReleased();
    void on_pushButton_scale_clicked();
    void on_pushButton_return_clicked();
    void on_pushButton_move_clicked();
    void on_pushButton_rotate_clicked();
    void on_pushButton_rotateClear_clicked();
    void on_pushButton_moveClear_clicked();
    void on_pushButton_scaleClear_clicked();

private:
    f_node *f = NULL;
    matrix_t *v = NULL;
    int v_count = 0;
    int f_count = 0;
    int max_W = 1000;
    int max_H = 800;
    double scale = 50;
    double Xmax = -1000000, Ymax = -1000000, Zmax = -1000000;
    QPen pen_line;
    void drawLines();
    void get_str(char *input_str, QString src);
private:
    Ui::Viewer *ui;
    QGraphicsScene *scene;
};
#endif // VIEWER_H
