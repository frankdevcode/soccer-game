# Git Workflow Guidelines

## Branch Strategy

### Main Branches
- **`main`**: Production-ready code (stable releases)
- **`develop`**: Integration branch for features

### Supporting Branches
- **`feature/*`**: New features (`feature/player-movement`)
- **`fix/*`**: Bug fixes (`fix/ball-physics-collision`)
- **`refactor/*`**: Code improvements (`refactor/ai-system`)
- **`perf/*`**: Performance work (`perf/rendering-optimization`)
- **`docs/*`**: Documentation (`docs/api-guide`)

## Workflow

### 1. Create Feature Branch
```bash
# From develop
git checkout develop
git pull origin develop
git checkout -b feature/your-feature-name
```

### 2. Make Changes
```bash
# Commit frequently with clear messages
git add <files>
git commit -m "[FEAT] Brief description of changes"

# Push to remote
git push origin feature/your-feature-name
```

### 3. Create Pull Request
- Target: `develop` (or `main` for hotfixes)
- Description: Clear explanation of changes
- Checklist: Complete PR template

### 4. Code Review
- Wait for approval from maintainers
- Address feedback
- Update commits if needed

### 5. Merge
```bash
# After approval, merge to develop
git checkout develop
git pull origin develop
git merge feature/your-feature-name
git push origin develop

# Delete feature branch
git push origin --delete feature/your-feature-name
git branch -d feature/your-feature-name
```

### 6. Release
```bash
# When develop is ready for release
git checkout main
git pull origin main
git merge --no-ff develop
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin main --tags
```

## Commit Message Rules

### Format
```
[CATEGORY] Brief description (50 chars max)

Detailed explanation (if needed)
- Bullet point 1
- Bullet point 2

Fixes #issue_number
```

### Categories
- `[INIT]` - Project initialization/setup
- `[FEAT]` - New feature
- `[FIX]` - Bug fix
- `[REFACTOR]` - Code refactoring
- `[PERF]` - Performance improvement
- `[DOCS]` - Documentation changes
- `[TEST]` - Testing updates
- `[STYLE]` - Code style/formatting
- `[CLEAN]` - Code cleanup

### Examples

#### Good Commits
```
[FEAT] Implement player movement system

- Add forward/backward movement control
- Implement strafe movement
- Add smooth acceleration/deceleration
- Create animation blending transitions

Fixes #42
```

```
[FIX] Fix ball physics collision detection

The ball was bouncing incorrectly against walls due to
incorrect collision box scaling. Updated collision system
to use proper world space calculations.

Fixes #87
```

```
[PERF] Optimize rendering pipeline

- Reduced draw calls by 40%
- Implemented frustum culling
- Added LOD system for distant objects

Performance improved from 45 FPS to 60 FPS stable.
```

#### Bad Commits
```
Fixed stuff
Update code
Changes
```

## Pull Request Guidelines

### Before Submitting
- [ ] Code follows [DEVELOPMENT_STANDARDS.md](../DEVELOPMENT_STANDARDS.md)
- [ ] Tests added/updated
- [ ] Documentation updated
- [ ] Commit messages follow format
- [ ] No merge conflicts
- [ ] All CI/CD checks pass

### Description Template
```markdown
## Description
Clear explanation of what was changed and why.

## Type of Change
- [x] New feature
- [ ] Bug fix
- [ ] Performance improvement
- [ ] Refactoring
- [ ] Documentation

## Related Issue
Fixes #123

## Testing
How to test these changes.

## Checklist
- [x] Code follows standards
- [x] Tests added
- [x] Documentation updated
```

## Continuous Integration

### Automatic Checks
All PRs automatically run:
- Build verification
- Code quality checks
- Documentation validation
- Requirements tracking

### Merge Requirements
- ✅ All CI/CD checks passing
- ✅ Code review approved
- ✅ No conflicts with base branch
- ✅ Commit message format correct

## Versioning

### Semantic Versioning: MAJOR.MINOR.PATCH

- **MAJOR**: Breaking changes (1.0.0 → 2.0.0)
- **MINOR**: New features, backward compatible (1.0.0 → 1.1.0)
- **PATCH**: Bug fixes (1.0.0 → 1.0.1)

### Tagging
```bash
# Create release tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# Create pre-release tag
git tag -a v1.0.0-beta.1 -m "Beta release"
```

## Collaboration Guidelines

### Code Review
- Respect reviewer's time
- Provide context for changes
- Be open to feedback
- Respond to comments promptly

### Resolving Conflicts
```bash
# Fetch latest changes
git fetch origin

# Rebase on latest develop
git rebase origin/develop

# Resolve conflicts
# Then force push
git push origin feature/your-feature -f
```

### Keeping Branch Updated
```bash
# Regular rebase to avoid conflicts
git fetch origin
git rebase origin/develop
git push origin feature/your-feature -f
```

## Best Practices

1. **Commit Often**: Small, logical commits are better than large ones
2. **Clear Messages**: Write descriptive commit messages
3. **Test Locally**: Ensure changes work before pushing
4. **Review Your Own Code First**: Catch obvious issues
5. **Respect CI/CD**: Fix failing checks before merge
6. **Document Changes**: Update docs alongside code
7. **Communicate**: Discuss complex changes in PR comments
8. **Keep Branches Updated**: Regular rebases prevent conflicts

---

**Last Updated**: 2026-05-26
