#include "Randomizer.h"

Randomizer::Randomizer()
{
	m_MTwister.seed(randomDevice());
}

uint16_t Randomizer::Generate(uint16_t minim, uint16_t maxim)
{
	const std::uniform_int_distribution<uint16_t> dist(minim, maxim);
	return dist(m_MTwister);
}

int Randomizer::Generate(int minim, int maxim)
{
	const std::uniform_int_distribution<int> dist(minim, maxim);
	return dist(m_MTwister);
}

float Randomizer::Generate(float minim, float maxim)
{
	const std::uniform_real_distribution<float> dist(minim, maxim);
	return dist(m_MTwister);
}

