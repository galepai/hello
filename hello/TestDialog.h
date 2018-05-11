#ifndef TestDialog_H
#define TestDialog_H

#include <QDialog>
#include <QLineEdit>
#include <vector>

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
	QTimer* m_pTimer;
	std::vector<bool> m_Y_States;
	std::vector<bool> m_X_States;
	virtual void paintEvent(QPaintEvent *event);

	void setBtnQss(QPushButton *btn,
		QString normalColor, QString normalTextColor,
		QString hoverColor, QString hoverTextColor,
		QString pressedColor,QString pressedTextColor);

	void setTxtQss(QLineEdit *txt, QString normalColor, QString focusColor);
	void PaintCirle(QPainter& painter, const QPointF& center_circle, int radius, const QPen &pen, const QBrush &brush);
	void updateButtonStatu(QPushButton* PushButton, bool status);

public slots:
	void readyDataSlot(QByteArray str);
	void ChangeStyle();
	void CloseWindow();
	void OnZhuaShouLeft();	//抓手左
	void OnZhuaShouRight();	//抓手右
	void OnZhuaShouUp();	//抓手上
	void OnZhuaShouDown();	//抓手下
	void OnHandOpen();	//手指开
	void OnHandClose();	//手指关
	void OnBoShouUp();	//拨手伸
	void OnBoShouDown();	//拨手缩
	void OnBoShouShiPingUp();	//拨手左
	void OnBoShouShiPingDown();	//拨手右
};

#endif // TestDialog_H
