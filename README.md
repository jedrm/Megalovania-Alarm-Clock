# Megalovania Alarm Clock
 
![Image of the alarm clock](https://i.imgur.com/cBfMdTH.png)

Using an Arduino MEGA2560, I created an alarm clock using an LCD display, LED matrix, passive buzzer, 2 buttons, and a real time clock component in the circuit.

The real time clock component was used to keep track of the time and the date in the circuit. The LCD display was used to print out the current date and time on the screen. The two buttons were used to set the wanted the desired alarm time for the alarm clock. The whole project is in a forever loop that checks whether the current hour and minute both matches the hour alarm and the minute alarm. If the conditions are met, the LED matrix flashes the phrase "WAKE UP" and the first four measures of the song "Megalovania" from the Undertale soundtrack plays and a skull flashes 3 times as a reference to one of the characters from the game named Sans.

Only two buttons were used because I figured that it was the least amount of buttons necessary to change the alarm instead of having four buttons: one for setting the alarm, one for changing the hour alarm, another for changing the minute alarm, and the last one to exit the alarm settings.
