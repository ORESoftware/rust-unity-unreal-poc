#!/bin/bash
set -e

ORG="unreal-unity-poc"
BASE_DIR="/Users/maca5/codes/unreal-unity-poc/rust-unity-unreal-poc"

# The specific repos to migrate based on the README
REPOS=(
  "bevy" "browserffi" "cef" "cocos2d-x" "cryengine" "defold" "dioxus" 
  "electron-abi" "electron-wasm" "flax" "godot" "leptos" "monogame" 
  "o3de" "qt" "rust-engine" "stride" "tauri" "unity" "unreal" 
  "v8-blink" "wasm" "webview2"
)

cd "$BASE_DIR"

for repo in "${REPOS[@]}"; do
  echo "Processing $repo..."
  if [ -d "$repo" ]; then
    # Create the repo in the GitHub org
    echo "Creating GitHub repo $ORG/$repo"
    gh repo create "$ORG/$repo" --public --confirm || true
    
    # Initialize git and push
    cd "$repo"
    
    # Check if already a git repo
    if [ ! -d ".git" ]; then
      git init
      git add .
      git commit -m "Initial commit from monorepo split"
      git branch -M main
    fi
    
    # Set remote and push
    git remote remove origin 2>/dev/null || true
    git remote add origin "https://github.com/$ORG/$repo.git"
    echo "Pushing to $ORG/$repo"
    git push -u origin main
    
    cd ..
  else
    echo "Directory $repo not found, skipping."
  fi
done

echo "Done!"
