#include "mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>

#include "../smartcalc.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  //  шрифт в конструкторе

  this->setWindowTitle("Smart Calc (hakonoze)");

  QFont customFont;
  customFont.setFamily(
      "SFMono-Medium");

  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));

  connect(ui->pushButton_open_br, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_close_br, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(digits_numbers()));

  connect(ui->pushButton_divide, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this,
          SLOT(digits_numbers()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(digits_numbers()));

  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(digits_numbers()));

  // кнопка "=" (нажатие ровно)
  connect(ui->pushButton_equal, SIGNAL(clicked()), this, SLOT(get_result()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::get_result() {
  // Нажата кнопка "="
  equal_is_pressed = 1;

  // получение итоговой строки
  QString result_text = ui->result_show->text();
  result_text.replace("÷", "/");
  result_text.replace("×", "*");
  result_text.replace(",", ".");

  // получение x
  if (this->x_is_pressed == 1) {
    // Вызов функции on_pushButton_x_value_clicked() снова
    on_pushButton_x_value_clicked();
  }

  qInfo() << "string -- out" << result_text;

  // передаем данные в бек
  double result = 0;

  int result_code =
      smart_calc(result_text.toStdString().c_str(), &result, this->x);
  if (result_code == 1) {
    ui->result_show->setText(QString::number(result, 'g', 10));
  } else if (result_code == 0) {
    ui->result_show->setText("Invalid input");
    this->was_error_printed = 1;
  } else if (result_code == 3) {
    ui->result_show->setText("Error");
    this->was_error_printed = 1;
  }
}

void MainWindow::on_pushButton_x_value_clicked() {
  // Открываем диалоговое окно для ввода значения X
  bool ok;
  QString xText = QInputDialog::getText(
      this, "", "Введите значение X:", QLineEdit::Normal, QString(), &ok);

  // Если пользователь нажал OK и ввел значение, то сохраняем его
  if (ok && !xText.isEmpty()) {
    // Преобразовываем строку в число (предполагается, что X - это числовое
    // значение)
    double xValue = xText.toDouble();
    this->x = xValue;
    this->x_is_pressed = 0;
  }
}

void MainWindow::on_pushButton_x_clicked() {
  this->x_is_pressed = 1;  // Был нажат X
  //     if (xValue == 0) this->x_is_pressed = 0;
}

void MainWindow::digits_numbers() {
  if (this->was_error_printed == 1) {
    ui->result_show->clear();
    this->was_error_printed = 0;
  }

  QPushButton *button = (QPushButton *)sender();
  QString new_label;
  new_label = (ui->result_show->text() + button->text());

  if (button->text() == "SIN" || button->text() == "COS" ||
      button->text() == "TAN" || button->text() == "ACOS" ||
      button->text() == "ASIN" || button->text() == "ATAN" ||
      button->text() == "SQRT" || button->text() == "LN" ||
      button->text() == "LOG") {
    new_label += "(";
  }

  ui->result_show->setText(new_label);
}

void MainWindow::on_pushButton_clear_clicked() {
    this->x_is_pressed = 0;
    ui->result_show->clear();
}

void MainWindow::on_pushButton_draw_graph_clicked() {
  //Рисуем график y=x*x
  //Сгенерируем данные
  //Для этого создадим два массива точек:
  //один для сохранения x координат точек,
  //а второй для y соответственно
  double x_min = ui->doubleSpinBox_x_min
                     ->value();  //Начало интервала, где рисуем график по оси Ox
  double x_max = ui->doubleSpinBox_x_max
                     ->value();  //Конец интервала, где рисуем график по оси Ox

  double step = 0.1;  //Шаг, с которым будем пробегать по оси Ox

  //    int N = (b - a) / h + 2; //Вычисляем количество точек, которые будем
  //    отрисовывать
  QVector<double> x, y;  //Массивы координат точек

  int is_valid = validation(ui->result_show->text().toStdString().c_str());
  //Пробегаем по всем точкам
  if (is_valid == 1) {
    this->was_error_printed = 0;
    for (double X = x_min; X <= x_max; X += step) {
      double res = 0;
      x.push_back(X);
      smart_calc(ui->result_show->text().toStdString().c_str(), &res, X);
      y.push_back(res);
    }

    ui->widget->clearGraphs();  //Если нужно, но очищаем все графики

    //Добавляем один график в widget
    ui->widget->addGraph();

    //Говорим, что отрисовать нужно график по нашим двум массивам x и y
    ui->widget->graph(0)->setData(x, y);

    //Установим область, которая будет показываться на графике
    ui->widget->xAxis->setRange(x_min, x_max);  //Для оси Ox
    ui->widget->yAxis->setRange(-10, 10);       //Для оси Oy

    // задаем цвет точки
    ui->widget->graph(0)->setPen(QColor(251, 116, 45));

    //    ui->widget->yAxis->setRange(minY, maxY);//Для оси Oy

    // создание зума для гарфика
    ui->widget->setInteraction(QCP::iRangeZoom, true);

    QPen graphPen =
        ui->widget->graph(0)->pen();  // Получаем текущую кисть графика
    graphPen.setWidth(
        3);  // Устанавливаем толщину линии в 2 пикселя (по умолчанию 1)
    ui->widget->graph(0)->setPen(
        graphPen);  // Устанавливаем новую кисть для графика

    //И перерисуем график на нашем widget
    ui->widget->replot();
  } else if (is_valid == 0) {
    ui->result_show->setText("Invalid input");
    this->was_error_printed = 1;
  }
}

void MainWindow::on_pushButton_open_finance_clicked() {
  financewindow = new Finance(this);
  financewindow->show();
}

void MainWindow::on_pushButton_open_debit_clicked() {
  deposit_window = new DepositWindow(this);
  deposit_window->show();
}
