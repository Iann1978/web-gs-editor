# GitHub Pages Manual Deployment

## Overview

Step-by-step guide to manually deploy the Vite + Vue 3 application to GitHub Pages, matching the result at `http://localhost:5173/`.

## Prerequisites

- Git repository initialized and connected to GitHub
- Node.js and npm installed
- Project dependencies installed (`npm install`)

## Step 1: Configure Vite Base Path

If your repository is `username.github.io/repo-name`, update `vite.config.ts`:

```typescript
export default defineConfig({
  base: '/repo-name/',
  plugins: [vue()],
})
```

If repository is `username.github.io` (root), skip this step.

## Step 2: Build the Project

```bash
npm run build
```

This creates the `dist/` folder with production files.

## Step 3: Create gh-pages Branch

```bash
git checkout --orphan gh-pages
git rm -rf .
```

This creates a new orphan branch (no history) and removes all files.

## Step 4: Copy Build Files

```bash
copy dist\* .
```

On Windows, this copies all files from `dist/` to the root of the branch.

## Step 5: Commit and Push

```bash
git add .
git commit -m "Deploy to GitHub Pages"
git push -u origin gh-pages
```

## Step 6: Enable GitHub Pages

1. Go to repository Settings → Pages
2. Under "Source", select branch: `gh-pages`
3. Select folder: `/ (root)`
4. Click Save

## Step 7: Verify Deployment

Visit `https://username.github.io/repo-name/` (or `https://username.github.io/` for root).

Wait 1-2 minutes for GitHub to build. The site should match `http://localhost:5173/`.

## Troubleshooting

**404 Errors:** Verify `base` path in `vite.config.ts` matches repository name.

**Assets Not Loading:** Ensure `base` path includes trailing slash: `/repo-name/`

**Update Deployment:** Repeat Steps 2-5 after making changes.

