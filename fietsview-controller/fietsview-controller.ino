const int upPin = 5;
const int downPin = 3;
const int leftPin = 4;
const int rightPin = 2;
const int confirmButtonPin = 7;
const int randomButtonPin = 8;
const int gridOpacityButtonPin = 9;
const int videoToggleButtonPin = 10;
const int infoButtonPin = 11;

const unsigned long buttonDebounceMs = 8;
const unsigned long buttonPressLockoutMs = 120;
const unsigned long directionDebounceMs = 45;

int lastDirectionReading = -1;
int stableDirectionState = -1;
unsigned long lastDirectionChangeMs = 0;
bool lastConfirmReading = HIGH;
bool stableConfirmState = HIGH;
unsigned long lastConfirmChangeMs = 0;
unsigned long lastConfirmPressMs = 0;
bool lastRandomReading = HIGH;
bool stableRandomState = HIGH;
unsigned long lastRandomChangeMs = 0;
unsigned long lastRandomPressMs = 0;
bool lastVideoToggleReading = HIGH;
bool stableVideoToggleState = HIGH;
unsigned long lastVideoToggleChangeMs = 0;
unsigned long lastVideoTogglePressMs = 0;
bool lastGridOpacityReading = HIGH;
bool stableGridOpacityState = HIGH;
unsigned long lastGridOpacityChangeMs = 0;
unsigned long lastGridOpacityPressMs = 0;
bool lastInfoReading = HIGH;
bool stableInfoState = HIGH;
unsigned long lastInfoChangeMs = 0;
unsigned long lastInfoPressMs = 0;

const char* dirName(int state) {
  switch (state) {
    case 0b1000: return "UP";
    case 0b0100: return "DOWN";
    case 0b0010: return "LEFT";
    case 0b0001: return "RIGHT";
    case 0b1001: return "UP-RIGHT";
    case 0b1010: return "UP-LEFT";
    case 0b0101: return "DOWN-RIGHT";
    case 0b0110: return "DOWN-LEFT";
    case 0b0000: return "NEUTRAL";
    default:     return "INVALID";
  }
}

void setup() {
  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(confirmButtonPin, INPUT_PULLUP);
  pinMode(randomButtonPin, INPUT_PULLUP);
  pinMode(videoToggleButtonPin, INPUT_PULLUP);
  pinMode(gridOpacityButtonPin, INPUT_PULLUP);
  pinMode(infoButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
  delay(300);
  Serial.println("FW:FIETSVIEW_CONTROLLER_V7 D7=confirm D8=random D9=grid-opacity D10=video D11=info");
}

void loop() {
  bool u = digitalRead(upPin) == LOW;
  bool d = digitalRead(downPin) == LOW;
  bool l = digitalRead(leftPin) == LOW;
  bool r = digitalRead(rightPin) == LOW;

  int directionState = (u << 3) | (d << 2) | (l << 1) | r;
  if (directionState != lastDirectionReading) {
    lastDirectionChangeMs = millis();
    lastDirectionReading = directionState;
  }

  if (millis() - lastDirectionChangeMs > directionDebounceMs && directionState != stableDirectionState) {
    stableDirectionState = directionState;
    Serial.print(stableDirectionState);
    Serial.print(" ");
    Serial.println(dirName(stableDirectionState));
  }

  handleButton(confirmButtonPin, lastConfirmReading, stableConfirmState, lastConfirmChangeMs, lastConfirmPressMs, "BTN:confirm");
  handleButton(randomButtonPin, lastRandomReading, stableRandomState, lastRandomChangeMs, lastRandomPressMs, "BTN:random");
  handleButton(videoToggleButtonPin, lastVideoToggleReading, stableVideoToggleState, lastVideoToggleChangeMs, lastVideoTogglePressMs, "BTN:video");
  handleButton(gridOpacityButtonPin, lastGridOpacityReading, stableGridOpacityState, lastGridOpacityChangeMs, lastGridOpacityPressMs, "BTN:grid-opacity");
  handleButton(infoButtonPin, lastInfoReading, stableInfoState, lastInfoChangeMs, lastInfoPressMs, "BTN:info");

  delay(5);
}

void handleButton(
  int pin,
  bool &lastReading,
  bool &stableState,
  unsigned long &lastChangeMs,
  unsigned long &lastPressMs,
  const char *message
) {
  bool reading = digitalRead(pin);

  if (reading != lastReading) {
    lastChangeMs = millis();
    lastReading = reading;
  }

  if (millis() - lastChangeMs > buttonDebounceMs && reading != stableState) {
    stableState = reading;
    if (stableState == LOW) {
      unsigned long nowMs = millis();
      if (lastPressMs == 0 || nowMs - lastPressMs >= buttonPressLockoutMs) {
        lastPressMs = nowMs;
        Serial.println(message);
      }
    }
  }
}
