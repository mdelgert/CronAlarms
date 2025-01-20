#!/bin/bash

# --------------------------------------------------------
# Script: update-master.sh
#
# Purpose:
# This script updates the `master` branch from the `dev` branch
# for a PlatformIO or Arduino library repository. It removes
# unnecessary files, cleans up the structure, and moves the
# library files from `lib/<LIBRARY_NAME>` to the root of the
# `master` branch for production-ready distribution.
#
# Workflow:
# 1. Switches to the `dev` branch (where development happens).
# 2. Cleans up all files in the `dev` branch except:
#    - `.git` directory
#    - `README.md`
#    - `.gitignore`
#    - `lib/<LIBRARY_NAME>` folder.
# 3. Moves the contents of `lib/<LIBRARY_NAME>` to the root directory.
# 4. Commits the changes.
# 5. Force-pushes the cleaned-up `dev` branch to the `master` branch.
#
# Usage:
# 1. Save this script as `update-master.sh` in your repository.
# 2. Make it executable: `chmod +x update-master.sh`.
# 3. Run it from the repository root: `./update-master.sh`.
#
# Prerequisites:
# - The `dev` branch must exist and contain the library files
#   in the `lib/<LIBRARY_NAME>` folder.
# - Ensure you have Git installed and authenticated.
#
# Example:
# ./update-master.sh
#
# Author: Matthew Elgert
# Last Updated: January 2025
# --------------------------------------------------------

set -e  # Exit on any error

# Configurable variables
LIBRARY_NAME="CronAlarms"  # Change this to the name of your library

echo "Switching to dev branch..."
git checkout dev

echo "Checking if lib/$LIBRARY_NAME exists in dev branch..."
if [ ! -d "lib/$LIBRARY_NAME" ]; then
  echo "Error: lib/$LIBRARY_NAME does not exist in the dev branch."
  exit 1
fi

echo "Removing everything except files specified..."
find . -mindepth 1 \
  -name '.git' -prune -o \
  -name 'README.md' -prune -o \
  -name '.gitignore' -prune -o \
  -path './lib' -prune -o \
  -exec rm -rf {} +

echo "Moving library contents to the root directory..."
mv lib/"$LIBRARY_NAME"/* . || { echo "Error: Failed to move files from lib"; exit 1; }
rm -rf lib

echo "Staging and committing changes..."
git add .
git commit -m "Update master branch from dev" || echo "No changes to commit"

echo "Force-pushing dev to master branch..."
git push origin dev:master --force

echo "Master branch updated successfully!"
