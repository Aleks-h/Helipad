#ifndef LOGGING_H
#define LOGGING_H


#include <QWidget>

namespace Ui
{
class MainWindow;
}

class Logging : public QWidget
{
    Q_OBJECT

public:
    explicit Logging(QWidget *parent = 0);
    ~Logging();

signals:
    void firstWindow();  // Сигнал для первого окна на открытие

private slots:
    // Слот-обработчик нажатия кнопки
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // LOGGING_H
