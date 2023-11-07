simpleGA is basically a simple implementation of a genetic algorithm

Use at YOUR OWN risk, I take no responsibility for misuse.

included types:

- individual<IndividualType> - allows you to create a custom genotype based on IndividualType
- objective<TargetType, IndividualType> - allows you to create a custom target type based on TargetType
- simpleGA<TargetType, IndividualType> - a class that takes care of crossover, mutation and fitness calculation for a given generation.

default values:

population_size = 1000
unsigned max_epoch = 2000

usage:
- initialize rand generator: srand(std::time(NULL));
- create an initial random population (std::vector<individual<IndividualType>>)
- define custom fitness, mutation and crossover functions for the custom type
- create a loop: check if the best fitness is greater than 0 (you can also limit the number of epochs), and then call the next_generation() method.

Usage example:
Based on the article https://www.geeksforgeeks.org/genetic-algorithms/, I created a simple use case that can be used