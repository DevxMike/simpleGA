#ifndef simpleGA_h
#define simpleGA_h

#include <vector>
#include <functional>
#include <algorithm>

namespace simpleGA_defaults {
	constexpr unsigned population_size = 1000;
	constexpr unsigned max_epoch = 2000;
};

int get_rand_int(int s, int e) {
	return s + (rand() % ((e - s) + 1));
}

template <typename IndividualType>
struct individual {
	IndividualType genome;
	int fitness;

	individual(IndividualType& i):
		genome{ i }, fitness{ 100 } {}
};

template <typename I>
bool operator<(const individual<I>& i1, const individual<I>& i2) {
	return i1.fitness < i2.fitness;
}

template <typename TargetType, typename IndividualType>
struct objective {
	using fitness_fn_type = std::function<int(TargetType&, IndividualType&)>;
	using population_type = std::vector<individual<IndividualType>>;

	TargetType target;
	fitness_fn_type fitness_fn;

	objective(TargetType& t, fitness_fn_type f)
		: target{ std::move(t) }, fitness_fn{ f }{}
};

template <typename TargetType, typename IndividualType>
class simpleGA {
private:
	using population_type = std::vector<individual<IndividualType>>;
	using mating_fn_type = std::function<individual<IndividualType>(individual<IndividualType>&, individual<IndividualType>&)>;
	using mutating_fn_type = std::function<void(individual<IndividualType>&)>;
	using fitness_fn_type = std::function<int(TargetType&, IndividualType&)>;

	std::vector<individual<IndividualType>> population;
	objective<TargetType, IndividualType> target;
	mating_fn_type mating_fn;
	mutating_fn_type mutating_fn;
	unsigned generations;

public:
	simpleGA(population_type& initial_population,
		TargetType& target, 
		fitness_fn_type fn1, 
		mating_fn_type fn2,
		mutating_fn_type fn3
		) :
		population{ initial_population },
		target{ target, fn1 }, mating_fn{ fn2 }, mutating_fn{ fn3 },
		generations{ 0 } {}

	void calc_fitness() {
		for (auto& x : population) {
			x.fitness = target.fitness_fn(target.target, x.genome);
		}
	}

	void next_generation() {
		++generations;

		population_type new_population;
		unsigned elite_count = 0.1 * simpleGA_defaults::population_size;
		unsigned reproduce_count = 0.9 * simpleGA_defaults::population_size;

		for (unsigned i = 0; i < elite_count; ++i) {
			new_population.push_back(population[i].genome);
		}

		for (unsigned i = 0; i < reproduce_count; ++i) {
			int p1_index = get_rand_int(0, 0.5 * simpleGA_defaults::population_size);
			int p2_index = get_rand_int(0, 0.5 * simpleGA_defaults::population_size);
			auto& parent1 = population[p1_index];
			auto& parent2 = population[p2_index];

			auto child = mating_fn(parent1, parent2);
			mutating_fn(child);

			new_population.push_back(child);
		}

		population = new_population;

		calc_fitness();

		std::sort(population.begin(), population.end());
	}

	int get_epoch() {
		return generations;
	}

	individual<IndividualType>& get_best() {
		return population.at(0);
	}
};

#endif