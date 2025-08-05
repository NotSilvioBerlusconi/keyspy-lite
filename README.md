# keyspy-lite

🔑 A minimalist Linux keylogger in C that captures keystrokes by reading from `/dev/input/eventX`. Built for educational and ethical purposes only.

## 🚀 Features
- Reads low-level input events from `/dev/input/eventX`
- Converts key codes into readable ASCII characters
- Logs keystrokes into `keylog.txt`
- Lightweight, dependency-free C code (uses only libc)

## 📦 Build & Run

### 🔧 Prerequisites
- Linux system with `/dev/input/`devices (e.g., a VM)
- GCC (or Clang)
- Root access (`sudo`required to read input devices)

### 🛠️ Build
```bash
gcc -o keyspy keyspy.c
```

## Disclaimer
This project is for **educational and research purposes only**. 
- Do **not** use this software outside of isolated environments (e.g. virtual machines).
- I am **not responsible** for any damage, data loss, or legal consequences resulting from misuses.
- By using this code, you agree to use it **at your own risk** and only for **ethical und lawful purposes**. 
