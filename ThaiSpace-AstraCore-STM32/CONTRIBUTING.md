# Contributing to AstraCore

Thank you for your interest in contributing to the ThaiSpace AstraCore Framework!

## 🛠️ Development Workflow

### 1. Branching Policy
- `main`: Stable releases only.
- `develop`: Integration branch for new features.
- `feature/*`: Specific feature development.
- `fix/*`: Bug fixes.

### 2. Code Standards
- Adhere to the existing C style used in the project.
- Use `cppcheck` locally before pushing to ensure static analysis passes.
- Documentation for new modules should be included in their respective folders.

### 3. Commit Messages
We follow conventional commits:
- `feat:` for new features.
- `fix:` for bug fixes.
- `docs:` for documentation changes.
- `chore:` for maintenance tasks.

## 🚀 Testing Requirements
Every pull request must pass the automated CI Pipeline, which includes:
1. Compilation without warnings.
2. Successful execution on target hardware (HIL Test).

**Pro-tip:** Before pushing, it is highly recommended to run HIL tests locally. Refer to the [HIL Testing Guide](HIL_TESTING_GUIDE.md) for local environment setup and reproduction steps.

## 📞 Reporting Issues
Please use the GitLab Issue tracker to report bugs or request new features.
