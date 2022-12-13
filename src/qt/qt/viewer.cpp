#include "viewer.h"
#include "../../parser.h"
#include "QString"
#include "ui_viewer.h"
#include <QColorDialog>
#include <QDialog>
#include <QPainter>
#include <math.h>

extern "C" {
#include "../../parser.h"
}

Viewer::Viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::Viewer) {
  ui->setupUi(this);
  scene = new QGraphicsScene();
  ui->graphicsView->setScene(scene);
  scene->setSceneRect(-(max_W / 2), -(max_H / 2), max_W, max_H);
  ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  ui->statusbar->showMessage("Open *.obj file ");
}

Viewer::~Viewer() {
  delete ui;
  free_all(&v, &f, v_count, f_count);
}

void Viewer::get_str(char *input_str, QString src) {
  QByteArray ba = src.toLocal8Bit();
  char *str = ba.data();
  int i;
  for (i = 0; i < 255 && *str; i++) {
    *input_str = *str;
    str++;
    input_str++;
  }
  *input_str = '\0';
}

void Viewer::drawLines() {
  scene->clear();
  ui->graphicsView->centerOn(0, 0);
  pen_line.setColor(QColorConstants::White);
  for (int i = 0; i < f_count; i++) {
    double x0 = v[f[i].array_numbers[0]].matrix[0][0] * scale;
    double y0 = -v[f[i].array_numbers[0]].matrix[1][0] * scale;
    double x1 = x0;
    double y1 = y0;
    for (int j = 1; j < f[i].count_numbers; j++) {
      double x2 = v[f[i].array_numbers[j]].matrix[0][0] * scale;
      double y2 = -v[f[i].array_numbers[j]].matrix[1][0] * scale;
      scene->addLine(x1, y1, x0, y0, pen_line);
      x1 = x2;
      y1 = y2;
    }
    scene->addLine(x1, y1, x0, y0, pen_line);
  }
  qDebug() << Xmax << " " << Ymax << scale;
}

void Viewer::on_pushButton_openfile_clicked() {
  QString fileName;
  fileName = QFileDialog::getOpenFileName(
      this, tr("Open OBJ"), "", tr("OBJ Files (*.obj) ;; All files (*.*)"));
  if (!fileName.isEmpty()) {
    QFileInfo fi(fileName);
    ui->label_name->setText(fi.fileName());
    char input_str[255] = {};
    qDebug() << fileName;
    get_str(input_str, fileName);
    qDebug() << input_str;
    get_result(input_str, &v, &f, &v_count, &f_count);
    QString status = "Vertex = " + QString::number(v_count) +
                     "  Planes = " + QString::number(f_count) +
                     "  Path: " + fileName;
    ui->statusbar->showMessage(status);
    drawLines();
    qDebug() << "";
  }
}

void Viewer::on_horizontalSlider_sliderReleased() {
  double slider = ui->horizontalSlider->value();
  scale *= slider / 50.0;
  drawLines();
  qDebug() << slider << "  slide    " << scale;
  ui->horizontalSlider->setValue(50);
}

void Viewer::on_pushButton_return_clicked() {
  scale = 50;
  drawLines();
}

void Viewer::on_pushButton_scale_clicked() {
  get_matrix_multi(&v, v_count,
                   matrix_scaling(ui->doubleSpinBox_scaleX->value(),
                                  ui->doubleSpinBox_scaleY->value(),
                                  ui->doubleSpinBox_scaleZ->value()));
  drawLines();
}

void Viewer::on_pushButton_move_clicked() {
  get_matrix_multi(&v, v_count,
                   matrix_shifting(ui->doubleSpinBox_moveX->value(),
                                   ui->doubleSpinBox_moveY->value(),
                                   ui->doubleSpinBox_moveZ->value()));
  drawLines();
}

void Viewer::on_pushButton_rotate_clicked() {
  double degree = 180 / M_PI;
  get_matrix_multi(
      &v, v_count,
      xyz_axis_rotation(ui->doubleSpinBox_rotateX->value() / degree,
                        ui->doubleSpinBox_rotateY->value() / degree,
                        ui->doubleSpinBox_rotateZ->value() / degree));
  drawLines();
}

void Viewer::on_pushButton_rotateClear_clicked() {
  ui->doubleSpinBox_rotateX->setValue(0);
  ui->doubleSpinBox_rotateY->setValue(0);
  ui->doubleSpinBox_rotateZ->setValue(0);
}

void Viewer::on_pushButton_moveClear_clicked() {
  ui->doubleSpinBox_moveX->setValue(0);
  ui->doubleSpinBox_moveY->setValue(0);
  ui->doubleSpinBox_moveZ->setValue(0);
}

void Viewer::on_pushButton_scaleClear_clicked() {
  ui->doubleSpinBox_scaleX->setValue(1);
  ui->doubleSpinBox_scaleY->setValue(1);
  ui->doubleSpinBox_scaleZ->setValue(1);
}
