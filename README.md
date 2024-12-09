# helpingHands
In 2018 my mom and I moved from Colorado to New Mexico after her and my father divorced. It was a great new start for both of us, but she fell ill in 2019, she suffered a major heart attack, leaving her with a brain injury which makes it hard for her to remember things, especially short term. My mom has always been strong, she took care of me and my brother, and she also took care of my grandmother, I learned from my mom at a young age that we care for our elders as they did for us. So she now lives with my wife and I and my mother in law Lisa and our three dogs.
The struggle to help my mom remember simple tasks is hard, sometimes, she remembers some things more then other times, and sometimes I forget to remind her of things and I'm left worrying about her.
My solution? Create a helping hands buddy for her that is both helpful, interactive and fun for her to use.
# Program
This program is written in C++ for particle IDE using the Photon2 microcontroller. 
This program realise on a SQL database that pushes scheduled information to the device using the cloud.
# Functionality:
The device syncs with Particle cloud for accurate time
Checks for timer events (e.g. every minute) and time-of-day events
Displays reminders on the screen when they are due
Uses neopixels to visually indicate which reminder is active
Allows executing custom tasks when reminders trigger
# Key Features:
Displays current time and date
Shows reminders at set times throughout the day
Uses neopixels to indicate which reminder is active
Allows executing custom tasks when reminders trigger
# The Device Features
Adafruit ILI9341 touchscreen display
Adafruit NeoPixels for LED indicators
Particle cloud integration for time synchronization
Custom reminder system with scheduled events
# Device abilities
Can operate devices connected to WeMo plugs, display time and date, schedule, neopixels light up with indications, software timers are used to display reminders throughout the day. 

# Includes and definitions
Global variables
Constants
Objects and structs for events and reminders
Display functions
Time-related functions
Reminder and task execution functions
Main loop logic
# Key Functions:
updateTimeAndDate(): Updates the displayed time and date
checkTimerEvents(): Checks if any timer-based reminders are due
checkTimeOfDayEvents(): Checks if any time-of-day based reminders are due
executeTask(): Executes a reminder task when triggered
updatePixelState(): Updates the neopixel color to indicate which reminder is active
