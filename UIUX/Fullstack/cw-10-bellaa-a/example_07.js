function getId(student) {
    return new Promise((resolve, reject) => {
        console.log(`Fetch ${student} id`)
        let id;
        let success;

        setTimeout(() => {
            success = false;
            id = "jdoe123";
            
            if (success) {
                console.log(`Got ${student} id`)
                resolve(id);
            } else {
                const error = new Error(`Unable to fetch ${student} id`)
                reject(error);
            }
        }, 1000);
    });
}

function getCourses(student_id) {
    // console.log(`Fetch ${student_id} courses`)
    // let courses;
    
    // setTimeout(() => {
    //     courses = ["course-1", "course-2"];
    //     console.log(`Got ${student_id} courses`)
    //     callback(courses);
    // }, 1000);
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

