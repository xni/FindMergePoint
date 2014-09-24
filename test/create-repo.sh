#!/bin/sh -e

if [ -d testrepo ]
then
  rm -rf testrepo
fi
if [ -d testrepo_bare ]
then
  rm -rf testrepo_bare
fi

git init testrepo

cd testrepo
echo "Init" > README
git add README
git commit -m "Initial commit"
git checkout -b feature-1
echo "Feature 1" > README
git add -u
git commit -m "Feature 1"
git checkout master
git merge feature-1 --no-ff -m "Merged Feature 1"
git checkout -b feature-2
echo "Feature 2" > README
git add -u
git commit -m "Feature 2"
git checkout master
git merge feature-2 --no-ff -m "Merged Feature 2"
echo "Direct commit" > README
git add -u
git commit -m "Blocker fix"
git checkout -b feature-3
echo "Feature 3" > README
git add -u
git commit -m "Feature 3"
git checkout master
git merge feature-3 --no-ff -m "Merged Feature 3"
git checkout -b feature-4 HEAD^
echo "Feature 4" > main.cc
git add main.cc
git commit -m "Feature 4"
git checkout master
git merge feature-4 --no-ff -m "Merged Feature 4"
git checkout -b feature-5
echo "Feature 5" > main.cc
git add main.cc
git commit -m "Feature 5"
echo "Feature 5.1" > main.cc
git add main.cc
git commit -m "Feature 5.1"
git checkout master
git merge feature-5 --no-ff -m "Merged Feature 5"
cd ..
git clone --bare testrepo testrepo_bare
