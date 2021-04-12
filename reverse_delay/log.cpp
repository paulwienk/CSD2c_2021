
// logging methods
void DelayLine::logRWPos()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logDistanceRW()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logSize()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logAllSettings()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "Read head position: " << m_readH << "\n";
	std::cout << "Write head position: " << m_writeH << "\n";
  std::cout << "Distance between read and write head: " << m_distanceRW << "\n";
  std::cout << "Buffer size: " << m_size << "\n";
	std::cout << "_________________________________________________________________\n";
}

void DelayLine::logAllValues()
{
	std::cout << "\n______________________ DelayLine ______________________________\n";
	std::cout << "DelayLine - buffer contains: \n";
	for (unsigned int i = 0; i < m_size - 1; i++)
	{
		std::cout << m_buffer[i] << ", ";
	}
	std::cout << m_buffer[m_size - 1] << "\n";
	std::cout << "_________________________________________________________________\n";
}
