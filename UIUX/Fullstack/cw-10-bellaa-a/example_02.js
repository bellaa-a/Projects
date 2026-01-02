function task(id) {
    console.log(`Task ${id}`);
}

function longTask(id) {
    console.log(`Task ${id} started`);
    for(let i = 0; i < 5e9; i++) {
        // do some important work
    }
    console.log(`Task ${id} finished`);
}

console.log("Listening to events!")
task(1);  // Task 1
longTask(2);  // Task 2
task(3);  // Task 3
task(4);  // Task 4
console.log("Still listening to events!")