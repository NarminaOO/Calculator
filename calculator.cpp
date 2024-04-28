#include "calculator.h"
#include <QRegularExpression>
#include <QDebug>
#include <QMediaPlayer>


Calculator::Calculator (QWidget *parent) : QWidget(parent) {
 displayString = new QLabel("");
 displayString->setMinimumSize(150,50);
 displayString->setStyleSheet("font-size: 20pt");

 QChar aButtons[4][4] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'0', '.', '=', '+'}
 };

 QGridLayout *myLayout = new QGridLayout;
 myLayout->addWidget(displayString, 0, 0, 2, 4);
 myLayout->addWidget(createButton("CE"), 1, 3);

 for (int i = 0; i < 4; ++i) {
  for (int j = 0; j < 4; ++j) {
   myLayout->addWidget(createButton(aButtons[i][j]), i+2, j);
  }
 }
 setLayout(myLayout);
 //player = new QMediaPlayer(this);
}


QPushButton* Calculator::createButton (const QString& str) {
 QPushButton* button = new QPushButton(str);
 button->setIcon(QIcon(":/cat.png"));
 button->setIconSize(QSize(35,35));
 button->setStyleSheet("background-color: lightGreen");
 button->setMinimumSize(50,50);
 connect(button, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
 return button;
}


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

 bool isInteger = (dResult == static_cast<int>(dResult));
 if(isInteger){
     displayString->setText(QString::number(static_cast<int>(dResult)));
 }else
 {
     displayString->setText(QString::number(dResult,'f',3));
 }

 //displayString->setText(QString("%1").arg(dResult, 0, 'f', 3));

 // добавление звука

}


void Calculator::slotButtonClicked() {
 QString str = ((QPushButton*)sender())->text();
 if (str == "CE") {
  stack.clear(); displayString->setText(""); //return;
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

 QMediaPlayer *player = new QMediaPlayer(this);
 player->setMedia(QUrl("qrc:/cat-meow-sound-effect.mp3"));
 player->setVolume(150);

 if(player->state() == QMediaPlayer::PlayingState)
 {
     player->setPosition(0);
     player->play();
 }
 else if(player->state() == QMediaPlayer::StoppedState)
 {
     player->play();
 }

}





