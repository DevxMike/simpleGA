#include <iostream>
#include "simpleGA.h"

#include <ctime>
#include <cstdlib>

#include <string>

const std::string GENES = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890,.-;:_!\"#%&/()=?@${[]}";

std::string TARGET = "Hello world from simpleGA :)";

using population_type = std::vector<individual<std::string>>;

individual<std::string> get_random_individual() {
	std::string result(TARGET.size(), 'a');

	for (int i = 0; i < result.size(); ++i) {
		result[i] = GENES[get_rand_int(0, GENES.size() - 1)];
	}

	if (result.size() != TARGET.size()) {
		throw "";
	}

	return result;
}

int get_fitness(std::string& t, std::string& i) {
	int f = 0;

	for (int j = 0; j < t.size(); ++j) {
		if (t[j] != i[j]) ++f;
	}

	return f;
}

void mutating_fn(individual<std::string>& i) {
		float p = get_rand_int(0, 100) / 100;
		if (p < 0.07) {
			i.genome[get_rand_int(0, i.genome.size() - 1)] = GENES[get_rand_int(0, GENES.size() - 1)];
		}
}

individual<std::string> mating_fn(individual<std::string>& i1, individual<std::string>& i2) {
	std::string child = "";

	for (int i = 0; i < i1.genome.size(); ++i) {
		float p = get_rand_int(0, 100) / 100;

		if (p < 0.50) {
			child += i1.genome[i];
		}
		else {
			child += i2.genome[i];
		}
	}

	return individual<std::string>{ child };
}

int main(void) {
	srand(std::time(NULL));

	population_type initial;

	for (int i = 0; i < simpleGA_defaults::population_size; ++i) {
		initial.push_back(get_random_individual());
	}

	simpleGA<std::string, std::string> algo(
		initial, TARGET, get_fitness, mating_fn, mutating_fn
	);

	unsigned tmp = 0;

	while (algo.get_best().fitness > 0 && tmp < simpleGA_defaults::max_epoch) {
		tmp = algo.get_epoch();
		if (tmp % 10 == 0) {
			std::cout << "Epochs: " << tmp << ", Solution: " << algo.get_best().genome << std::endl;
		}
		algo.next_generation();
	}

	std::cout << "Solution found: " << algo.get_best().genome << ", after " << algo.get_epoch() << " epochs." << std::endl;
}
