#include "calculator.h"
#include "ui_calculator.h"
#include <QPushButton>
#include <QMessageBox>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), ui(new Ui::Calculator), pendingOperand(0), waitingForOperand(true) {
    ui->setupUi(this);

    // Connect digit buttons
    for (int i = 0; i <= 9; ++i) {
        QString name = "btn" + QString::number(i);
        QPushButton *button = findChild<QPushButton *>(name);
        connect(button, &QPushButton::clicked, this, &Calculator::digitClicked);
    }

    // Connect operator buttons
    connect(ui->btnAdd, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->btnSub, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->btnMul, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->btnDiv, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &Calculator::clearClicked);
    connect(ui->btnEqual, &QPushButton::clicked, this, &Calculator::equalsClicked);
}

Calculator::~Calculator() {
    delete ui;
}

void Calculator::digitClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString clickedDigit = clickedButton->text();

    if (waitingForOperand) {
        ui->display->clear();
        waitingForOperand = false;
    }

    ui->display->setText(ui->display->text() + clickedDigit);
}

void Calculator::operatorClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    QString clickedOperator = clickedButton->text();
    double operand = ui->display->text().toDouble();

    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(pendingOperand));
    } else {
        pendingOperand = operand;
    }

    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalsClicked() {
    double operand = ui->display->text().toDouble();

    if (!pendingOperator.isEmpty()) {
        if (!calculate(operand, pendingOperator)) {
            abortOperation();
            return;
        }
        ui->display->setText(QString::number(pendingOperand));
        pendingOperator.clear();
    }

    waitingForOperand = true;
}

void Calculator::clearClicked() {
    ui->display->clear();
    pendingOperator.clear();
    pendingOperand = 0;
    waitingForOperand = true;
}

bool Calculator::calculate(double rightOperand, const QString &operatorText) {
    if (operatorText == "+") {
        pendingOperand += rightOperand;
    } else if (operatorText == "-") {
        pendingOperand -= rightOperand;
    } else if (operatorText == "*") {
        pendingOperand *= rightOperand;
    } else if (operatorText == "/") {
        if (rightOperand == 0.0)
            return false;
        pendingOperand /= rightOperand;
    }
    return true;
}

void Calculator::abortOperation() {
    clearClicked();
    QMessageBox::warning(this, "Error", "Invalid operation");
}



