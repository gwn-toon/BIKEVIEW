# Fietsview Arduino Local Package

This folder contains the local Fietsview Arduino website, the Arduino controller sketch, the controller monitor, the map data, street-run videos, and bundled grid predictions needed to run the project on another Mac.

## Start The Website

1. Open this folder on the iMac.
2. Double-click `start-fietsview.command`.
3. Chrome or Edge should open:

```text
http://127.0.0.1:8080/fietsview-style-draft-arduino.html
```

If the browser does not open, keep the Terminal window running and paste that URL into Chrome or Edge.

## Start The Finish / Expo Website

Double-click `start-fietsview-finish.command`.

It opens:

```text
http://127.0.0.1:8080/fietsview-finish.html
```

This version shows one fullscreen start button, starts from a random street, and keeps the page in fullscreen during the controller force-reload combo.

## Connect The Controller

1. Upload `arcade/fietsview-controller/fietsview-controller.ino` to the Arduino.
2. Close Arduino Serial Monitor before using the website.
3. In the Fietsview page, click `Connect Controller`.
4. Pick the Arduino serial port.

## Button Mapping

```text
D5  -> joystick up
D3  -> joystick down
D4  -> joystick left
D2  -> joystick right
D7  -> confirm
D8  -> random street
D9  -> grid opacity
D10 -> video on/off
D11 -> info overlay
GND -> shared ground
```

## Test Page

Use this page to check the raw controller input:

```text
http://127.0.0.1:8080/arcade/joystick.html
```

## Notes

- Use Chrome or Edge for Web Serial.
- The included Python server supports MP4 range requests, which keeps the videos responsive.
- If upload fails with a missing port, unplug/replug the Arduino and select the current port in Arduino IDE under `Tools > Port`.
