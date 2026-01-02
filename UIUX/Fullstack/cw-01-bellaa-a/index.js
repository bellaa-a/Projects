// const now = new Date();
const fallAsleepTime = new Date();
fallAsleepTime.setMinutes(fallAsleepTime.getMinutes() + 14);

// console.log(now);
// console.log(now.toLocaleString("en-US"));
// console.log(now.toLocaleDateString("en-US"));
// console.log(now.toLocaleTimeString("en-US"));

const config = {
    timeStyle: "short"
}

console.log("You will fall asleep", fallAsleepTime.toLocaleTimeString("en-US", config));

const wakeUpTime = new Date(fallAsleepTime);
const wakeUpTimes = [];

for (let i = 1; i <= 6; i++) {
    wakeUpTime.setMinutes(wakeUpTime.getMinutes() + 90);
    const wakeUpTimeString = wakeUpTime.toLocaleTimeString("en-US", config);
    wakeUpTimes.push(wakeUpTimeString);
}

console.log(`Wake up times are: `, wakeUpTimes);