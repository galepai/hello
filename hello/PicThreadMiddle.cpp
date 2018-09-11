#include "PicThreadMiddle.h"
#include <QTime>
#include "CHH.h"

int PicThreadMiddle::num = 0;
void PicThreadMiddle::run()
{
	//qDebug() << "Worker Run Thread : " << QThread::currentThreadId();

	if (m_Image.Key() != 0)
	{
		try
		{
			/*HObject Region;
			Threshold(m_Image, &Region, 5, 200);
			SetDraw(m_WindowHandle, "margin");
			SetColor(m_WindowHandle, "red");
			DispObj(Region, m_WindowHandle);*/

			OnHandle(m_WindowHandle);

			num++;
			CHH::disp_message(m_WindowHandle, HTuple("number: ") + num, "image", 12, 12, "black", "true");

			qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
			if (num % 3)
			{
				emit resultReady(MiddleGood);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "black", "true");
			}
			else
			{
				emit resultReady(MiddleGood);
				CHH::disp_message(m_WindowHandle, HTuple("Good "), "image", 120, 12, "red", "true");
			}
		}
		catch (HException& e)
		{
			QString error = e.ErrorMessage().Text();
			//DispText(m_WindowHandle, error.toStdString().c_str(), "image", 120, 12, "red", HTuple(), HTuple());
		}
	}
		
}

void PicThreadMiddle::OnHandle(HTuple WindowHandle)
{
	HTuple hv_DownRow;
	CHH::PingJie(m_Image, &m_Image, 1000, 30, 3, 10, &hv_DownRow);
	DispObj(m_Image, WindowHandle);
}