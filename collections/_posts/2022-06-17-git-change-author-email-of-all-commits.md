---
title: 'git: change name/email of commits'
date: 2022-06-17
categories:
- it basics
tags:
- git
- mail
---
Use case: local commits contain personalized information, i.e. your personal email address, and you want to get rid of them.

# Method 1: Same name/email for all commits

## Method 1a: Using newly set name/email in config
Set new name/email via 
```bash
$ git config user.name "New Name"
$ git config user.email "new@email.com"
```
Apply new name/email to all commits _after_ a specific commit
```bash
$ git rebase -i b19777848301 -x "git commit --amend --reset-author -CHEAD"
```

## Method 1b: explicitly specifying names and emails
This is the brute force approach: override all commits using the same new info (disregarding initial contributor info)
```bash
$ git filter-branch -f --env-filter "GIT_AUTHOR_NAME='yourname'; GIT_AUTHOR_EMAIL='youremail@example.com'; GIT_COMMITTER_NAME='yourname'; GIT_COMMITTER_EMAIL='youremail@example.com';" HEAD
```

# Method 2: override commits for specific contributors
Lets assume that your repo has two contributors: (Alice, a@a.a) and (Bob, b@b.b).
You might want to change them to (Axxx, "") and (Bxxx, ""). This can be achieved by the following commands:
```bash
$ export FILTER_BRANCH_SQUELCH_WARNING=1
$ git filter-branch -f --env-filter '
WRONG_EMAIL="a@a.a"
NEW_NAME="Axxx"
NEW_EMAIL=""

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags

$ git filter-branch -f --env-filter '
WRONG_EMAIL="b@b.b"
NEW_NAME="Bxxx"
NEW_EMAIL=""

if [ "$GIT_COMMITTER_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$NEW_NAME"
    export GIT_COMMITTER_EMAIL="$NEW_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$WRONG_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$NEW_NAME"
    export GIT_AUTHOR_EMAIL="$NEW_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags

$ git push -f origin master
```

# Method 3: Rebase and manually amend commit message
```bash
# rebase. 
# commit span: the "5" in "@~5" is only an example
# --rebase-merge needed if merges within commit span
git rebase @~5 -i [--rebase-merge]

# replace "pick" with "e" wherever needed
git commit --amend --no-edit --author "Name <my@email.com>"
git rebase --continue
```