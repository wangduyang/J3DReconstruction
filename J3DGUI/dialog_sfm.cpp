#include "dialog_sfm.h"
#include "ui_dialog_sfm.h"
Dialog_SFM::Dialog_SFM(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_SFM)
{
	ui->setupUi(this);
}

Dialog_SFM::~Dialog_SFM()
{
	delete ui;
}


void Dialog_SFM::on_btn_OK_clicked()
{
	QString matchesDir, sfmOutputDir, triangulationMethod, resectionMethod;
	if (Global::GetProcessidFromName("J3DEngine.exe") == 0)
	{
		QMessageBox::critical(this, u8"���� ", u8"δ�ҵ�J3DEngine����", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	else
		Global::connectEngine();



	if (ui->lineEdit_inputDir->text() == "")
	{
		QMessageBox::critical(this, u8"���� ", u8"δ����ƥ���ļ�·�� ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	if (ui->lineEdit_OutputDir->text() == "")
	{
		QMessageBox::critical(this, u8"���� ", u8"δ�������·�� ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}

	matchesDir = ui->lineEdit_inputDir->text();
	Global::matchesDir = matchesDir;

	sfmOutputDir = ui->lineEdit_OutputDir->text();
	Global::sfmOutputDir = sfmOutputDir;

	triangulationMethod = triangulationMethod.number(ui->comboBox_triangulationMethod->currentIndex());

	resectionMethod = resectionMethod.number(ui->comboBox_resectionMethod->currentIndex());

	_mkdir("C:\\ProgramData\\J3DEngine");

	QFile cmdcache("C:\\ProgramData\\J3DEngine\\cmdCache.tmp");

	if (cmdcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
	{
		QString head = "sfmandsfp\n";
		cmdcache.write(head.toUtf8());
		cmdcache.write(matchesDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(sfmOutputDir.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(triangulationMethod.toUtf8());
		cmdcache.write("\n");
		cmdcache.write(resectionMethod.toUtf8());
		cmdcache.write("\n");

		cmdcache.close();
		QMessageBox::information(NULL, u8"���", u8"������� ", QMessageBox::Ok, NULL);
		PostThreadMessageA(Global::engineTid, CMD_SFMANDSFP, 0, 0);
		Global::tasking = true;
		this->close();
	}
	else
	{
		QMessageBox::information(NULL, u8"����", u8"�޷�д�뻺�棬����Ȩ�� ", QMessageBox::Ok, NULL);
	}

}

void Dialog_SFM::on_btn_CANCEL_clicked()
{
	this->close();
}

void Dialog_SFM::on_pushButton_browseInputDir_clicked()
{
	Global::matchesDir = QFileDialog::getExistingDirectory(this, u8"���ͼƬ�ļ��� ", "", NULL);
	ui->lineEdit_inputDir->setText(Global::matchesDir);
}

void Dialog_SFM::on_pushButton_browseOutputDir_clicked()
{
	Global::sfmOutputDir = QFileDialog::getExistingDirectory(this, u8"���ͼƬ�ļ��� ", "", NULL);
	ui->lineEdit_OutputDir->setText(Global::sfmOutputDir);
}
