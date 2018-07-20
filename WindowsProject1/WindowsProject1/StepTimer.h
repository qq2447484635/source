#pragma once
#include <Windows.h>
#include <math.h>
#include <wrl.h>

typedef unsigned long long uint64;
typedef unsigned long  uint32;
typedef  long  int64;

namespace DX
{
	// ������ģ���ʱ�İ��������ࡣ
	class StepTimer
	{
	public:
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw "timer error";
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw "timer error";
			}

			// �����������ʼ��Ϊ 1/10 �롣
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		// ��ȡ��һ�� Update ���ú������ʱ�䡣
		uint64 GetElapsedTicks() const { return m_elapsedTicks; }
		double GetElapsedSeconds() const { return TicksToSeconds(m_elapsedTicks); }

		// ��ȡ��������֮�����ʱ�䡣
		uint64 GetTotalTicks() const { return m_totalTicks; }
		double GetTotalSeconds() const { return TicksToSeconds(m_totalTicks); }

		// ��ȡ�Գ�������֮��ĸ��´�����
		uint32 GetFrameCount() const { return m_frameCount; }

		// ��ȡ��ǰ֡���ʡ�
		uint32 GetFramesPerSecond() const { return m_framesPerSecond; }

		// ������ʹ�ù̶��Ļ��ǿɱ��ʱ�䲽��ģʽ��
		void SetFixedTimeStep(bool isFixedTimestep) { m_isFixedTimeStep = isFixedTimestep; }

		// ��ʹ�ù̶�ʱ�䲽��ģʽʱ�����õ��� Update ��Ƶ�ʡ�
		void SetTargetElapsedTicks(uint64 targetElapsed) { m_targetElapsedTicks = targetElapsed; }
		void SetTargetElapsedSeconds(double targetElapsed) { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// ������ʽʹ��ÿ�� 10,000,000 �εδ�����ʾʱ�䡣
		static const uint64 TicksPerSecond = 10000000;

		static double TicksToSeconds(uint64 ticks) { return static_cast<double>(ticks) / TicksPerSecond; }
		static uint64 SecondsToTicks(double seconds) { return static_cast<uint64>(seconds * TicksPerSecond); }

		// �ڹ����жϼ�ʱ(���磬��ֹ�� IO ����)֮��
		// ���ô˺����Ա���̶�ʱ�䲽���߼�����һϵ���ֲ�
		// Update ���á�

		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw "timer error";
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		// ���¼�ʱ��״̬�������ʵ�������ָ�� Update ������
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// ��ѯ��ǰʱ�䡣
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw "timer error";
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// �̶������ʱ������(���磬 �ڵ���������֮ͣ��)��
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// �� QPC ��λת��Ϊ�淶�ļ�ʱ��ʽ�������Ϊ��һ�ι̶����޷������
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// �̶�ʱ�䲽�������߼�

				// ���Ӧ�õ�����ʱ��ʮ�ֽӽ�Ŀ������ʱ��(1/4 ������)����ֱ�ӹ̶�
				// ʱ������Ŀ��ֵ��ȫһ�¡��⽫��ֹ΢С���޹صĴ���
				// ��ʱ���ۻ����ڲ�ʹ�ô˹̶�������£����� 60 fps �̶����µ�
				// ��Ϸ����� 59.94 NTSC ��ʾ��������ʱ������ vsync�������ջ�
				// �ۻ��㹻���С��������µ�֡����ý�
				// ��Сƫ��Բ�����㣬����һ��ƽ�����С�

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// �ɱ�ʱ�䲽�������߼���
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// ���ٵ�ǰ֡���ʡ�
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// Դ��ʱ����ʹ�� QPC ��λ��
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// �����ļ�ʱ����ʹ�ù淶�ļ�ʱ��ʽ��
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// ���ڸ���֡���ʵĳ�Ա��
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// �������ù̶�ʱ�䲽��ģʽ�ĳ�Ա��
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;
	};
}
