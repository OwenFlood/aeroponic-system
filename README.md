
# Aeroponic System :seedling:

"Where we're going, we don't need soil" :shipit:

## Description

This project is my attempt to build a cheap alternative to the current options for at-home aeroponic systems that can cost between $200 and over $1000.
That feels expensive, for a microcontroller, pump, and some plastic. One improvement I am looking to make is removing the requirement to pre-sprout the seeds before adding them to the system. I believe the need for constant intervention from the user deters many potential users who simply want fresh vegetables but do not want a hobby. The beauty in at-home farming is that it transfers the power of affordable nutritious food from big businesses to the people who need it most.

## Current Iteration

This is hopefully the lowest fidelity version that I will have going forward, it is running on an Arduino Uno with a Real Time Clock module to
manage the timing of the pump. The pump is a regular Home Depot fountain pump and seems extremely overpowered, future versions can probably
have less expensive pumps. The PVC pipes are not glued for easy troubleshooting and configuration changes which leads to leaks, once a configuration
is decided the pipes will be glued. Due to not having the proper sized holesaw, the openings for the seeds have some minor leaks, but can be
fixed by simply purchasing a new set of holesaws.

| Day 1 | Day 7 | Day 9 |
| :----: | :----: | :----: |
| <img src="media/lettuce-day-1.jpg" alt="Lettuce Day One" height=200 /> | <img src="media/lettuce-day-7.jpg" alt="Lettuce Day One" height=200 /> | <img src="media/lettuce-day-9.jpg" alt="Lettuce Day One" height=200 /> |

## Future Iterations
- I am currently working to add support for temperature, water level, and pH sensors which will provide useful data
- A CAD model for an acryllic, laser-cut version is partially done, and will be significantly cheaper and easier to modify
- Consumer versions would need a user interface through either a touch screen or a mobile app (an app would likely use bluetooth)
- The ability to control the watering of specific plants at different intervals would allow multiple species to grow together
