function getId(student, callback) {
    console.log(`Fetch ${student} id`)
    let id;
    
    setTimeout(() => {
        id = "jdoe123";
        console.log(`Got ${student} id`)
        callback(id);
    }, 5000);
}

function getCourses(student_id, callback) {
    console.log(`Fetch ${student_id} courses`)
    let courses;
    
    setTimeout(() => {
        courses = ["course-1", "course-2"];
        console.log(`Got ${student_id} courses`)
        callback(courses);
    }, 5000);
}

function getGrades(student_id, student_courses, callback) {
    console.log(`Fetch ${student_id} grades`)
    let grades;
    
    setTimeout(() => {
        grades = student_courses.map((course) => {
            return {
                course,
                grade: Math.floor(Math.random() * 100)
            }
        })
        console.log(`Got ${student_id} grades`)
        callback(grades);
    }, 5000);
}

console.log("Listening to events");

getId("John Doe", (id) => {
    getCourses(id, (courses) => {
        getGrades(id, courses, (grades) => {
            console.log(grades);
        });
    });
});

console.log("Still listening to events");

// callback pattern

// promise pattern

// async/await pattern