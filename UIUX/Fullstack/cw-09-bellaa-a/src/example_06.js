function getId(student) {
    return new Promise((resolve, reject) => {
        console.log(`Fetch ${student} id`)
        let id;
        let success;

        // go and try to get the ID of the student
        setTimeout(() => {
            success = true;
            id = "jdoe123";
            
            if (success) {
                console.log(`Got ${student} id`)
                resolve(id);
            } else {
                const error = new Error(`Unable to fetch ${student} id`)
                reject(error);
            }
        }, 5000);
    });
}

console.log("Listening to events");

getId("John Doe")
    .then((id) => {
        console.log(id)
    })
    .catch((err) => {
        console.error(err)
    })

console.log("Still listening to events");

