#include <iostream>

#include "git2.h"

int main() {
  git_repository* repo = NULL;
  int error = git_repository_open_bare(&repo, "/home/stromsund/Development/request-by-commit/test/testrepo_bare");
  if (error) {
    std::cerr << "Error on opening repo! (" << error << ")\n";
    return 1;
  }
  std::cout << error << "\n";
  return 0;
}
