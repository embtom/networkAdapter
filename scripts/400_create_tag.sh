#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TOP_DIR="${DIR}/.."
cd ${TOP_DIR}

git_tag_new="v$(head -1 debian/changelog | awk -F'[()]' '{print $2}')"
echo ${TOP_VERSION}

git_rev_id=$(git -C $TOP_DIR rev-parse HEAD)
git_tags=$(git tag)
for git_tag in $git_tags; do
    git_temp_tag=$(git cat-file tag $git_tag | grep $git_rev_id);
    echo "$git_temp_tag"
    if [ -z "$git_temp_tag" ]
    then
        false; #do nothing
    else
        git_tag_exists=$git_tag
    fi
done

if [ -z "$git_tag_exists" ]
then
  echo "Create New Tag $git_tag_new "
  git tag -a ${git_tag_new} -m "gitlab release build ${git_tag_new}"
  git push origin --tags
else
  echo "Found tag: "$git_tag_exists
fi


