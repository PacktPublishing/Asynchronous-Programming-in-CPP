#include <algorithm>
#include <forward_list>
#include <future>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

template <typename C>
bool search(const C& container, const std::string& target) {
    return std::find(container.begin(), container.end(), target) != container.end();
}

int main() {
    std::vector<std::string> africanAnimals = {"elephant", "giraffe", "lion", "zebra"};
    std::list<std::string> americanAnimals = {"alligator", "bear", "eagle", "puma"};
    std::forward_list<std::string> asianAnimals = {"orangutan", "panda", "tapir", "tiger"};
    std::set<std::string> europeanAnimals = {"deer", "hedgehog", "linx", "wolf"};

    std::string target = "elephant";

    // Launch asynchronous searches on different containers
    auto fut1 = std::async(std::launch::async, search<std::vector<std::string>>, africanAnimals, target);
    auto fut2 = std::async(std::launch::async, search<std::list<std::string>>, americanAnimals, target);
    auto fut3 = std::async(std::launch::async, search<std::forward_list<std::string>>, asianAnimals, target);
    auto fut4 = std::async(std::launch::async, search<std::set<std::string>>, europeanAnimals, target);

    // Collect results
    bool found = fut1.get() || fut2.get() || fut3.get() || fut4.get();
    if (found) {
        std::cout << target << " found in one of the containers.\n";
    } else {
        std::cout << target << " not found in any of the containers.\n";
    }

    return 0;
}
