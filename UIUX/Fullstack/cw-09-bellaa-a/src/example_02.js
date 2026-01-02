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

task(1);
longTask(2);
task(3);
task(4);