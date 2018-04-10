#ifndef TestDialog_H
#define TestDialog_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class TestDialog;
}

class TestDialog : public QDialog
{
    Q_OBJECT

public:
	explicit TestDialog(QWidget *parent = 0);
	~TestDialog();


private:
	Ui::TestDialog *ui;

	void setBtnQss(QPushButton *btn,
		QString normalColor, QString normalTextColor,
		QString hoverColor, QString hoverTextColor,
		QString pressedColor,QString pressedTextColor);

	void setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor);

public slots:
	void ChangeStyle();
	void CloseWindow();
};

#endif // TestDialog_H
