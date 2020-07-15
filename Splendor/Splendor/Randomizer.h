#pragma once
#include <random>
#include <vector>
#include <array>

class Randomizer
{
public:
	Randomizer();
	uint16_t Generate(uint16_t minim, uint16_t maxim);
	int Generate(int minim, int maxim);
	float Generate(float minim, float maxim);
	template <class T>
	void Shuffle(std::vector<T>& container);
	template <class T, std::size_t SIZE>
	void Shuffle(std::array<T, SIZE>& container);

private:
	std::random_device randomDevice;
	std::mt19937 m_MTwister;
};

template <class T>
void Randomizer::Shuffle(std::vector<T>& container)
{
	std::shuffle(std::begin(container), std::end(container), m_MTwister);
}

template <class T, std::size_t SIZE>
void Randomizer::Shuffle(std::array<T, SIZE>& container)
{
	auto rng = std::default_random_engine{};
	std::shuffle(std::begin(container), std::end(container), m_MTwister);
}
