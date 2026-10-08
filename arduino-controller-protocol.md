# Arduino Controller Protocol

Use `fietsview-style-draft-arduino.html` through a local server in Chrome or Edge. Click `Connect Controller`, choose the Arduino serial port, then send one newline-terminated message per input change. The page now matches the sketches in `arcade/` and opens the port at `9600` baud.

Fietsview has a small top-right controller indicator. In fullscreen it shows the raw serial line, parsed direction, matching arrow status, and last button action.

The current joystick sketch already works:

```text
0 NEUTRAL
8 UP
9 UP-RIGHT
10 UP-LEFT
4 DOWN
5 DOWN-RIGHT
6 DOWN-LEFT
2 LEFT
1 RIGHT
```

Joystick direction mapping in Fietsview:

```text
UP         -> forward arrow
DOWN       -> back arrow
LEFT       -> left arrow
RIGHT      -> right arrow
UP-RIGHT   -> right-forward arrow
UP-LEFT    -> left-forward arrow
DOWN-RIGHT -> right-back arrow
DOWN-LEFT  -> left-back arrow
NEUTRAL    -> clear selected arrow
```

The combined Fietsview sketch starts by printing this firmware line:

```text
FW:FIETSVIEW_CONTROLLER_V7 D7=confirm D8=random D9=grid-opacity D10=video D11=info
```

All five button inputs use a short `8ms` debounce and a `120ms` per-button lockout. That keeps quick taps responsive while blocking most extra bounce clicks that can double-toggle video or confirm.

For the actual Fietsview setup, use one Arduino and upload:

```text
arcade/fietsview-controller/fietsview-controller.ino
```

Pins in that combined sketch:

```text
D5 -> joystick up
D3 -> joystick down
D4 -> joystick left
D2 -> joystick right
D7 -> confirm button
D8 -> random button
D9 -> grid opacity button
D10 -> video on/off button
D11 -> info button
GND -> shared ground for all switches/buttons
```

The confirm button should connect between `D7` and `GND`. The optional random button should connect between `D8` and `GND`. The grid opacity button should connect between `D9` and `GND`. The video toggle button should connect between `D10` and `GND`. The info button should connect between `D11` and `GND`. Do not reuse `D2`, `D3`, `D4`, or `D5` for buttons because those pins are already direction inputs.

Current button mapping:

```text
D7  -> confirm selected turn / start if idle
D8  -> random street
D9  -> toggle grid opacity between 100% and 50%
D10 -> toggle video background on/off
D11 -> toggle controller info overlay
```

The page also still accepts this cleaner output if you later change the sketch:

```text
DIR:left-forward
DIR:forward
DIR:center
BTN:confirm
BTN:start
BTN:random
BTN:video
BTN:grid-opacity
BTN:reset
BTN:play
BTN:fullscreen
BTN:info
```

Supported directions:

```text
forward
back
left
right
left-forward
right-forward
left-back
right-back
center
```

Supported button actions:

```text
confirm
start
random
video
grid-opacity
reset
play
fullscreen
handlebar
heading
info
```

The page also accepts compact aliases like `up`, `down`, `NE`, `SW`, `BTN:A`, `button1=1`, and JSON lines such as:

```json
{"dir":"right-forward","confirm":true}
```

Recommended Arduino sketch shape:

```cpp
void sendDirection(const char *direction) {
  static String lastDirection = "";
  if (lastDirection == direction) return;
  lastDirection = direction;
  Serial.print("DIR:");
  Serial.println(direction);
}

void sendButton(const char *button) {
  Serial.print("BTN:");
  Serial.println(button);
}
```

Use `Serial.begin(9600);` to match the current page.
