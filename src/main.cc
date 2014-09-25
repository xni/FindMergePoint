#include <iostream>

#include "git2.h"

int show_ref(git_reference *ref, void *data) {
  std::cout << "Next ref\n";
  return 0;
}

int main() {
  git_threads_init();
  git_repository* repo = NULL;
  int error = git_repository_open_bare(&repo, "/home/stromsund/Development/request-by-commit/test/testrepo_bare");
  if (error) {
    std::cerr << "Error on opening repo! (" << error << ")\n";
    return 1;
  }

  git_revwalk *walker;
  git_revwalk_new(&walker, repo);
  git_revwalk_push_glob(walker, "*");
  git_revwalk_sorting(walker, GIT_SORT_NONE);
  git_oid oid;
  int counter = 0;
  while (!git_revwalk_next(&oid, walker)) {
    counter += 1;
  }
  git_revwalk_free(walker);
  std::cout << counter << '\n';

  git_repository_free(repo);
  git_threads_shutdown();
  return 0;
}
