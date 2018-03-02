#ifndef CONFIGUREDLG_H
#define CONFIGUREDLG_H

#include <QDialog>

namespace Ui {
class ConfigureDlg;
}

class ConfigureDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureDlg(QWidget *parent = 0);
    ~ConfigureDlg();

public slots:
	void SavePortToIni();
	void SelectRawPath();
	void SaveRawPath();

private:
    Ui::ConfigureDlg *ui;
};

#endif // CONFIGUREDLG_H
