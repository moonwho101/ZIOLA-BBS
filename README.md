# ZIOLA BBS

> A fully featured Bulletin Board System (BBS) from the early 1990s — written in Borland C for MS-DOS by Mark Longo.

[![Language](https://img.shields.io/badge/language-C%20%28Borland%29-blue.svg)](https://en.wikipedia.org/wiki/Borland_C%2B%2B)
[![Platform](https://img.shields.io/badge/platform-MS--DOS-lightgrey.svg)](https://en.wikipedia.org/wiki/MS-DOS)
[![Era](https://img.shields.io/badge/era-1990s-orange.svg)](https://en.wikipedia.org/wiki/Bulletin_board_system)
[![License](https://img.shields.io/github/license/moonwho101/ZIOLA-BBS)](LICENSE)

ZIOLA BBS is a retro dial-up bulletin board system that showcases the full stack of 1990s BBS software engineering: modem control, ANSI terminal graphics, multi-area message threading, Zmodem file transfer, door game integration, and SysOp administration — all in C, running bare-metal on MS-DOS.

---

## Table of Contents

- [Screenshots](#screenshots)
- [Features](#features)
- [Project Structure](#project-structure)
- [Building](#building)
- [Running](#running)
- [Historical Context](#historical-context)
- [Contributing](#contributing)
- [License](#license)

---

## Screenshots

<img src="https://github.com/moonwho101/ZIOLA-BBS/blob/main/ZIOLA/SCREEN/ziola1.jpg" width="800" alt="ZIOLA BBS main screen">
<img src="https://github.com/moonwho101/ZIOLA-BBS/blob/main/ZIOLA/SCREEN/ziola2.jpg" width="800" alt="ZIOLA BBS in action">

---

## Features

| Category | Details |
|---|---|
| **User Accounts** | Registration, security levels, ANSI preferences, transfer stats |
| **Message Areas** | Multiple boards with post / read / reply / delete, private threads, new-mail scanning |
| **Message Editor** | Built-in ANSI-aware line editor for composing and replying |
| **File Areas** | Categorized libraries with descriptions, tagging, queues, and new-file highlights |
| **File Transfers** | Upload/download queue with **DSZ/Zmodem** support and stat tracking |
| **Door Games** | External door launch via standard DORINFO drop files and door counters |
| **SysOp Chat** | Live break-in chat plus maintenance tools for users, messages, and files |
| **Time Limits** | Per-session timers with countdown warnings and usage tracking |
| **ANSI Graphics** | Color menus, art screens, and optional non-ANSI (plain ASCII) mode |
| **Modem Management** | Auto-init, baud detection, carrier monitoring, local/remote handling |

---

## Project Structure

```
ZIOLA/
├── ELWING.C        # Main entry point — BBS configuration and startup
├── MENU.C / .H     # Menu engine and routing
├── MESSAGE.C       # Message area post/read/reply
├── EDIT.C          # Line editor for message composition
├── FILE.C / .H     # File area browsing and management
├── MODEM.C / .H    # Modem initialization and carrier control
├── SERIAL.H        # Serial port I/O definitions
├── DANSI.H         # ANSI terminal escape handling
├── DRIVER.C        # Low-level hardware driver
├── TOOLS.C         # SysOp utilities and maintenance
├── SHELL.C         # DOS shell / door game launcher
├── SORT.C          # File/user list sorting
├── GINPUT.C / .H   # Input handling (keyboard + modem)
├── RMESS.C / .H    # Read-message routines
├── WMESS.H         # Write-message definitions
├── MISC.H          # Shared structures and constants
├── MAIN.H          # Global function prototypes
├── DORINFO1.DEF    # Sample DORINFO door drop file
├── MENU/           # Pre-built ANSI and ASCII menu art screens
├── MESS/           # Message area data directory
├── SCREEN/         # Screenshot assets
├── USERS/          # User account data directory
└── SRE/            # Supporting scripts and resources
```

---

## Building

ZIOLA BBS was written for **Borland C++ / Turbo C** targeting MS-DOS real-mode. The original project files are included:

| File | Purpose |
|---|---|
| `BBS.PRJ` | Turbo C project file |
| `BBS.IDE` | Borland C++ IDE project |
| `BBS.DSW` / `BBS.DSK` | Borland project workspace/desktop state |


### Option A — Original toolchain (DOSBox + Borland C++ 3.1)

1. Install [DOSBox](https://www.dosbox.com/) or [DOSBox-X](https://dosbox-x.com/).
2. Mount the workspace and a copy of Borland C++ 3.1.
3. Open `BBS.PRJ` in the Turbo C IDE, or run:
   ```

---

## Running

1. Run `BBS.EXE` from the BBS root directory inside DOS or DOSBox.

---

## Historical Context

Bulletin Board Systems (BBSs) were the internet of the pre-web era. Callers dialed in via modem, browsed message boards, downloaded files, played door games, and chatted with the SysOp. ZIOLA BBS represents the kind of hand-crafted, single-author C software that powered thousands of small community BBSs throughout the late 1980s and early 1990s before the public internet displaced them.

Key technologies preserved here:
- **RS-232 serial / modem AT commands** — direct hardware port programming
- **ANSI escape sequences** — color terminal art before GUIs
- **Zmodem (DSZ)** — error-correcting file transfer over noisy phone lines
- **DORINFO drop files** — the standard interface for launching door games
- **Real-mode DOS C** — memory models, `far` pointers, and interrupt-level I/O

---

## License

See [LICENSE](LICENSE) for details.

