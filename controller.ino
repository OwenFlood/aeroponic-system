#include <Wire.h>
#include "RTClib.h"
#include <string.h>

RTC_DS3231 timer;

#define MANUAL_DRIVEN 0
#define CONTEXT_DRIVEN 1
#define MAX_TIMERS 10

#define SPRAY_DURATION 12 // Seconds
#define SPRAY_INTERVAL 30 // Minutes

struct Timer {
    DateTime time;
    int expired;
};

// Pin Definitions
int togglePumpIn = 7;
int togglePumpOut = 8;

// Global Variables
Timer currentTimers[MAX_TIMERS * sizeof(Timer)];
String oldTime;
DateTime pauseAnchor;
int timersActive = 1;
int timerIndex = 0;
int togglePumpDebounce = 0;
int recursiveMode = 1;

String formatDate(DateTime date);
int setTimer(DateTime time);
int expireTimer(int timerId);
int checkTimers(DateTime currentTime);
int isDuplicateTimer(DateTime time);
int pauseTimers();
int resumeTimers();
int clearTimers();
int beginCycle();
int beginRecursiveCycle();
void testTimer();

void setup() {
    pinMode(togglePumpIn, INPUT);
    pinMode(togglePumpOut, OUTPUT);
    
    Serial.begin(115200);
    delay(3000); // wait for console opening

    if (!timer.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (timer.lostPower()) { // Reset timer
        Serial.println("RTC lost power, lets set the time!");

        timer.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    for (int i = 0; i < MAX_TIMERS; i++) { // Fill timer array with invalid timer placeholders
        Timer newTimer;
        newTimer.expired = 1;

        currentTimers[i] = newTimer;
    }
    
}

void loop() {
    int togglePump = digitalRead(togglePumpIn);

    DateTime currentTimeRaw = timer.now();
    String currentTime = formatDate(currentTimeRaw);

    // Listen to button to add new timer
    if (togglePump == HIGH && togglePumpDebounce == 0) {
        testTimer();
        togglePumpDebounce = 1;
    } else if (togglePump == LOW && togglePumpDebounce == 1) {
        togglePumpDebounce = 0;
    }
    
    if (oldTime && currentTime.compareTo(oldTime)) { // Runs once per second
        // Serial.print(currentTime + "\n");
        // Serial.print("----------------------- \n");
        checkTimers(currentTimeRaw);
    }

    oldTime = currentTime;
}

String formatDate(DateTime date) {
    String minute = String(date.minute());
    String hour = String(date.hour());
    String second = String(date.second());
    return hour + ":" + minute + ":" + second + "\n";
}

int setTimer(DateTime time) {
    if (isDuplicateTimer(time)) {
        return 0;
    }

    for (int i = 0; i < MAX_TIMERS; i++) {
        if (currentTimers[i].expired) {
            Timer nextTimer = { time: time, expired: 0 };
            currentTimers[i] = nextTimer;
            break;
        }
    }

    return 1;
}

int expireTimer(int timerId) {
    currentTimers[timerId].expired = 1;

    return 1;
}

int pauseTimers() {
    pauseAnchor = timer.now();

    timersActive = 0;
}

int resumeTimers() {
    TimeSpan timeElapsed = timer.now() - pauseAnchor;
    if (!timersActive) { // Just in case resume is called while timers are active
        for (int i = 0; i < MAX_TIMERS; i++) {
            if (!currentTimers[i].expired) {
                currentTimers[i].time = currentTimers[i].time + timeElapsed;
            }
        }
    }

    timersActive = 1;
}

int clearTimers() {
    for (int i = 0; i < MAX_TIMERS; i++) {
        expireTimer(i);
    }
}

int checkTimers(DateTime currentTime) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!currentTimers[i].expired && currentTimers[i].time <= currentTime) {
            beginCycle();
            expireTimer(i);
        }
    }
    return 1;
}

int beginCycle() {
    Serial.print("Spraying Starting\n");
    digitalWrite(togglePumpOut, HIGH);
    delay(SPRAY_DURATION * 1000);
    Serial.print("Spraying Ending\n");
    digitalWrite(togglePumpOut, LOW);
    
    if (recursiveMode == 1) {
        setTimer(timer.now() + TimeSpan(SPRAY_INTERVAL * 60));
    }
}

void testTimer() {
    setTimer(timer.now() + TimeSpan(1));
    Serial.println("Timer Set");
}

int isDuplicateTimer(DateTime time) {
    int isDuplicate = 0;

    for (int i = 0; i < MAX_TIMERS; i++) {
        if (currentTimers[i].time == time) {
            isDuplicate = 1;
        }        
    }
    
    return isDuplicate;
}