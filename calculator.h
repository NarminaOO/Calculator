 #ifndef _Calculator_h_
 #define _Calculator_h_
 #include <QWidget>
 #include <QStack>
 #include <QLabel>
 #include <QPushButton>
 #include <QGridLayout>
 #include <QMediaPlayer>


 class Calculator : public QWidget
 {
  Q_OBJECT
  private:
   QLabel *displayString;
   //QMediaPlayer *player;
   QStack <QString> stack;
  public:
   Calculator (QWidget* parent = 0);
   QPushButton* createButton (const QString& str);
   void calculate ();
  public slots:
   void slotButtonClicked ();

 };
#endif
