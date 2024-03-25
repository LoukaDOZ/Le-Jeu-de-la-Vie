# Le jeu de la vie / Conway's game of life

![Capture d’écran du 2024-03-25 16-09-37](https://github.com/LoukaDOZ/Le-Jeu-de-la-Vie/assets/46566140/c812abf2-a168-4ff0-8a4e-492816a56140)

## Usage

### Makefile

| Command          | Description                                 |
| ---------------- | ------------------------------------------- |
| `make install`   | Install SDL2 and SDL2 TTF libraries         |
| `make uninstall` | Uninstall SDL2 and SDL2 TTF libraries       |
| `make build`     | Compile project                             |
| `make run`       | Build and run project (with default config) |
| `make clean`     | Clean compiled files                        |

### Manual run

```bash
make install
make build
./exec
```

Arguments :
| Argument                         | Description                                                           | Type                  | Default                  |
| -------------------------------- | --------------------------------------------------------------------- | --------------------- | ------------------------ |
| `-h, --help`                     | Show help                                                             |                       |                          |
| `-p, --probability <value>`      | Probability for a cell to be alive when filling board at startup in % | int (>= 0 and <= 100) | 50                       |
| `-r, --framerate <value>`        | Simulation refresh rate in times per seconds                          | int (> 0)             | 25                       |
| `-c, --cell-size <value>`        | Cell size in pixels                                                   | int (> 0)             | 10                       |
| `--window-size=<width>x<height>` | Window size in pixels                                                 | "intxint" (int > 0)   | 500x500                  |
| `-i, --no-interactive`           | Disable interactible board                                            |                       | Active                   |
| `-g, --show-generation`          | Show generation count                                                 |                       | Hidden                   |
| `-f, --fullscreen`               | Fullscreen window                                                     |                       | Value of `--window-size` |


### In game controls

| Control              | Description                |
| -------------------- | -------------------------- |
| `P`                  | Pause/resume simulation    |
| `G`                  | Show/hide generation count |
| `C`                  | Clear simulation           |
| `Q`                  | Quit                       |
| `Mouse left button`  | Add cell                   |
| `Mouse right button` | Remove cell                |

# Example

Default configuration:
```bash
./exec
```

All arguments with a window size of 1000x700:
```bash
./exec -p 70 -r 30 -c 50 --window-size=1000x700 -i -g
```

All arguments with fullscreen:
```bash
./exec -p 70 -r 30 -c 50 -i -g -f
```
