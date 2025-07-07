#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void clearClicked();
    void equalsClicked();

private:
    Ui::Calculator *ui;
    QString pendingOperator;
    double pendingOperand;
    bool waitingForOperand;

    void abortOperation();
    bool calculate(double rightOperand, const QString &operatorText);
};
#endif // CALCULATOR_H
