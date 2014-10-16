#include <iostream>
#include <vector>
#include <unordered_map>

#include "git2.h"

struct GitOidHasher {
  size_t operator()(const git_oid& oid) const {
    std::vector<char> str(40, '\0');
    git_oid_fmt(&str[0], &oid);
    std::string id(&str[0]);
    std::cout << "Commit Id is " << id << '\n';
    return str[0];
  }  
};

typedef std::unordered_map<git_oid, unsigned char, GitOidHasher> ParentCounter;

bool operator==(const git_oid& lfs, const git_oid& rhs) {
  return git_oid_equal(&lfs, &rhs);
}


bool is_merge_commit(const git_oid& oid,
                     git_repository* repo,
                     ParentCounter* parent_counter) {
  ParentCounter::const_iterator element = parent_counter->find(oid);
  size_t parent_count;
  if (element == parent_counter->end()) {
    git_commit* commit;
    git_commit_lookup(&commit, repo, &oid);    
    parent_count = git_commit_parentcount(commit);
    (*parent_counter)[oid] = parent_count;
    git_commit_free(commit);
  } else {
    std::cout << "Cache pick!\n";
    parent_count = element->second;
  }
  return parent_count > 1;
}

bool traverse_children(const git_oid& current,
                       const git_oid& parent,
		       git_repository* repo,
		       ParentCounter* parent_counter) {
  git_commit* commit;
  git_commit_lookup(&commit, repo, &current);    
  size_t parent_count = git_commit_parentcount(commit);
  for (size_t parent_index = 0; parent_index < parent_count; ++parent_index) {
    const git_oid* parent_id = git_commit_parent_id(commit, parent_index);
    if (!is_merge_commit(*parent_id, repo, parent_counter)) {
	    std::cout << "Traverce child\n";
      traverse_children(*parent_id, parent, repo, parent_counter);
    }
  }
  git_commit_free(commit);
}

int show_ref(git_reference *ref, void *data) {
  std::cout << "Next ref\n";
  return 0;
}

int main() {
  std::unordered_map<git_oid, unsigned char, GitOidHasher> parent_count;
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
    if (!is_merge_commit(oid, repo, &parent_count)) {
	    traverse_children
		    (oid, oid, repo, &parent_count);
	    continue;
    }
    git_commit* commit;
    git_commit_lookup(&commit, repo, &oid);
    git_commit_free(commit);
    parent_count[oid] = git_commit_parentcount(commit);
    std::cout << git_commit_parentcount(commit) << ' ' << git_commit_message(commit) << '\n';
    counter += 1;
  }
  git_revwalk_free(walker);
  std::cout << counter << '\n';

  git_repository_free(repo);
  git_threads_shutdown();
  return 0;
}
