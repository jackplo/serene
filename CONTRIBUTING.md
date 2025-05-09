# Contributing to Serene

Thank you for your interest in contributing to Serene! 🎉 Whether you're fixing bugs, improving the UI, or extending functionality — we welcome your help.

## 🛠️ Setting Up Your Development Environment

### 1. Clone the Repository

```bash
git clone https://github.com/jackplo/serene.git
cd serene
```

### 2. Install Dependencies

Ensure the following packages are installed:

```bash
- `clang++` or `g++`
- `make`
- GTK4 and gtkmm-4.0 development libraries
- Common GTK-related dependencies:
  - `glibmm-2.68`
  - `giomm-2.68`
  - `pangomm-2.48`
  - `cairomm-1.16`
  - `sigc++-3.0`
```

> If you use a different Linux distribution, install the equivalent packages via your package manager.

### 3. Build the Project

```bash
make
```

Run the app with:

```bash
./spotlight
```

Run tests with:

```bash
make test
```

Clean the build with:

```bash
make clean
```

## 💡 Tips for Contributors

- Stick to modern C++ (C++17) practices.
- Follow naming conventions used in the project.
- Write clean, readable, and modular code.
- Keep UI components in `src/components/`, logic in `src/core/`, and search backend in `src/backend/`.
- Test your changes before submitting a pull request.

## ✅ Submitting Changes

1. Fork the repository.
2. Create a feature branch: `git checkout -b feature/my-feature`.
3. Commit your changes: `git commit -am 'Add my feature'`.
4. Push to your fork: `git push origin feature/my-feature`.
5. Create a Pull Request from GitHub.

We'll review your PR and provide feedback or merge it in!

---

Thanks for contributing to serene!
