# Simple Calculator

![image](https://github.com/NarminaOO/Calculator/assets/149943881/ac29ff63-f4d8-4e81-9ec6-f93f261936df)

Simple Calculator is a basic calculator application implemented in C++ using the Qt framework. It provides a user-friendly interface for performing simple arithmetic operations.

## Features
**Basic Arithmetic Operations:** Addition, subtraction, multiplication, and division.

**Clear Entry (CE) Functionality:** Allows users to clear the current entry.

**Decimal Support:** Supports decimal numbers for precise calculations.

**Error Handling:** Handles division by zero error and displays an error message.

**Sound Effects:** Provides sound effects (a cat's meow) upon button clicks using QMediaPlayer.



## Files
+ calculator.pro
  >This file manages project settings, dependencies and build configurations.
+ calculator.cpp
  >This file contains a C++ class called Calculator, which includes the functionality and some visual aspects of the calculator     widget.
+ calculator.h
  >A header file of the class Calculator.
+ main.cpp
  >This file contains the main function that invokes the calculator widget.


## Code explanation
### in calculator.cpp
#### headers
```c++
#include "calculator.h"
#include <QRegularExpression>
#include <QDebug>
#include <QMediaPlayer>
```
**"calculator.h":** Includes the declaration of the Calculator widget class, defining its structure and behavior.

**QRegularExpression:** Provides Qt's functionality for working with regular expressions, used for string validation and manipulation.

**QDebug:** Enables debugging output in the application, allowing developers to print debug messages to the console for troubleshooting.

**QMediaPlayer:**  Integrates multimedia playback capabilities into the application, specifically used for playing sound effects in response to button clicks.


#### constructor `Calculator::Calculator(QWidget *parent)`

```c++
Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    displayString = new QLabel("");
    displayString->setMinimumSize(150,50);
    displayString->setStyleSheet("font-size: 20pt");

    // Define the layout of the calculator buttons
    QChar aButtons[4][4] = {
        {'7', '8', '9', '/'},
        {'4', '5', '6', '*'},
        {'1', '2', '3', '-'},
        {'0', '.', '=', '+'}
    };

    // Create a grid layout to arrange the buttons
    QGridLayout *myLayout = new QGridLayout;
    myLayout->addWidget(displayString, 0, 0, 2, 4);
    myLayout->addWidget(createButton("CE"), 1, 3);

    // Add buttons to the layout
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            myLayout->addWidget(createButton(aButtons[i][j]), i+2, j);
        }
    }

    // Set the layout for the calculator widget
    setLayout(myLayout);
}
```
In this constructor, the UI for the calculator is set up using Qt widgets. It includes creating a label (`displayString`) for showing input and results, defining a grid layout for arranging buttons, adding buttons to the layout, and setting the layout for the calculator widget.


#### Method `Calculator::createButton(const QString& str)`

```c++
QPushButton* Calculator::createButton(const QString& str) {
    QPushButton* button = new QPushButton(str);
    button->setIcon(QIcon(":/cat.png")); // Set button icon
    button->setIconSize(QSize(35,35));
    button->setStyleSheet("background-color: lightGreen"); // Set button style
    button->setMinimumSize(50,50);
    connect(button, SIGNAL(clicked()), this, SLOT(slotButtonClicked())); // Connect button click signal to slot
    return button;
}

```
This method creates and configures a button with the given text. It also sets an icon (`":/cat.png"`), style (`"background-color: lightGreen"`), and minimum size for the button. Then, a connection is made between the button's `clicked()` signal and the `slotButtonClicked()` slot, which will be called when the button is clicked.

#### Method `Calculator::calculate()`

```c++
void Calculator::calculate() {
    double dOperand2 = stack.pop().toDouble();
    QString strOperation = stack.pop();
    double dOperand1 = stack.pop().toDouble();
    double dResult = 0;
    if (strOperation == "+") { dResult = dOperand1 + dOperand2; }
    else if (strOperation == "-") { dResult = dOperand1 - dOperand2; }
    else if (strOperation == "/") {
        if(dOperand2 == 0) {qDebug()<<"ERROR!DIVISION BY ZERO";}
        else{dResult = dOperand1 / dOperand2;}
    }
    else if (strOperation == "*") { dResult = dOperand1 * dOperand2; }

    // Round the result to 3 decimal places if it's not an integer
    bool isInteger = (dResult == static_cast<int>(dResult));
    if(isInteger){
        displayString->setText(QString::number(static_cast<int>(dResult)));
    }else {
        displayString->setText(QString::number(dResult,'f',3));
    }
}
```
This method performs the calculation based on the operands and operation stored in the stack (`stack`). After the calculation, the result is displayed in `displayString`. If division by zero occurs, an error message is printed.


#### Method `Calculator::slotButtonClicked()`

```c++
void Calculator::slotButtonClicked() {
    QString str = ((QPushButton*)sender())->text();
    if (str == "CE") {
        stack.clear();
        displayString->setText("");
    }

    QString text = displayString->text();
    int len = text.length();
    QString last = "";
    if (len>0) last = text.right(1);

    if (((len==0 && stack.count()==0) ||
         ((stack.count()==2 && len>1 && (last=="+"||last=="-"||last=="*"||last=="/")))) &&
        (str.contains(QRegularExpression("[0-9]")) || str=="-")) {
        text=str;
    }
    else if ((text+str).contains(QRegularExpression("^-?[0-9]+\\.?[0-9]*$"))) {
        text+=str;
    }
    else if (text.contains(QRegularExpression("^-?[0-9]+\\.?[0-9]*$"))) {
        if (str=="*"||str=="/"||str=="+"||str=="-"||str=="=") {
            if (stack.count()==2) {
                stack.push(text);
                calculate();
                text=displayString->text();
            }
            if (str!="=") {
                stack.push(text);
                text+=str;
                stack.push(str);
            }
        }
    }
    displayString->setText(text);

    // Create QMediaPlayer object for sound playback
    QMediaPlayer *player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/cat-meow-sound-effect.mp3"));
    player->setVolume(150);

    // Play sound on button click
    if(player->state() == QMediaPlayer::PlayingState) {
        player->setPosition(0);
        player->play();
    } else if(player->state() == QMediaPlayer::StoppedState) {
        player->play();
    }
}

```
This method is called when any button is clicked. It handles the click event, updates the input string, performs operations based on the entered values and operations, and plays a sound effect (meow) on button click.

### in main.cpp

```c++
#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Calculator calculator;
    calculator.setStyleSheet("background-color: pink");
    calculator.setWindowTitle("Calculator");
    calculator.setFixedWidth(400);
    calculator.setFixedHeight(375);
    calculator.show();
    return app.exec();
}

```
The `main` function initializes the Qt application, creates an instance of the `Calculator` widget, sets its properties (such as style, window title, size), displays it on the screen, and starts the event loop to handle user interactions.



## Getting Started
### Prerequisites
- C++ compiler
- Qt framework
  
### Installation
1. Clone the repository or download the source code.
2. Download png files for button icon, mp3 files for meow sound.
3. Open the project in Qt Creator or your preferred IDE.
4. Build and run the project.

### Usage
Simply click the buttons on the calculator interface to input numbers and perform operations. The CE button clears the current entry, and the "=" button calculates the result. Have fun :)












